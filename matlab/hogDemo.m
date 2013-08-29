function hogDemo(varargin)
% HOGDEMO Extract HOG from sample images.

% Author: Paolo D'Apice

clc
setup('all');

opts.cellsize = 8;
opts.bins = 8;
opts.save = 'no';
opts = vl_argparse(opts, varargin);

image_dir = fullfile(fileparts(pwd), 'images');

images = { ... 'peppers.png', 'saturn.png', 'pears.png', ...
           ... 'coloredChips.png', 'gantrycrane.png', ...
           ... 'bag1.jpg', 'shoe1.jpg', 'shoe2.jpg', ...
           'bag1.jpg', 'shoe1.jpg', 'peppers.png' ...
         };
for str = images
    name = char(str);
    image = load_image(fullfile(image_dir, name));
    
    image = standardizeImage(image, 'height', 128);
    [~,hog] = hogDescriptors(image, 'cellsize', opts.cellsize, 'bins', opts.bins, 'edges', false);
    [~,ehog] = hogDescriptors(image, 'cellsize', opts.cellsize, 'bins', opts.bins, 'edges', true);
 
    display_results
        
    disp('Press a key to continue or CTRL-C to stop.')
    pause
end
close


function display_results
% DISPLAY_RESULTS  Nested function to display results.
    figure(1)
    set(1, 'units', 'normalized', 'outerposition', [0 0 1 1])
    subplot(131), drawimage(image, 'Original image')
    subplot(132), drawhog(hog,  opts.cellsize, opts.bins, false);
    subplot(133), drawhog(ehog, opts.cellsize, opts.bins, true);
    
    switch opts.save
        case 'eps'
            print(sprintf('%s-hog.eps', name), '-depsc2', '-f1')
        case 'png'
            print(sprintf('%s-hog.png', name), '-dpng', '-f1')
    end
end
end % hogDemo


function drawhog(hog, cellsize, bins, edges)
% DRAWHOG  Render HOGs in for display.
    hog_image = vl_hog('render', hog, 'NumOrientations', bins);
    imagesc(hog_image)
    colormap bone
    axis image
    axis off
    title(sprintf('HOG cellsize=%d\norientations=%d edges=%s', ...
                  cellsize, bins, tif(edges, 'true', 'false')))
end