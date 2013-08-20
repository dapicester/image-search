function hogDemo(varargin)
% HOGDEMO Extract HOG from sample images.
%
% Options:
%
%   Cellsize:: [8]
%
%   Bins:: [8]
%
%   Level:: [1]
%     Pyramidal level, set to 0 to disable.
%
%   Save:: ['no']
%     Set the save format (available 'eps', 'png')

% Author: Paolo D'Apice

clc
setup('all');

opts.cellsize = 8;
opts.bins = 8;
opts.level = 1;
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
    [~,hog] = hogDescriptors(image, 'cellsize', opts.cellsize, ...
                                    'bins', opts.bins, 'edges', false, ...
                                    'level', opts.level);
    [~,ehog] = hogDescriptors(image, 'cellsize', opts.cellsize, ...
                                     'bins', opts.bins, 'edges', true, ...
                                     'level', opts.level);
 
    display_results
        
    disp('Press a key to continue or CTRL-C to stop.')
    pause
end
close


function display_results
% DISPLAY_RESULTS  Nested function to display results.
    figure(1)
    set(1, 'Units', 'Normalized', 'OuterPosition', [0 0 1 1])
    
    rows = opts.level + 2;
    subplot(rows,2,1), drawimage(image, 'Original image')
    subplot(rows,2,2), drawimage(edge(rgb2gray(image), 'canny'), 'Edges');
    
    % level 0
    subplot(rows,2,3), drawhog(hog{1},  opts.cellsize, opts.bins, false);
    subplot(rows,2,4), drawhog(ehog{1}, opts.cellsize, opts.bins, true);
    
    if opts.level > 0
        % show only level 1
        subplot(6,4,17), drawhog(hog{2},  opts.cellsize, opts.bins, false);
        subplot(6,4,18), drawhog(hog{3},  opts.cellsize, opts.bins, false);
        subplot(6,4,21), drawhog(hog{4},  opts.cellsize, opts.bins, false);
        subplot(6,4,22), drawhog(hog{5},  opts.cellsize, opts.bins, false);
        
        subplot(6,4,19), drawhog(ehog{2},  opts.cellsize, opts.bins, true);
        subplot(6,4,20), drawhog(ehog{3},  opts.cellsize, opts.bins, true);
        subplot(6,4,23), drawhog(ehog{4},  opts.cellsize, opts.bins, true);
        subplot(6,4,24), drawhog(ehog{5},  opts.cellsize, opts.bins, true);
    end
    
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
