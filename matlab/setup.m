function setup(what)
% SETUP  Check the environment.
%
%   SETUP('vlfeat')

% Author: Paolo D'Apice

if nargin < 1, what = 'all'; end

root = fileparts(pwd);

switch what
    case { 'vlfeat', 'all' }
        if exist('vl_version', 'file') ~= 3
            setup_vlfeat
        end
end

function setup_vlfeat()
    vlfeat_path = fullfile(root, 'dependencies', 'vlfeat-0.9.16');
    assert(exist(vlfeat_path, 'dir') == 7, 'VLFeat path not found: %s', vlfeat_path)
    
    script = fullfile(vlfeat_path, 'toolbox', 'vl_setup');
    run(script)
    assert(exist('vl_version', 'file') == 3, 'Failed to setup VLFeat')
end

end
