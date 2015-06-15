set terminal png size 640,480 enhanced font "Helvetica,10"
set output 'plots/lexicomp_comp.png'
set xtics 1
set xlabel "Chunk size (* 256B)"
set ylabel "Compression time (ms)"
plot "plots/lexicomp_comp.data" using 1:2 title 'lexi' with lines, \
     "plots/lexicomp_comp.data" using 1:4 title 'uni' with lines, \
     "plots/lexicomp_comp.data" using 1:6 title 'suffix' with lines



