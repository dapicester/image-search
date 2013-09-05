function histogram = computeHistogramFromImage(vocabulary, im, varargin)
% COMPUTEHISTOGRAMFROMIMAGE  Compute histograms for an image.

% Author: Paolo D'Apice

if ischar(im)
    im = imread(im);
end

im = standardizeImage(im);

% hog
descriptors = hogDescriptors(im);
histogram.hog = bowHistogram(vocabulary, descriptors);

% colors
histogram.colors = colorHistogram(im, colorcube(256));

% hsv colors
histogram.hsvcolors = hsvHistogram(im);
