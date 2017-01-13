
prog=$1

for i in 1 2 3 4 5
do
	$prog ut $i > test-ut-t-$i.csv
done
