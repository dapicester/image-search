function histogram = bowHistogram(vocabulary, descriptors)
% BOWHISTOGRAM  Compute histogram for bag-of-words descriptors.

% Author: Paolo D'Apice

words = quantizeDescriptors(vocabulary, descriptors);
numWords = size(vocabulary.words, 2);
histogram = hist(words, numWords);
histogram = histogram/sum(histogram);