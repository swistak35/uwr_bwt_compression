set terminal png size 640,480 enhanced font "Helvetica,10"
set output 'plots/rfc_comp.png'
set xtics 1
set xlabel "Chunk size (100KB)"
set ylabel "Compression time (ms)"
plot "plots/rfc_comp.data" using 1:2 title 'bzip' with lines, \
     "plots/rfc_comp.data" using 1:3 title 'suffix' with lines, \
     "plots/rfc_comp.data" using 1:4 title 'uni' with lines


