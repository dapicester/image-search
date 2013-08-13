function reduced = reduce_colors(image, colormap, dither)
% REDUCE_COLORS  Reduces the number of colors of the image.
%
%   IMAGE = REDUCE_COLORS(IMAGE, COLORMAP)  Returns the indexed image with 
%   colors specified in COLORMAP.
%
%   ... = REDUCE_COLORS(..., DITHER)  Enables or disables dithering.
%   DITHER can be either 'dither' or 'nodither' (default).

% Author: Paolo D'Apice

if nargin < 3, dither = 'nodither'; end

reduced = rgb2ind(image, colormap, dither);
