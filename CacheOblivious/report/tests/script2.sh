
gnuplot.exe << END
set grid
set xlabel 'matrix dimension'
set ylabel 'average misses for swap'
set terminal png size 1280,720
set logscale x 2 
set output 'graph2.png'
set format x '%g'
set term png
plot 'output-n-64_64.log.csvS' with lines , 'output-n-64_1024.log.csvS' with lines , 'output-n-64_4096.log.csvS' with lines , 'output-n-512_512.log.csvS' with lines , 'output-n-4096_64.log.csvS' with lines 
END
