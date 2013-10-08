function setup(what)
% SETUP  Check the environment.
%
%   SETUP(WHAT)  WHAT can one string or a cell string of the following values:
%   - 'vlfeat'  VLFeat library setup
%   - 'parfor'  initializes Matlab pool of workers
%   - 'all'     all of the above

% Author: Paolo D'Apice

root = fileparts(pwd);

if any(strcmp(what, { 'vlfeat', 'all' }))
    setup_vlfeat(root);
end
if any(strcmp(what, { 'parfor', 'all' }))
    setup_parfor;
end


function setup_vlfeat(root)
if exist('vl_version', 'file') ~= 3
    vlfeat_path = fullfile(root, 'dependencies', 'vlfeat-0.9.17');
    assert(exist(vlfeat_path, 'dir') == 7, 'VLFeat path not found: %s', vlfeat_path)

    script = fullfile(vlfeat_path, 'toolbox', 'vl_setup');
    run(script)
    assert(exist('vl_version', 'file') == 3, 'Failed to setup VLFeat')
end


function setup_parfor()
if exist('matlabpool', 'file') == 2 && matlabpool('size') == 0
    matlabpool('open')
end
