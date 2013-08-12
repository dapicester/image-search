function histogram = computeHistogramFromImage(vocabulary, im, varargin)
% COMPUTEHISTOGRAMFROMIMAGE  Compute histograms for an image.
%
%   HISTOGRAM = COMPUTEHISTOGRAMFROMIMAGE(VOCABULARY, IM) compute the
%   histogram of visual words (PHOW) and the histogram of oriented 
%   gradients (PHOG) for image IM. The visual word vocabulary VOCABULARY
%   is used to compute visual words.
%   HISTOGRAM is a structure containing the fields 'phow' and 'phog'.
%
%   The function accepts the following options:
%
%   PHOG:: [true]
%     Compute PHOG.
%
%   PHOW:: [true]
%     Compute PHOW.
%
%   See also: COMPUTEVOCABULARYFROMIMAGELIST().

opts.phow = true;
opts.phog = true;
[opts, varargin] = vl_argparse(opts, varargin);

% Author: Andrea Vedaldi
% Author: Paolo D'Apice

if ischar(im)
    im = imread(im);
end

[height, width] = size(im);
descriptors = computeFeatures(im, varargin{:});
words = quantizeDescriptors(vocabulary, descriptors);
numWords = size(vocabulary.words, 2);
%histogram.hog = computeHistogram(width, height, descriptors, words, numWords);
histogram.hog = hist(words, numWords);

