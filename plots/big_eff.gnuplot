set terminal png size 640,480 enhanced font "Helvetica,10"
set output 'plots/big_eff.png'
set xtics 1
set xlabel "Chunk size (MB)"
set ylabel "Compression level (in %, the lower the better)"
plot "plots/big_eff.data" using 1:2 title 'rfc3' with lines, \
     "plots/big_eff.data" using 1:3 title 'rfc4' with lines

