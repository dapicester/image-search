function names = readFileNames(class, dataDir)
% READFILENAMES  Read image class file names from file.
%
%   NAMES = READFILENAMES(CLASS, DIR)  Read file 'CLASS.txt' in DIR
%   containing file names corresponding to the image class CLASS.
%   CLASS can be either a string or a cell of strings.

% Author: Paolo D'Apice

if ischar(class)
    % one class
    names = readFile(class, dataDir);
    return
end

% multi-class
names = cell(1, numel(class));
for i = 1:numel(class)
    names{i} = readFile(class{i}, dataDir);
end
names = cat(1, names{:});


function names = readFile(class, dataDir)
% READFILE  Read file names from text file.
filename = fullfile(dataDir, [class '.txt']);
try
    names = textread(filename, '%s');
    names = cellfun(@(name) fullfile(dataDir, class, name), names, ...
                    'UniformOutput', false);
catch err
    error('Cannot read file %s', filename);
end
