
prog=$1

for i in 1 2 3 4 5
do
	$prog u $i > test-u-t-$i.csv
done
