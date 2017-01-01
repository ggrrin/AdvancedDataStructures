
gnuplot.exe << END
set grid
set xlabel 'size'
set ylabel 'average steps in insert'
set terminal png size 1280,720
set logscale x 2 
set logscale y 2 
set output 'uniform-test2.png'
set format x '%g'
set term png
plot  'test-s-1-max.csv' with lines , 'test-s-2-max.csv' with lines 
END
