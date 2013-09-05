function index = build_index(category, data, name, varargin)
% BUILD_INDEX  Build kd-tree index.
%
%   INDEX = BUILD_INDEX(CATEGORY, DATA, NAME) Builds or loads the index on
%   DATA for images in CATEGORY
%

% Author: Paolo D'Apice

global DATA_DIR

opts.saveDir = DATA_DIR;
opts.force = false;
opts.numtrees = 1;
opts.distance = 'l2';
[opts, ~] = vl_argparse(opts, varargin);

filename = fullfile(opts.saveDir, sprintf('%s_%s_index.mat', category, name));

if opts.force || ~exist(filename, 'file')
    index = vl_kdtreebuild(data, 'verbose', ...
                           'numtrees', opts.numtrees, ...
                           'distance', opts.distance );
    index.category = category;
    index.name = name;
    save(filename, '-struct', 'index');
    fprintf('Index for class %s saved to %s.\n', category, filename);
else
    % no need to compute
    index = load(filename);
    fprintf('Index for class %s loaded from %s.\n', category, filename);
end
