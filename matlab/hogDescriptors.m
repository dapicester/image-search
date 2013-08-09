function [hog_matrix] = hogDescriptors(image, cellsize, bins)
% HOGDESCRIPTORS  Extract histogram of oriented gradients from the image.
%
%   HOG = HOGDESCRIPTORS(IMAGE) extracts HOG
%
%   ... = HOGDESCRIPTORS(..., cellsize, bins) allows to specify the
%   cellsize (defaults to 8) and the number of bins into wich divide the
%   [0, pi) range.

% Author: Paolo D'Apice

    if nargin < 2, cellsize = 8; end
    if nargin < 3, bins = 9; end

    % use grayscale
    img = im2single(rgb2gray(image));
    hog = vl_hog(img, cellsize, 'NumOrientations', bins);

    [m,n,d] = size(hog);
    hog_matrix = reshape(hog, m*n, d);
    
    display = true;
    if display
        figure(1)
        subplot(121), drawimage(image, 'Original image')
        subplot(122), drawhog(hog, cellsize, bins)
    end
end


function drawhog(hog, cellsize, bins)
    hog_image = vl_hog('render', hog, 'NumOrientations', bins);
    imagesc(hog_image)
    colormap(gray)
    title(sprintf('HOG cellsize=%d', cellsize))
end