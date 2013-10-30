set title 'Query Index'
set logscale x
set xrange [500:2e6] reverse
set xlabel '#comparisons'
set ylabel 'time (ms)'
plot '-' with linespoints title '', '-' with yerrorbars title ''

