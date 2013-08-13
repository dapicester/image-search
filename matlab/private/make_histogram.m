function histogram = make_histogram(indices, values, bins)
% MAKE_HISTOGRAM  Makes an histogram.
%
%  HISTOGRAM = MAKE_HISTOGRAM(INDICES, VALUES, BINS) Makes an histogram
%  with BINS values in which values at INDICES are valued with VALUES.

% Author: Paolo D'Apice

histogram = zeros(1, bins);
histogram(indices + 1) = values;
