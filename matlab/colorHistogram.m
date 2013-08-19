function [histogram, colors, freqs, quant] = colorHistogram(image, colormap, varargin)
% COLORHISTOGRAM  Computes the color histogram.
%
%   [HISTOGRAM, COLORS, FREQS, QUANT] = COLORHISTOGRAM(IMAGE, COLORMAP)  Computes
%   color histogram on the COLORMAP and return the color indexes, normalized 
%   frequency and the quantized indexed image.

% Author: Paolo D'Apice
    
quant = rgb2ind(image, colormap, 'nodither');
[colors, freqs] = count_colors(quant);
histogram = make_histogram(colors, freqs, length(colormap));


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
