function [output, blocks] = tiling(regions, fhandle)
% TILING  Apply the function the given image regions.
%
%   [OUTPUT,BLOCKS] = TILING(REGIONS, FHANDLE)
%
% See also: SPATIALLAYOUT()

% Author: Paolo D'Apice

blocks = cellfun(fhandle, regions, 'UniformOutput', false);
output = cell2mat(blocks);
