set terminal png size 640,480 enhanced font "Helvetica,10"
set output 'plots/ebook_eff.png'
set xtics 100
set xlabel "Chunk size (KB)"
set ylabel "Average compression level (in %, the lower the better)"
plot "plots/ebook_eff.data" using 1:2 title "epub" with lines, \
     "plots/ebook_eff.data" using 1:3 title "mobi" with lines, \
