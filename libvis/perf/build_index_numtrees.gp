set title 'Build Index'
set logscale xy 2
set xrange [0.9:9]
set xlabel '#trees'
set ylabel 'time (ms)'
plot '-' with linespoints title '', '-' with yerrorbars title ''
