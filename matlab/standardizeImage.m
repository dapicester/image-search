function im = standardizeImage(im)
% STANDARDIZEIMAGE  Rescale an image to a standard size
%   IM = STANDARDIZEIMAGE(IM) rescale IM to have a height of at most
%   480 pixels.

% Author: Andrea Vedaldi
% Author: Paolo D'Apice

HEIGHT = 480;

% TODO: ensure it is a JPEG in RGB colorspace

% single precision
if ~isa(im, 'single')
    im = im2single(im);
end

% max size
if size(im,1) > HEIGHT
    im = imresize(im, [HEIGHT NaN]);
end
