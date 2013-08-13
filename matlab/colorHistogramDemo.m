% COLORHISTOGRAMDEMO Extract color histograms from sample images.

% Author: Paolo D'Apice

clc
clear all

image_dir = fullfile(fileparts(pwd), 'images');

images = { ... %'peppers.png', 'saturn.png', 'pears.png', ...
           ... %'coloredChips.png', 'gantrycrane.png', ...
           'bag1.jpg', 'shoe1.jpg', 'shoe2.jpg', ...
         };

map = colorcube(256);

for str = images
    name = char(str);
    image = load_image(fullfile(image_dir, name));
    colorHistogram(image, map, 'topk', 10, 'display', true);
    %print(sprintf('%s-histogram.eps', name), '-depsc2', '-f1')
    %print(sprintf('%s-histogram.png', name), '-dpng', '-f1')
    disp('Press a key to continue or CTRL-C to stop.')
    pause
end
close
