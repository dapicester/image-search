function [histogram, colors, freqs, quant] = colorHistogram(image, colormap, varargin)
% COLORHISTOGRAM  Computes the color histogram.
%
%   [HISTOGRAM, COLORS, FREQS, QUANT] = COLORHISTOGRAM(IMAGE, COLORMAP)  Computes
%   color histogram on the COLORMAP and return the color indexes, normalized 
%   frequency and the quantized indexed image.
%
%   The function accepts the following options:
%
%   Level:: [0]
%     Pyramid decomposition level, 0 for none.

% Author: Paolo D'Apice

opts.level = 0;
opts = vl_argparse(opts, varargin);

quant = rgb2ind(image, colormap, 'nodither');

[width, height] = size(quant);
fhandle = @(x) do_color_histogram(x, length(colormap));

histogram = [];
colors = [];
freqs = [];

for i = 0:opts.level
    % number of blocks
    l = 2^i;
    % block size
    w = floor(width/l) .* ones(1, l);
    h = floor(height/l) .* ones(1, l);
    
    blocks = mat2cell(quant(1:sum(w), 1:sum(h)), w, h);
    [h,c,f] = cellfun(fhandle, blocks, 'UniformOutput', false);
    
    histogram = cat(1, histogram, h(:));
    colors = cat(1, colors, c(:));
    freqs = cat(1, freqs, f(:));
end


function [histogram, colors, freqs] = do_color_histogram(quant, num_colors)
% DO_COLOR_HISTOGRAM  Actually compute color histogram.

[colors, freqs] = count_colors(quant);
histogram = make_histogram(colors, freqs, num_colors);


function [indices, freqs] = count_colors(image)
% COUNT_COLORS  Gets unique color indices and their occurrence
% normalized frequency.
%
%   [COLORS, FREQS] = COUNT_COLORS(IMAGE, MAP)

words = sort(image(:));

[~, first] = unique(words, 'first');
[indices, last]  = unique(words, 'last');
freqs = last - first + 1;
freqs = freqs./sum(freqs); % normalize


function histogram = make_histogram(indices, values, bins)
% MAKE_HISTOGRAM  Makes an histogram.
%
%  HISTOGRAM = MAKE_HISTOGRAM(INDICES, VALUES, BINS) Makes an histogram
%  with BINS values in which values at INDICES are valued with VALUES.

histogram = zeros(1, bins);
histogram(indices + 1) = values;
