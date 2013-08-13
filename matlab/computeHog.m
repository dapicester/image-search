function hog = computeHog(im, varargin)
% COMPUTEHOG  Extract HOG from image.

% Author: Paolo D'Apice

sim = standardizeImage(im);
hog = hogDescriptors(sim, 'cellsize', 8, 'bins', 8);
