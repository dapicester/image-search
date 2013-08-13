function vocabulary = buildVocabulary(category, varargin)
% BUILD_VOCABULARY  Builds a visual word vocabulary.
%
%   VOCABULARY = BUILD_VOCABULARY(CATEGORY)  Computes vocabulary of visual 
%   words for images in the given category. Image files are stored in
%   DATA_DIR/CATEGORY directory.
%
%   The function accepts the following options:
%
%   DataDir:: [global DATA_DIR]
%     The directory containing image files.
%
%   SaveDir:: [global DATA_DIR]
%     The directory where to save the dictionary.
%
%   Force:: [false]
%     Overwrite existing vocabulary.
%
%   NumImages:: [100]
%     Build the vocabulary using only a subset of all the available images
%     for each class. If set to 0 it will use all the available images.
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

vocabularyFile = fullfile(conf.saveDir, sprintf('vocabulary_%s.mat', category));

if conf.force || ~exist(vocabularyFile, 'file')
    % load image file names
    names = readFileNames(category, conf.dataDir);

    if conf.numImages > 0
        % Use only a (not small) subset of training images
        names = vl_colsubset(names', conf.numImages, 'uniform');
    end
                        
    vocabulary = computeVocabularyFromImageList(category, names, ...
                        conf.numWords, varargin{:});
                    
    % save to file
    save(vocabularyFile, '-struct', 'vocabulary');
    fprintf('Vocabulary saved to %s.\n', vocabularyFile);
else
    % load from file
    vocabulary = load(vocabularyFile);
    fprintf('Vocabulary loaded from %s\n', vocabularyFile);
end

vocabulary %#ok<NOPRT>
