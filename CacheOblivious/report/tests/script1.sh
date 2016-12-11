
gnuplot.exe << END
set grid
set xlabel 'size of matrix dimensions'
set ylabel 'average time'
set terminal png size 1280,720
#set logscale y 10101010101010101010  
set output 'graph1.png'
set format x '%g'
set term png
plot 'measure-trivial.csv' with lines , 'measure-normal.csv' with lines 
END
