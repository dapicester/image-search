% HOGDEMO Extract HOG from sample images.

% Author: Paolo D'Apice

clc
clear all

images = { ... %'peppers.png', 'saturn.png', 'pears.png', ...
           ... %'coloredChips.png', 'gantrycrane.png', ...
           'bag1.jpg', 'shoe1.jpg', 'shoe2.jpg', ...
         };
for str = images
    name = char(str);
    image = load_image(name);
    hogDescriptors(image, 'cellsize', 8, 'bins', 8);
    %print(sprintf('%s-hog.eps', name), '-depsc2', '-f1')
    %print(sprintf('%s-hog.png', name), '-dpng', '-f1')
    pause
end
close
