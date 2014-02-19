BEGIN {
    prev_ftp = ""

    # The best buffer size (in packets) for different bandwidth
    QLEN[0.5] = 2
    QLEN[1] = 3
    QLEN[2] = 3
    QLEN[3] = 3
    QLEN[4] = 3
    QLEN[5] = 3
    QLEN[6] = 4
    QLEN[8] = 4
    QLEN[10] = 4
    QLEN[12] = 4
    QLEN[14] = 4
    QLEN[16] = 6
    QLEN[20] = 6
    QLEN[24] = 6
    QLEN[30] = 8
    QLEN[40] = 8
    QLEN[50] = 10
    QLEN[60] = 12
    QLEN[80] = 14
    QLEN[100] = 16
}
{
    if (NF == 10)
    {
        if (QLEN[$1] == $4) # Match queue length
        {
            if (prev_ftp != "" && $2 != prev_ftp) # New {bw, ftp}
            {
                if (num_70ms > 0)
                    print record_70ms
                else if (num_80ms > 0)
                    print record_80ms
                else if (num_90ms > 0)
                    print record_90ms
                else
                    print "-"
                
                num_90ms = 0
                num_80ms = 0
                num_70ms = 0
                record_90ms = ""
                record_80ms = ""
                record_70ms = ""
            }
            
            if ($7 <= 90)
            {
                record_90ms = $0
                num_90ms += 1
            }
            if ($7 <= 80)
            {
                record_80ms = $0
                num_80ms += 1
            }
            if ($7 <= 70)
            {
                record_70ms = $0
                num_70ms += 1
            }
                        
            prev_ftp = $2
        }
    }
}

END {
    if (num_70ms > 0)
        print record_70ms
    else if (num_80ms > 0)
        print record_80ms
    else if (num_90ms > 0)
        print record_90ms
    else
        print "-"
}
