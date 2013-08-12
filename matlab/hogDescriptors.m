function [hog_matrix] = hogDescriptors(image, varargin)
% HOGDESCRIPTORS  Extract histogram of oriented gradients from the image.
%
%   HOG = HOGDESCRIPTORS(IMAGE) extracts HOG
%
%   The function accepts the following options:
%
%   Cellsize:: [8]
%
%   Bins:: [9]
%
%   Edges:: [false]
%
%   Display:: [true]
%
%   The returned a matrix contains HISTOGRAM_SIZE x NUM_HISTOGRAMS values, 
%   where HISTOGRAM_SIZE = 3 * BINS + 1.
%
%   See also VL_HOG

% Author: Paolo D'Apice

    opts.cellsize = 8;
    opts.bins = 9;
    opts.edges = false;
    opts.display = false;
    [opts, ~] = vl_argparse(opts, varargin);
    
    % use grayscale
    sz = size(image);
    if numel(sz) == 3 && sz(3) == 3
        img = rgb2gray(image);
    else
        img = image;
    end
    
    % edges
    if opts.edges
        img = edge(img, 'Canny');
    end
    
    % descriptors
    hog = vl_hog(im2single(img), opts.cellsize, 'NumOrientations', opts.bins);

    [m,n,d] = size(hog);
    hog_matrix = reshape(hog, m*n, d)';
    
    if opts.display
        figure(1)
        subplot(121), drawimage(image, 'Original image')
        subplot(122), drawhog(hog, opts.cellsize, opts.bins)
    end
end


function drawhog(hog, cellsize, bins)
    hog_image = vl_hog('render', hog, 'NumOrientations', bins);
    imagesc(hog_image)
    colormap(gray)
    axis image
    axis off
    title(sprintf('HOG cellsize=%d orientations=%d', cellsize, bins))
end