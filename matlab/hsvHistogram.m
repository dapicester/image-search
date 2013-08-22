function [histogram, varargout] = hsvHistogram(image, varargin)
% HSVHISTOGRAM  Compute the HSV color histogram.
%
%   [HISTOGRAM,QUANTIZED] = HSVHISTOGRAM(IMAGE)  Get the HSV color
%   histogram on the quantized image.
%
%   The function accepts the following options:
%
%   Levels:: [18 3 3]
%     Bins into wich partition the HSV colorspace, eg. [8 2 2].

% Author: Paolo D'Apice

opts.levels = [18, 3, 3];
opts = vl_argparse(opts, varargin);

image = rgb2hsv(image);
quant = quantize(image, opts.levels);

[n,m,c] = size(quant);
data = reshape(quant, n*m, c);

% for each [H S V] tuple fill the corresponding histogram bin
histogram = zeros(opts.levels);
for i = 1:n*m
    t = data(i,:);
    %if ~all(t), continue; end % no 0 elements
    histogram(t(1),t(2),t(3)) = histogram(t(1),t(2),t(3)) + 1;
end

% normalize to sum 1
histogram = histogram(:)';
histogram = histogram./sum(histogram);

% quantized image
if nargout > 1
    quantized = zeros(size(quant));
    for i = 1:3
        quantized(:,:,i) = quant(:,:,i)./opts.levels(i);
    end
    varargout{1} = hsv2rgb(quantized);
end


function q = quantize(image, levels)
q = zeros(size(image));
for i = 1:3
    channel = image(:,:,i);
    step = 1/levels(i);
    thresh = 0:step:1-step;
    q(:,:,i) = imquantize(channel, thresh(2:end));
end
    