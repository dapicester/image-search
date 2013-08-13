function environment()
% ENVIRONMENT  Load global variables.
%
%   DATA_DIR
%       Directory containing images subdirectories and saved data
%       (vocabularies, histograms, etc.)

% Author: Paolo D'Apice

global DATA_DIR
DATA_DIR = fullfile(fileparts(pwd), 'data');
