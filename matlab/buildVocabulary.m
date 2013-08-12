function vocabulary = buildVocabulary(class, varargin)
% BUILDVOCABULARY  Build visual word vocabulary.
%
%   VOCABULARY = BUILDVOCABULARY('NAME')  Compute vocabulary of visual 
%   words for images in class NAME.
%
%   VOCABULARY = BUILDVOCABULARY(NAMES)  Compute vocabulary of visual words
%   for classes in cell NAMES.
%
%   The function accepts the following options:
%
%   DataDir:: [global DATA_DIR]
%     The directory containing image files.
%
%   SaveDir:: [global DATA_DIR]
%     The directory where to save the dictionary.
%
%   Force:: [true]
%     Build the vocabulary even if it already exists.
%
%   NumImages:: [100]
%     Build the vocabulary using only a subset of all the available images
%     for each class;
%
%   NumWords:: [300]
%     The number of visual words.


% Author: Paolo D'Apice

global DATA_DIR

conf.dataDir = DATA_DIR;
conf.saveDir = DATA_DIR;
conf.force = false;
conf.numImages = 100;
conf.numWords = 300;
[conf, varargin] = vl_argparse(conf, varargin);

if ischar(class)
    % one class
    vocabularyFile = fullfile(conf.saveDir, ['vocabulary_' class '.mat']);
    vocabulary = loadOrBuildVocabulary(vocabularyFile, ...
                                       conf.numImages);
else
    % multi-class
    vocabularyFile = fullfile(conf.saveDir, 'vocabulary.mat');
    vocabulary = loadOrBuildVocabulary(vocabularyFile, ...
                                       conf.numImages * numel(class));
end

function vocabulary = loadOrBuildVocabulary(vocabularyFile, numImages)
% LOADORBUILDVOCABULARY  Actually load or compute the visual words vocabulary.
    if conf.force || ~exist(vocabularyFile, 'file')
        % load image file names
        names = readFileNames(class, conf.dataDir);

        % Use only a (not small) subset of training images
        vocabulary = computeVocabularyFromImageList(class, ...
                            vl_colsubset(names', numImages, 'uniform'), ...
                            conf.numWords, varargin{:});
        % save to file
        save(vocabularyFile, '-struct', 'vocabulary');
        fprintf('Vocabulary saved to %s.\n', vocabularyFile);
    else
        % load from file
        vocabulary = load(vocabularyFile);
        fprintf('Vocabulary loaded from %s\n', vocabularyFile);
    end
end % loadOrBuildClassVocabulary

end % buildVocabulary
