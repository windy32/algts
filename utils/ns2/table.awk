{
    if (NF == 10)
    {
        if ($1 != prev_bw)
        {
            print ""
        }
        printf("%d\t", $7);
        prev_bw = $1
    }
}
END {
    print ""
}
