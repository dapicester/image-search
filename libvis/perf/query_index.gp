set title 'Query Index'
set logscale xy
set xrange [500:2e6]
set xlabel '#records'
set ylabel 'time (ms)'
plot '-' with linespoints title '', '-' with yerrorbars title ''
