function drawimage(image, varargin)
% DRAWIMAGE  Display an image in a subplot.
%
%   DRAWIMAGE(IMAGE, TITLE)
%   DRAWIMAGE(IMAGE, COLORMAP, TITLE)

% Author: Paolo D'Apice

switch nargin
    case 2
        subimage(image)
        title(varargin{1})
    case 3
        subimage(image, varargin{1})
        title(varargin{2})
end
axis off
