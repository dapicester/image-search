% Extract color histograms.

clc
clear all

images = { 'peppers.png', 'saturn.png', 'pears.png', ...
           'coloredChips.png', 'gantrycrane.png', ...
           'bag1.jpg', ...
           'shoe1.jpg', ...
           'shoe2.jpg', ...
         };
for str = images
    image = imread(char(str));
    colorHistogram(image);
    %print(sprintf('%s-histogram.eps', char(str)), '-depsc2', '-f1')
    %print(sprintf('%s-histogram.png', char(str)), '-dpng', '-f1')
    pause
end
close
