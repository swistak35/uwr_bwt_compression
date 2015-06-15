set terminal png size 640,480 enhanced font "Helvetica,10"
set output 'plots/rfc_eff.png'
set xtics 1
set xlabel "Chunk size (100KB)"
set ylabel "Compression level (in %, the lower the better)"
plot "plots/rfc_eff.data" using 1:2 title 'bzip' with lines, \
     "plots/rfc_eff.data" using 1:3 title 'mine' with lines


