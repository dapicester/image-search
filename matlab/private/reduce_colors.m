function [image, map] = reduce_colors(image, num_colors, dither)
% REDUCE_COLORS  Reduces the number of colors of the image.
%
%   IMAGE = REDUCE_COLORS(IMAGE, NUM_COLORS)  Returns the RGB image reduced
%   to NUM_COLORS.
%
%   [X, MAP] = REDUCE_COLORS(...)  returns the indexed image.
%
%   [...] = REDUCE_COLORS(__, DITHER)  enables or disables dithering.
%   DITHER can be either 'dither' or 'nodither' (default).

% Author: Paolo D'Apice

isint = @(x) round(x) == x;
if ~isint(num_colors), error('num_color must be integer: %g', num_colors); end

if nargin < 3, dither = 'nodither'; end

% uniform RGB colorspace
map = colorcube(num_colors);

indexed = rgb2ind(image, map, dither);

if nargout == 1
    image = ind2rgb(indexed, map);
else
    image = indexed;
end
