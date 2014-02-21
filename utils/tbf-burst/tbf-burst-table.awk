{
    if (NF == 4)
    {
        if ($1 != prev_bw)
        {
            print ""
        }
        printf("%d\t", $4);
        prev_bw = $1
    }
}
END {
    print ""
}
