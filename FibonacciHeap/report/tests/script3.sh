
../gnuplot/bin/gnuplot.exe << END
set grid
set xlabel 'set size'
set ylabel 'average find depth'
set output '$1.png'
set logscale y 1.2 
set terminal png size 1280,720
set format x '%g'
set term png
plot 't100.out' with lines , 't100N.out' with lines , 't10000.out' with lines , 't10000N.out' with lines , 't1000000.out' with lines , 't1000000N.out' with lines
END
