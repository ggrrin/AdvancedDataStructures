
gnuplot.exe << END
set grid
set xlabel 'N'
set ylabel 'Misses'
set terminal png size 1280,720
set logscale x 2
set output 'graph3.png'
set format x '%g'
set term png
plot 'output-t-64_64.log.csvS' with lines , 'output-t-64_1024.log.csvS' with lines , 'output-t-64_4096.log.csvS' with lines , 'output-t-512_512.log.csvS' with lines , 'output-t-4096_64.log.csvS' with lines 
END
