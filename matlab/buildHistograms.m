function [histograms, names] = buildHistograms(classes, vocabulary, varargin)
% BUILDHISTOGRAMS  Computes image histograms.
%
%   [HISTOGRAMS, NAMES] = BUILDHISTOGRAMS(CLASSES, VOCABULARY) Computes
%   histograms for images in given CLASSES using the given VOCABULARY.
%   It returns also the file names.
%
%   The function accepts the following options:
%
%   DataDir:: [global DATA_DIR]
%     The directory containing image files.
%
%   SaveDir:: [global DATA_DIR]
%     The directory where to save histograms.
%
%   Force:: [false]
%     Overwrite existing histograms.
%
%   The function forwards any other unknown option.
%
% See also: PRIVATE/COMPUTEHISTOGRAMSFROMIMAGELIST()

% Author: Paolo D'Apice

global DATA_DIR

conf.dataDir = DATA_DIR;
conf.saveDir = DATA_DIR;
conf.force = false;
[conf, varargin] = vl_argparse(conf, varargin);

% only one class given
if ischar(classes), classes = {classes}; end

for c = classes
    class = char(c);
    filename = fullfile(conf.saveDir, [class '_hist.mat']);
    if conf.force || ~exist(filename, 'file')
        fprintf('Processing images in class %s ...\n', class);
        names = readFileNames(class, conf.dataDir);
        histograms = computeHistogramsFromImageList(vocabulary, names, varargin{:});
        save(filename, 'histograms', 'names');
        fprintf('Histograms for class %s saved to %s.\n', class, filename);
    else
        % no need to compute
        fprintf('Histograms for class %s already computed in %s.\n', class, filename);
        temp = load(filename);
        histograms = temp.histograms;
        names = temp.names;
    end
end