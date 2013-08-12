function descriptors = computeFeatures(im, varargin)

% Author: Paolo D'Apice

conf.cellsize = 8;
conf.bins = 8;
[conf, ~] = vl_argparse(conf, varargin);

im = standardizeImage(im);
descriptors = hogDescriptors(im, 'cellsize', conf.cellsize, ...
                                 'bins', conf.bins ...
                             );
