setup('vlfeat');
environment();

global DATA_DIR
image_dir = fullfile(DATA_DIR, 'test');
files = dir(image_dir);
names = {files.name};
names = names(4:end)';
names = cellfun(@(x) fullfile(image_dir, x), names, 'UniformOutput', false);

vocabulary = computeVocabularyFromImageList('test', names, 300);
save('test_vocabulary.mat', 'vocabulary')

