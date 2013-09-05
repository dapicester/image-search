function regions = spatialLayout(input)
% SPATIALLAYOUT  Divide an images into subblocks and apply a function.
%
%   REGIONS = SPATIALLAYOUT(INPUT)  Divides the input image in 4x4 blocks
%   and groups them into regions corresponding to the center and the
%   corners.

% Author: Paolo D'Apice

num_blocks = 4;
[width, height] = size(input);
w = floor(width/num_blocks) .* ones(1, num_blocks);
h = floor(height/num_blocks) .* ones(1, num_blocks);
blocks = mat2cell(input(1:sum(w), 1:sum(h)), w, h); % crop to size

regions = cell(1, 5);
regions{1} = cell2mat(blocks([6,7,10,11])); % center
regions{2} = cell2mat(blocks([1,2,5]));     % top-left
regions{3} = cell2mat(blocks([9,13,14]));   % top-right
regions{4} = cell2mat(blocks([12,15,16]));  % bottom-right
regions{5} = cell2mat(blocks([3,4,8]));     % bottom-left
