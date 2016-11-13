
../gnuplot/bin/gnuplot.exe << END
set grid
set xlabel 'set size'
set ylabel 'average find depth'
set output '$1.png'
set logscale y 1.2 
set terminal png size 1280,720
set format x '%g'
set term png
plot 't10$2.out' with lines , 't100$2.out' with lines , 't1000$2.out' with lines , 't10000$2.out' with lines , 't100000$2.out' with lines , 't1000000$2.out' with lines
END
#set logscale x 1.2 
