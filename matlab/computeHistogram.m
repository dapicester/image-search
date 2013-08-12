function histogram = computeHistogram(width, height, keypoints, words, numWords, varargin)
% COMPUTEHISTOGRAM  Compute a spatial histogram of visual words.
%
%   HISTOGRAM = COMPUTEHISTOGRAM(WIDTH, HEIGHT, KEYPOINTS, WORDS, NUMWORDS)
%   computes a RxC spatial histogram of the N visual words WORDS.
%   KEYPOINTS is a 2 x N matrix of x,y coordinates of the
%   visual words and WIDTH and HEIGHT are the image dimensions; these
%   quantities are needed for the geometric tiling.
%   By default the spatial histogram is computed on a 2x2 tile
%   grid using NUMWORDS words.  
%   Use COMPUTEHISTOGRAM(..., 'tiles', CELL) to override default value 
%   of {[2 2]}.
%
%   Example: 
%     histogram = COMPUTEHISTOGRAM(..., 'tiles', { [1 1] [2 2] [3 1] });

% Author: Andrea Vedaldi
% Author: Paolo D'Apice

opts.tiles = {[1 1]};
[opts, ~] = vl_argparse(opts, varargin);

htiles = cell(1, length(opts.tiles));
for i = 1:length(opts.tiles)
    dim = opts.tiles{i};
    binsx = vl_binsearch(linspace(1, width, dim(1) + 1), keypoints(1,:));
    binsy = vl_binsearch(linspace(1, height, dim(2) + 1), keypoints(2,:));
    bins = sub2ind([dim(2) dim(1) numWords], ...
                   binsy, binsx, words);
    htile = zeros(dim(2) * dim(1) * numWords, 1);
    htile = vl_binsum(htile, ones(size(bins)), bins);
    htiles{i} = single(htile / (sum(htile) + eps));
end

histogram = cat(1, htiles{:});
histogram = single(histogram / sum(histogram))';
