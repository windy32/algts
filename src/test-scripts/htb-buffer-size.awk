BEGIN {
    packets[0] = -1
    packets[1] = 2
    packets[2] = 4
    packets[3] = 8
    packets[4] = 16
    packets[5] = 32
    packets[6] = 64
    packets[7] = 128
    packets[8] = 256
    packets[9] = 512
    packets[10] = 1024
    i = 0
}
{
    if ($1 == "Bandwidth" && $5 == "threads" && $8 == "ceil") {
        bandwidth = $3
        threads = substr($7, 1, length($7) - 1)
        ceil = substr($10, 1, length($10) - 1)
        
        if (threads == 2) {
            if (i == 0) {
                print "default"
            }
            else
            {
                
                printf("pfifo limit %d\n", packets[i])
            }
            i += 1
        }
        printf("    Threads = %d", threads);
    }
    
    if ($1 == 1 && $2 == "|") {
        min = 0
        max = 0
        avg = 0
    }
    
    min += $3
    max += $5
    avg += $7
    
    if ($1 == 50 && $2 == "|") {
        printf(" min = %d, max = %d, avg = %d\n", min / 50, max / 50, avg / 50)
    }
}
