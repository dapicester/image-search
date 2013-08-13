function [indices, freqs] = count_colors(image)
% COUNT_COLORS  Gets unique color indices and their occurrence
% normalized frequency.
%
%   [COLORS, FREQS] = COUNT_COLORS(IMAGE, MAP)

% Author: Paolo D'Apice

words = sort(image(:));

[~, first] = unique(words, 'first');
[indices, last]  = unique(words, 'last');
freqs = last - first + 1;
freqs = freqs./sum(freqs); % normalize
