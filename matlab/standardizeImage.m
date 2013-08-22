function im = standardizeImage(im, varargin)
% STANDARDIZEIMAGE  Rescale an image to a standard size
%
%   IM = STANDARDIZEIMAGE(IM) rescale IM to have a maximum height.
%
%   The function accepts the following options:
%
%   Height:: [480]
%     Maximum height.

% Author: Andrea Vedaldi
% Author: Paolo D'Apice

opts.height = 480;
opts = vl_argparse(opts, varargin);

% TODO: ensure it is a JPEG in RGB colorspace

% single precision
if ~isa(im, 'single')
    im = im2single(im);
end

% max size
if size(im,1) > opts.height
    im = imresize(im, [opts.height NaN], 'bilinear');
    % when using the default 'bicubic' one must ensure that values are in
    % the correct range [0,1] or [0,255].
end
