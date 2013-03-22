{
    if ($1 == "->") {
        users = $2
        print "users = ", users
    }

    if ($1 == 1) {
        min = 0
        max = 0
        avg = 0
    }
    min += $3
    max += $5
    avg += $7
    
    if ($1 == 64) {
        printf(" min = %d, max = %d, avg = %d\n", min / 64, max / 64, avg / 64)
    }
}
