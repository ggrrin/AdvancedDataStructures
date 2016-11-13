
../gnuplot/bin/gnuplot.exe << END
set grid
set xlabel 'set size'
set ylabel 'average find depth'
set terminal png size 1280,720
set logscale y 1.2 
set output '$1.png'
set format x '%g'
set term png
plot 's.out' with lines , 'sN.out' with lines 
END
