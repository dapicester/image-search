% HOGDEMO Extract HOG from sample images.

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
    hogDescriptors(image, 'cellsize', 8, 'bins', 8);
    %print(sprintf('%s-hog.eps', name), '-depsc2', '-f1')
    %print(sprintf('%s-hog.png', name), '-dpng', '-f1')
    disp('Press a key to continue or CTRL-C to stop.')
    pause
end
close
