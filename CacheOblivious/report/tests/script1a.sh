
gnuplot.exe << END
set grid
set xlabel 'number of blocks to to store matrix row'
set ylabel 'average time for swap [micro seconds]'
set terminal png size 1280,720
#set logscale x 2 
set output 'graph1-0.png'
set format x '%g'
set term png
plot  '384bl.csvS' with lines 
END
