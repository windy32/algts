set ns [new Simulator]
set startTime 0.0
set stopTime 100

global defaultRNG
$defaultRNG seed 12345
ns-random 12345

# Parse command line
if {$argc == 6} {
    set num_ftps [lindex $argv 0]
    set web_rate [lindex $argv 1]
    set bw [bw_parse [lindex $argv 2]]
    set pkt_size [lindex $argv 3]
    set pct [lindex $argv 4]
    set queue_len [lindex $argv 5]
} else {
    puts "Usage:"
    puts "  ns link.tcl num_ftps web_rate bandwidth packet_size percentage queue_length"
    puts "Example:"
    puts "  ns link.tcl 1 0 4Mb 1500 0.85 2"
    puts "  (One ftp session, no web traffic, downlink rate is 1Mbps, 1.5 KB packets, 15% of the"
    puts "   bandwidth is reserved, and the queue length of the token bucket is 2 packets)"
    puts "Notes:"
    puts "  The queue length should not be set below 2 packets"
    puts "  The packet size includes IP header and TCP header"
    exit 0
}

# Set nominal rtt to 100ms
set nominal_rtt 0.1
set bdp [expr round($bw * $nominal_rtt / (8 * $pkt_size))]

# TCP settings
Agent/TCP set segsize_ [expr $pkt_size - 40]
Agent/TCP set packetSize_ [expr $pkt_size - 40]
Agent/TCP set window_ [expr $bdp * 16]
Agent/TCP set windowInit_ 4

Agent/TCP/FullTcp set segsize_ [expr $pkt_size - 40]
Agent/TCP/FullTcp set packetSize_ [expr $pkt_size - 40]
Agent/TCP/FullTcp set window_ [expr $bdp * 16]
Agent/TCP/FullTcp set windowInit_ 4

# Build topology
#                <--------------------------------------------
#                <-----------  Web / FTP Traffic  ------------
#                <--------------------------------------------      +---------o Server 1
#                                                                   |
#     o----------------o================================o-----------+---------o Server 2
# Client(n0)       Gateway(n1)      bottleneck       ISP(n2)        |             ...
#                   10ms queue                     500ms queue      +---------o Server n
proc setup_scenario {} {
    global ns bw pkt_size pct num_ftps web_rate queue_len
    global n0 n1 n2
    
    set n0 [$ns node]
    set n1 [$ns node]
    set n2 [$ns node]
    
    set local_bw [expr $bw * $pct]
    set local_queue $queue_len
    set isp_queue [expr round($bw * 0.5 / (8 * $pkt_size))]

    puts "Setting up scenario..."
    puts [format "   - Bandwidth = %.1f Mbps" [expr $bw / 1000000.0]]
    puts [format "   - FTP sessions = %d" $num_ftps]
    puts [format "   - Used = %d%%" [expr round($pct * 100.0)]]
#   puts [format "   - ISP queue = %d" $isp_queue]
    puts [format "   - Local queue = %d" $local_queue]
    puts "Running simulation..."
    
    $ns duplex-link $n0 $n1 $local_bw 5ms DropTail
    $ns duplex-link $n1 $n2 $bw 5ms DropTail
    $ns queue-limit $n2 $n1 $isp_queue
    $ns queue-limit $n1 $n0 $local_queue

    for {set i 1} {$i <= $num_ftps} {incr i 1} {
        set nx [$ns node]
        $ns duplex-link $n2 $nx [expr $bw * 10] 100ms DropTail
        add_ftp $nx $n0 $i
    }

    if {$web_rate > 0} {
        set nx [$ns node]
        $ns duplex-link $n2 $nx [expr $bw * 10] 100ms DropTail
        add_web $n0 $nx $web_rate
    }
}

# Add ftp
proc add_ftp {sender receiver flow_id} {
    global ns
    set tcp [new Agent/TCP/Newreno]
    set sink [new Agent/TCPSink]
    $ns attach-agent $sender $tcp
    $ns attach-agent $receiver $sink
    $ns connect $tcp $sink
    $tcp set fid_ $flow_id
    
    set ftp [new Application/FTP]
    $ftp attach-agent $tcp
    $ftp set type_ FTP
    $ns at 0.0 "$ftp start"
}

proc add_web {client server rate} {
    global ns
    set CLIENT 0
    set SERVER 1

    # Setup PackMime
    set pm [new PackMimeHTTP]
    # $pm set-TCP Sack
    $pm set-client $client
    $pm set-server $server
    $pm set-rate $rate; # new connections per second
    $pm set-http-1.1;   # use HTTP/1.1

    # Create random variables
    set flow_arrive [new RandomVariable/PackMimeHTTPFlowArrive $rate]
    set req_size [new RandomVariable/PackMimeHTTPFileSize $rate $CLIENT]
    set rsp_size [new RandomVariable/PackMimeHTTPFileSize $rate $SERVER]

    # assign RNGs to RandomVariables
    $flow_arrive use-rng [new RNG]
    $req_size use-rng [new RNG]
    $rsp_size use-rng [new RNG]

    # set PackMime variables
    $pm set-flow_arrive $flow_arrive
    $pm set-req_size $req_size
    $pm set-rsp_size $rsp_size

    $ns at 0.0 "$pm start"
}

# Finish procedure
proc finish {} {
    global ns tracefile
    $ns halt
    $ns flush-trace
    puts "Done"
    exit 0
}

# Scripts
#ns trace-all [open link.tr w]
setup_scenario
$ns trace-queue $n2 $n1 [open link-isp.tr w]
$ns trace-queue $n1 $n0 [open link-local.tr w]
$ns at [expr $stopTime ] "finish"
$ns run
exit 0

