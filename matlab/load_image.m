function image = load_image(filename)
% LOAD_IMAGE  Load RGB image from file.

% Author: Paolo D'Apice

[image, map] = imread(filename);
if ~isempty(map)
    image = ind2rgb(image, map);
end
