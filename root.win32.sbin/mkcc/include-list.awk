BEGIN {
    FS = "[ \t]";
    RS = "[\r\n]";
    s = "#include <...> search starts here:";
    e = "End of search list.";
    i = 0;
    sum = "";
} {
    if ($0 != "") {
        if ($0 == e) {
            i = 0;
        }
        if (i == 1) {
            sum = sum" "$0;
        }
        if ($0 == s) {
            i = 1;
        }
    }
} END {
    print(sum);
}
