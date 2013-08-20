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
%   Bins:: [8]
%
%   Edges:: [false]
%
%   Level:: [0]
%     Pyramid decomposition level, 0 for none.
%
%   See also VL_HOG.

% Author: Paolo D'Apice

opts.cellsize = 8;
opts.bins = 8;
opts.edges = false;
opts.level = 0;
opts = vl_argparse(opts, varargin);

% use grayscale
[width, height, cols] = size(image);
if cols == 3
    image = rgb2gray(image);
end

% edges
if opts.edges
    image = edge(image, 'Canny');
end

fhandle = @(x) vl_hog(im2single(x), opts.cellsize, 'NumOrientations', opts.bins);

hog = [];
hog_matrix = [];

for i = 0:opts.level
    % number of blocks
    l = 2^i;
    % block size
    w = floor(width/l) .* ones(1, l);
    h = floor(height/l) .* ones(1, l);
    
    blocks = mat2cell(image(1:sum(w), 1:sum(h)), w, h);
    h = cellfun(fhandle, blocks, 'UniformOutput', false); 
    h = h(:);
    
    [m,n,d] = size(h{1});
    hm = cellfun(@(x) reshape(x, m*n, d)', h, 'UniformOutput', false); 
    
    hog = cat(1, hog, h);
    hog_matrix = cat(1, hog_matrix, hm);
 end
