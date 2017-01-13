
gnuplot.exe << END
set grid
set xlabel 'load factor'
set ylabel 'average time to insert [micro seconds]'
set terminal png size 1024,720
set logscale y 10 
set output 'uniform-test.png'
set format x '%g'
set term png
set title "Average insert time"
plot 'test-ut-t-1.csv' title "linear probing - tabulation" with lines , 'test-ut-t-2.csv' title "linear probing - multiply-shift" with lines , 'test-ut-t-3.csv' title "linear probing - trivial modulo" with lines , 'test-ut-t-4.csv' title "cucko - tabulation" with lines , 'test-ut-t-5.csv' title "cucko - multiply-shift" with lines 
 
END
