
prog=$1

for i in 1 2 
do
	$prog s $i > test-s-t-$i.csv
done
