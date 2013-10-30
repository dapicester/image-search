set title 'Query Index'
set logscale x 5
set xrange [0.9:52]
set xlabel '#neighbors'
set ylabel 'time (ms)'
plot '-' with linespoints title '', '-' with yerrorbars title ''
