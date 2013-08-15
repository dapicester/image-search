function hogDemo()
% HOGDEMO Extract HOG from sample images.

% Author: Paolo D'Apice

clc
clear all
setup;

image_dir = fullfile(fileparts(pwd), 'images');

images = { ... 'peppers.png', 'saturn.png', 'pears.png', ...
           ... 'coloredChips.png', 'gantrycrane.png', ...
           ... 'bag1.jpg', 'shoe1.jpg', 'shoe2.jpg', ...
           'bag1.jpg', 'shoe1.jpg', 'peppers.png' ...
         };
for str = images
    name = char(str);
    image = load_image(fullfile(image_dir, name));
    cellsize = 8;
    bins = 8;
    [~,hog] = hogDescriptors(image, 'cellsize', cellsize, 'bins', bins, 'edges', false);
    [~,ehog] = hogDescriptors(image, 'cellsize', cellsize, 'bins', bins, 'edges', true);
 
    figure(1)
    set(1, 'units', 'normalized', 'outerposition', [0 0 1 1])
    subplot(131), drawimage(image, 'Original image')
    subplot(132), drawhog(hog, cellsize, bins, false);
    subplot(133), drawhog(ehog, cellsize, bins, true);
    %print(sprintf('%s-hog.eps', name), '-depsc2', '-f1')
    %print(sprintf('%s-hog.png', name), '-dpng', '-f1')
    disp('Press a key to continue or CTRL-C to stop.')
    pause
end
close
