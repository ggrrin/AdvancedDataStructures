
gnuplot.exe << END
set grid
set xlabel 'N'
set ylabel 'Misses'
set terminal png size 1280,720
#set logscale y 10
set output 'graph2.png'
set format x '%g'
set term png
plot 'output-n-64_64.log.csv' with lines , 'output-n-64_1024.log.csv' with lines , 'output-n-64_4096.log.csv' with lines , 'output-n-512_512.log.csv' with lines , 'output-n-4096_64.log.csv' with lines 
END
