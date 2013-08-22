function histogram = computeHistogramFromImage(vocabulary, im, varargin)
% COMPUTEHISTOGRAMFROMIMAGE  Compute histograms for an image.

% Author: Paolo D'Apice

if ischar(im)
    im = imread(im);
end

im = standardizeImage(im, 'height', 128);

% hog
descriptors = hogDescriptors(im);
words = quantizeDescriptors(vocabulary, descriptors);
numWords = size(vocabulary.words, 2);
histogram.hog = hist(words, numWords);

% colors
histogram.colors = colorHistogram(im, colorcube(256));

% hsv colors
histogram.hsvcolors = hsvHistogram(im);