
gnuplot.exe << END
set grid
set xlabel 'load factor'
set ylabel 'average time to insert [micro seconds]'
set terminal png size 1024,720
set logscale y 10 
set output 'tabulation-hash-test.png'
set format x '%g'
set term png
set title "Tabulation versions"
plot './4/test-ut-t-1.csv' title "linear probing - tabulation - 4 parts" with lines , './4/test-ut-t-4.csv' title "cucko - tabulation - 4 parts" with lines, './8/test-ut-t-1.csv' title "linear probing - tabulation - 8 parts" with lines , './8/test-ut-t-4.csv' title "cucko - tabulation - 8 parts" with lines, './16/test-ut-t-1.csv' title "linear probing - tabulation - 16 parts" with lines , './16/test-ut-t-4.csv' title "cucko - tabulation - 16 parts" with lines, './32/test-ut-t-1.csv' title "linear probing - tabulation - 32 parts" with lines , './32/test-ut-t-4.csv' title "cucko - tabulation - 32 parts" with lines 
 
END
