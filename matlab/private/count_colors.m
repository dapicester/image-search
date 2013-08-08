function [colors, freqs] = count_colors(image, map)
% COUNT_COLORS  Gets unique colors and their occurrence frequency.
%
%   [COLORS, FREQS] = COUNT_COLORS(IMAGE, MAP)

% Author: Paolo D'Apice

words = sort(image(:));

[~, first] = unique(words, 'first');
[col, last]  = unique(words, 'last');

colors = map(col + 1, :); % index 1-based
freqs = last - first + 1;
