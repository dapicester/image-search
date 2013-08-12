% COLORHISTOGRAMDEMO Extract color histograms from sample images.

% Author: Paolo D'Apice

clc
clear all

image_dir = fullfile(fileparts(pwd), 'images');

images = { ... %'peppers.png', 'saturn.png', 'pears.png', ...
           ... %'coloredChips.png', 'gantrycrane.png', ...
           'bag1.jpg', 'shoe1.jpg', 'shoe2.jpg', ...
         };
for str = images
    name = char(str);
    image = load_image(fullfile(image_dir, name));
    colorHistogram(image);
    %print(sprintf('%s-histogram.eps', name), '-depsc2', '-f1')
    %print(sprintf('%s-histogram.png', name), '-dpng', '-f1')
    pause
end
close
