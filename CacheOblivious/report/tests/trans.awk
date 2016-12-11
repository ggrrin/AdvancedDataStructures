#!/bin/awk

/^Elem/ {
printf("%s ", sqrt($2));

}

/^Misses:/ {
 print $2
}
