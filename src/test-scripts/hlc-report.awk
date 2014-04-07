BEGIN {
    # use pipe
    rtt = ARGV[1]
    bandwidth = ARGV[2]
    ARGC = 1

    # use file
    #rtt = ARGV[2]
    #bandwidth = ARGV[3]
    #ARGC = 2
}
{
    if ($1 == "fifo") {
        algorithm = 2
    }
    else if ($1 == "codel") {
        algorithm = 0
    }
    else if ($1 == "hlc") {
        algorithm = 1
    }

    if ($1 == rtt && $2 == bandwidth) {
        ftp = $3
        avgd[algorithm, ftp] = $4
        maxd[algorithm, ftp] = $5
        throughput[algorithm, ftp] = $6
    }
}
END {
    for (i = 1; i <= 64; i = i * 2) { # bandwidth
        for (j = 0; j <= 2; j++) { # algorithm
            if (j < 2)
                printf("%d\t", avgd[j, i]);
            else
                printf("%d\t\t\t", avgd[j, i]);
        }
        for (j = 0; j <= 2; j++) { # algorithm
            if (j < 2)
                printf("%d\t", maxd[j, i]);
            else
                printf("%d\t\t\t", maxd[j, i]);
        }
        for (j = 0; j <= 2; j++) { # algorithm
            if (j < 2)
                printf("%d\t", throughput[j, i]);
            else
                printf("%d\n", throughput[j, i]);
        }
    }
}
