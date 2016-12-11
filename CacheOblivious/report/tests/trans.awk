#!/bin/awk

/^Access/ {
printf("%s ", $2);

}

/^Misses:/ {
 print $2
}
