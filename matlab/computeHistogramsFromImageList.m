function histograms = computeHistogramsFromImageList(vocabulary, names, varargin)
% COMPUTEHISTOGRAMSFROMIMAGELIST  Computes histograms for multiple images.
%
%   HISTOGRAMS = COMPUTEHISTOGRAMSFROMIMAGELIST(VOCABULARY, NAMES)
%   computes the histograms of visual words for the list of image
%   paths NAMES by applying iteratively COMPUTEHISTOGRAMFROMIMAGE().
%
%   The function accepts the following options:
%
%   Descriptors:: ['both']
%     Descriptors to be computes. Only PHOG if set to 'phog', only PHOW
%     if set to 'phow', both otherwise.
%
%   Cache:: CACHE_DIR
%     Caches the histograms to the CACHE_DIR directory.

% Author: Andrea Vedaldi
% Author: Paolo D'Apice

conf.cache = [];
[conf, varargin] = vl_argparse(conf, varargin);

useCache = ~isempty(conf.cache);
if useCache, fprintf('Using cache %s\n', conf.cache); end

len = numel(names);
histograms = cell(1, len);
parfor i = 1:len % parfor
    fullPath = names{i};
    if useCache
        % try to retrieve from cache
        histograms{i} = getFromCache(fullPath, conf.cache);
        if ~isempty(histograms{i})
            continue
        end
    end
    fprintf('  Extracting histograms from %s (%d/%d)\n', fullPath, i, len);
    histograms{i} = computeHistogramFromImage(vocabulary, fullPath, varargin{:});
    if useCache
        % save to cache
        storeToCache(fullPath, conf.cache, histograms{i});
    end
end
histograms = [histograms{:}];


function histogram = getFromCache(fullPath, cache)
[~, name] = fileparts(fullPath);
cachePath = fullfile(cache, [name '.mat']);
if exist(cachePath, 'file')
    data = load(cachePath);
    histogram = data.histogram;
else
    histogram = [];
end


function storeToCache(fullPath, cache, histogram)
[~, name] = fileparts(fullPath);
cachePath = fullfile(cache, [name '.mat']);
vl_xmkdir(cache);
data.histogram = histogram; %#ok<STRNU>
save(cachePath, '-struct', 'data');

