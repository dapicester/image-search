function [vocabulary, histograms] = test_vocabulary()

% init
setup('vlfeat');
environment();
global DATA_DIR

% build vocabulary for 'test' class
image_dir = fullfile(DATA_DIR, 'test');
names = files(image_dir);

vocabulary = computeVocabularyFromImageList('test', names, 300);
save('test_vocabulary.mat', 'vocabulary')

% compute words for images in 'images' dir
image_dir = fullfile(fileparts(DATA_DIR), 'images');
names = files(image_dir);
histograms = computeHistogramsFromImageList(vocabulary, names);
save('test_vocabulary_hist.mat', 'histograms')


function names = files(dirname)
files = dir(dirname);
names = {files.name};
names = names(4:end)';
names = cellfun(@(x) fullfile(dirname, x), names, 'UniformOutput', false);
