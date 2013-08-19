function [hog_matrix, hog] = hogDescriptors(image, varargin)
% HOGDESCRIPTORS  Extract histogram of oriented gradients from the image.
%
%   MATRIX = HOGDESCRIPTORS(IMAGE) extracts HOG and return a matrix containing
%   HOG_SIZE x NUM_HISTOGRAMS values, where HOGR_SIZE = 3 * BINS + 1 and 
%   NUM_HISTOGRAMS is the number of blocks on which HOGs are computed.
%   The returned a matrix 
%
%   [MATRIX, HOG] = HOGDESCRIPTORS(IMAGE) returns also the HOG descriptor as
%   returned by VL_HOG, i.e. a matrix of size ROWS x COLS x HOG_SIZE where
%   NUM_HISOGRAMS = ROW x COLS.
%
%   The function accepts the following options:
%
%   Cellsize:: [8]
%
%   Bins:: [9]
%
%   Edges:: [false]
%
%   See also VL_HOG

% Author: Paolo D'Apice

opts.cellsize = 8;
opts.bins = 9;
opts.edges = false;
opts.display = false;
opts = vl_argparse(opts, varargin);

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
