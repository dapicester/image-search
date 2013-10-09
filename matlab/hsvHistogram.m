function [histogram, varargout] = hsvHistogram(image, varargin)
% HSVHISTOGRAM  Compute the HSV color histogram on the quantized version
% of the input image.
%
%   [HISTOGRAM,QUANTIZED] = HSVHISTOGRAM(IMAGE)  Get the HSV color
%   histogram on the quantized image.
%
%   The function accepts the following options:
%
%   Levels:: [18 3 3]
%     Bins into wich partition the HSV colorspace.
%     The colorspace is non-uniformly quantized into
%     Levels(1) x Levels(2) x Levels(3) colors and Levels(3) + 1 greys.
%     Using default values one has 162 + 4 = 166 colors.
%
%   MedFilt:: [true]
%     Perform median filtering on the quantized image.

% Author: Paolo D'Apice

opts.levels = [18, 3, 3];
opts.medfilt = true;
opts = vl_argparse(opts, varargin);

if size(image, 3) == 1 % grayscale
    image = repmat(image, [1 1 3]);
end

image = rgb2hsv(image);
quant = quantize(image, opts.levels + [0 1 1]);

if opts.medfilt
    for i = 1:3
        quant(:,:,i) = medfilt2(quant(:,:,i), 'symmetric');
    end
end

[n,m,c] = size(quant);
data = reshape(quant, n*m, c);

histogram = compute_histogram(opts.levels, data);

% HSV quantized image
if nargout > 1
    quantized = zeros(size(quant));
    for i = 1:3
        delta = opts.levels(i) + tif(i > 1, 1, 0);
        quantized(:,:,i) = quant(:,:,i)./delta;
    end
    varargout{1} = quantized;
end


function q = quantize(image, levels)
% Quantize the image into given levels for each channel.
q = zeros(size(image));
for i = 1:3
    channel = image(:,:,i);
    step = 1/levels(i);
    thresh = 0:step:1-step;
    q(:,:,i) = imquantize(channel, thresh(2:end));
end


function histogram = compute_histogram(levels, data)
% Compute the histogram using non-uniform mapping.
colors = zeros(levels(1), levels(2), levels(3)); % HxSxV colors
grays = zeros(levels(3) + 1, 1);                 % V+1 grays

% for each [H S V] tuple fill the corresponding histogram bin
for i = 1:length(data)
    t = data(i,:);
    if t(3) == 1 % black
        grays(1) = grays(1) + 1;
        continue
    end
    if t(2) == 1 && t(3) > 1 % grays
        grays(t(3)) = grays(t(3)) + 1;
        continue
    end
    % colors
    t = t - [0 1 1];
    colors(t(1),t(2),t(3)) = colors(t(1),t(2),t(3)) + 1;
end

% concatenate and normalize to sum 1
histogram = [colors(:); grays]';
histogram = histogram./sum(histogram);
    
