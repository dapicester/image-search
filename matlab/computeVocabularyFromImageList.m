function vocabulary = computeVocabularyFromImageList(category, names, numWords, varargin)
% COMPUTEVOCABULARYFROMIMAGELIST  Computes a visual word vocabulary.
%
%   VOCABULARY = COMPUTEVOCABULARYFROMIMAGELIST(CATEGORY, NAMES, NUMWORDS) 
%   computes a visual word vocabulary from the list of image names (paths)
%   NAMES belonging to the given image CATEGORY.
%
%   VOCABULARY is a structure with fields:
%     WORDS:: N x K matrix of K visual word centers of dimension N.
%     KDTREE:: KD-tree indexing the visual word for fast quantization.
%     CLASS:: the image category.

% Author: Andrea Vedaldi
% Author: Paolo D'Apice

% This extracts a number of visual descriptors from the specified images. 
% Only NUMFEATURES overall descriptors are retrieved as more do not really
% improve the estimation of the visual dictionary but slow down computation.

len = length(names);
descriptors = cell(1, len);
numFeatures = round(numWords * 100 / len);
for i = 1:len % parfor
    fullPath = names{i};
    fprintf('  Extracting features from %s (%d/%d)\n', fullPath, i, len);
    im = imread(fullPath);
    d = computeHog(im, varargin{:}); % XXX use fhandle?
    descriptors{i} = vl_colsubset(d, numFeatures, 'uniform');
end

% This clusters the descriptors into NUMWORDS visual words by using KMEANS.
% It then computes a KDTREE to index them. 
% The use of a KDTREE is optional, but speeds-up quantization significantly.

fprintf('Computing visual words and kdtree ...\n');
descriptors = single([descriptors{:}]);
vocabulary.words = vl_kmeans(descriptors, numWords, 'algorithm', 'elkan', 'verbose');
vocabulary.kdtree = vl_kdtreebuild(vocabulary.words, 'verbose');
vocabulary.class = category;
