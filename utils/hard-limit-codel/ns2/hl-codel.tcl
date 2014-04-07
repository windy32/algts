set ns [new Simulator]
set startTime 0.0
set stopTime 100
#set stopTime 300

global defaultRNG
$defaultRNG seed 12345
ns-random 12345

proc print_usage {} {
    puts "Usage:"
    puts "  ns hl-codel.tcl ftps rev_ftps web_rate bw delay bdelay packet_size mode \[dynamic\]"
    puts "CoDel Mode:"
    puts "  1. codel limit"
    puts "  2. hl-codel max_delay"
    puts "  3. fifo limit"
    puts "Examples:"
    puts "  - ns hl-codel.tcl 1 0 0 8Mb 5 50 1500 codel 1000"
    puts "     (One ftp session, no reverse ftp, no web traffic, downlink rate is 8 Mbps,"
    puts "      5ms local and Internet delay, 50ms bottleneck delay, 1.5 KB packets,"
    puts "      use original CoDel, buffer size is 1000 packets)"
    puts "  - ns hl-codel.tcl 1 0 0 8Mb 5 50 1500 hl-codel 50 dynamic"
    puts "     (Use hard limit CoDel, max delay = 50ms, dynamic bandwidth)"
}

# Parse command line
if {$argc >= 8} {
    set num_ftps [lindex $argv 0]
    set rev_ftps [lindex $argv 1]
    set web_rate [lindex $argv 2]
    set bw [bw_parse [lindex $argv 3]]
    set delay [expr [lindex $argv 4] * 0.001]
    set bdelay [expr [lindex $argv 5] * 0.001]
    set pkt_size [lindex $argv 6]
    set mode [lindex $argv 7]

    if {$argc == 9} {
        set dynamic 0
    } elseif {$argc == 10 && [lindex $argv 9] == "dynamic"} {
        set dynamic 1
    } else {
        print_usage
        exit 0
    }

    if {$mode == "codel"} {
        set limit [lindex $argv 8]
    } elseif {$mode == "hl-codel"} {
        set max_delay [lindex $argv 8]
    } elseif {$mode == "fifo"} {
        set limit [lindex $argv 8]
    } else {
        print_usage
        exit 0
    }
} else {
    print_usage
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

# The buffer size (in packets)
if {$mode == "codel" || $mode == "fifo"} {
    Queue set limit_ $limit
} else {
    Queue set limit_ 1000
    Queue/CoDel set maxdelay_ $max_delay
}

# Build topology
#                           <--------------------------------------------
#                           <------------  Web / FTP Traffic  -----------
#                           <--------------------------------------------    +---------o Server 1
#                                                                            |
#               o---------------o================================o-----------+---------o Server 2
#           Client(n0)      Gateway(n1)      bottleneck       ISP(n2)        |             ...
#                                                              CoDel          +---------o Server n
# Reverse Server 1 o----+     CoDel
#                       |   Gateway(n1)      bottleneck       ISP(n2)          Reverse client(r0)
# Reverse Server 2 o----+-------o================================o----------------o
#       ...             |                                         
# Reverse Server n o----+    -------------------------------------------->
#                            ------------ Reverse FTP Traffic ----------->
#                            -------------------------------------------->
proc setup_scenario {} {
    global ns stopTime num_ftps rev_ftps web_rate bw delay bdelay pkt_size mode dynamic
    global limit max_delay
    global n0 n1 n2
    
    set n0 [$ns node]
    set n1 [$ns node]
    set n2 [$ns node]
    set r0 [$ns node]
    
    puts "Setting up scenario..."
    puts [format "   - Traffic patterns"]
    puts [format "      + FTP flows = %d" $num_ftps]
    puts [format "      + Reverse FTP = %d" $rev_ftps]
    puts [format "      + Web rate = %d" $web_rate]
    puts [format "      + Packet size = %d" $pkt_size]
    puts [format "      + RTT = %d ms" [expr round(($delay * 2 + $bdelay) * 2 * 1000)]]
    puts [format "   - ISP settings"]
    puts [format "      + Bandwidth = %.1f Mbps" [expr $bw / 1000000.0]]
    if {$mode == "codel"} {
        puts [format "      + AQM mode = CoDel (%d packets)" $limit]
    } elseif {$mode == "hl-codel"} {
        puts [format "      + AQM mode = Hard limit CoDel (%d ms)" $max_delay]
    }
    puts "Running simulation..."

    $ns duplex-link $n0 $n1 [expr $bw * 10] [format "%.0fms" [expr $delay * 1000]] DropTail
    if {$mode == "fifo"} {
        $ns duplex-link $n1 $n2 [expr $bw]  [format "%.0fms" [expr $bdelay * 1000]] DropTail
    } else {
        $ns duplex-link $n1 $n2 [expr $bw]  [format "%.0fms" [expr $bdelay * 1000]] CoDel
    }
    $ns duplex-link $n2 $r0 [expr $bw * 10] [format "%.0fms" [expr $delay * 1000]] DropTail

    for {set i 1} {$i <= $num_ftps} {incr i 1} {
        set nx [$ns node]
        $ns duplex-link $n2 $nx [expr $bw * 10] [format "%.0fms" [expr $delay * 1000]] DropTail
        add_ftp $nx $n0 $i
    }
    
    for {set i 1} {$i <= $rev_ftps} {incr i 1} {
        set rx [$ns node]
        $ns duplex-link $n1 $rx [expr $bw * 10] [format "%.0fms" [expr $delay * 1000]] DropTail
        add_ftp $rx $r0 [expr 1000 + $i]
    }

    if {$web_rate > 0} {
        set nx [$ns node]
        $ns duplex-link $n2 $nx [expr $bw * 10] [format "%.0fms" [expr $delay * 1000]] DropTail
        add_web $n0 $nx $web_rate
    }
    
    # Dynamic bandwidth
    if {$dynamic == 1} {
        set bw_changes(1) 0.1
        set bw_changes(2) 0.01
        set bw_changes(3) 0.5
        set bw_changes(4) 0.01
        set bw_changes(5) 1.0

        for {set k 1} {$k <= 5} {incr k 1} {
            set changeTime [expr $k * $stopTime / 6]
            set new_bw [expr $bw_changes($k) * $bw]
            $ns at $changeTime "[[$ns link $n1 $n2] link] set bandwidth_ $new_bw"
            $ns at $changeTime "[[$ns link $n2 $n1] link] set bandwidth_ $new_bw"
        }
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
setup_scenario
$ns trace-queue $n2 $n1 [open link.tr w]
$ns at [expr $stopTime ] "finish"
$ns run
exit 0

