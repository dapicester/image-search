function names = data_files(name)
% DATA_FILES Get image filenames data dir.

% Author: Paolo D'Apice

image_dir = fullfile(fileparts(pwd), 'data', name);
if exist(image_dir, 'dir') ~= 7
    error('Cannot read dir: %s', image_dir)
end

files = dir(image_dir);
names = { files.name }';
