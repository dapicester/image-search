function out = superpixels(image, maxdist)
ratio = 0.5;
kernelsize = 2;
%maxdist = 10;
out = vl_quickseg(image, ratio, kernelsize, maxdist);