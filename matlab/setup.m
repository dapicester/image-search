function setup(what)
% SETUP  Check the environment.
%
%   SETUP('vlfeat')

% Author: Paolo D'Apice

if nargin < 1, what = 'all'; end

root = fileparts(pwd);

if any(strcmp(what, { 'vlfeat', 'all' }))
    setup_vlfeat
end
if any(strcmp(what, { 'parfor', 'all' }))
    setup_parfor
end

function setup_vlfeat()
    if exist('vl_version', 'file') ~= 3
        vlfeat_path = fullfile(root, 'dependencies', 'vlfeat-0.9.16');
        assert(exist(vlfeat_path, 'dir') == 7, 'VLFeat path not found: %s', vlfeat_path)

        script = fullfile(vlfeat_path, 'toolbox', 'vl_setup');
        run(script)
        assert(exist('vl_version', 'file') == 3, 'Failed to setup VLFeat')
    end
end

function setup_parfor()
    if exist('matlabpool', 'file') == 2 && matlabpool('size') == 0
        matlabpool('open')
    end
end

end
