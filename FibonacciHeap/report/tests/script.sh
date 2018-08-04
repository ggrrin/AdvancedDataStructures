
../gnuplot/bin/gnuplot.exe << END
set grid
set xlabel 'set size'
set ylabel 'average find depth'
set output '$1.png'
set format x '%g'
set terminal png size 1280,720
set format x '%g'
set term png
plot '$1.out' with lines
END
