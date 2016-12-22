
gnuplot.exe << END
set grid
set xlabel 'matrix dimension'
set ylabel 'average time for swap [micro seconds]'
set terminal png size 1280,720
set logscale x 2 
set output 'graph1.png'
set format x '%g'
set term png
plot 'measure-trivial.csvS' with lines , 'measure-normal.csvS' with lines 
END
