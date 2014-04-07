# Usage: awk -f print-cell.awk input filter-column filter-value target-column
# Example: awk -f print-cell.awk codel-1m.log 2 128 6
BEGIN {
    ARGC = 2
}
{
    if ($ARGV[2] == ARGV[3]) {
        printf("%d", $ARGV[4]);
    }
}

