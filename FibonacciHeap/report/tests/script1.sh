
gnuplot.exe << END
set grid
set xlabel 'N'
set ylabel 'average delete step count'
set terminal png size 1280,720
set logscale y 10 
set output 'graph1.png'
set format x '%g'
set term png
plot 'uniform-test.csv' with lines , 'unbalanced-test.csv' with lines 
END
