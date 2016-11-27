
gnuplot.exe << END
set grid
set xlabel 'N'
set ylabel 'average delete step count'
set terminal png size 1280,720
set logscale y 2 
set output 'graph2.png'
set format x '%g'
set term png
plot 'hard-test.csv' with lines , 'naive-hard-test.csv' with lines 
END
