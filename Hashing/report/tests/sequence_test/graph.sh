function transform_single(){
	awk "{ print \$1 \" \" \$$3 }" $1 > $2
}


function transform(){
	j=2
	for i in min max average median decil
	do
		transform_single test-s-t-1.csv test-s-1-${i}.csv $j
		transform_single test-s-t-2.csv test-s-2-${i}.csv $j

		((j++))
	done
}


function create_graph(){
gnuplot.exe << END
set grid
set xlabel 'size in power of 2'
set ylabel '$1 of steps in insert'
set terminal png size 1024,720
#set logscale x 2 
#set logscale y 2 
set output 'sequential-$1-test.png'
set format x '%g'
set term png
set title "$2"
plot 'test-s-1-$1.csv' title "linear probing - tabulation" with lines , 'test-s-2-$1.csv' title "linear probing - multiply-shift" with lines 
END
}

transform

for i in min max average median decil
do
	create_graph $i "${i} steps"
done

