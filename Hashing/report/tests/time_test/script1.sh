
gnuplot.exe << END
set grid
set xlabel 'load factor'
set ylabel 'average time of insert [micro seconds]'
set terminal png size 1280,720
#set logscale y 10 
set output 'uniform-test.png'
set format x '%g'
set term png
plot 'test-u-t-1.csv' with lines , 'test-u-t-2.csv' with lines , 'test-u-t-3.csv' with lines , 'test-u-t-4.csv' with lines , 'test-u-t-5.csv' with lines 
 
END
