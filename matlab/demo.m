clc
clear all

setup()
environment()

global DATA_DIR

force_hist = false;
force_index = false;
num_query = 25;

category = 'bag';
% category = 'shoe'; 

vocabulary = buildVocabulary(category, 'force', force_hist);
[histograms, names] = buildHistograms(category, vocabulary, 'force', force_hist);

% hog
hog = cat(1, histograms.hog)';
hog_index = build_index(category, hog, 'hog', 'force', force_index);

% colors
colors = cat(1, histograms.colors)';
colors_index = build_index(category, colors, 'colors', 'force', force_index);

% both
combined = [hog; colors];
combined_index = build_index(category, combined, 'combined', 'force', force_index);

% query image
images = dir(fullfile(DATA_DIR, 'test', [category '*.jpg']));
images = {images.name}';
image = imread(fullfile(DATA_DIR, 'test', images{1}));

query = computeHistogramFromImage(vocabulary, image);
figure(4), imshow(image)

% query by shape
[indices,rank] = query_index(hog_index, hog, query.hog', num_query);
figure(1), display_matches(image, names, indices, rank);

% query by color
[indices,rank] = query_index(colors_index, colors, query.colors', num_query);
figure(2), display_matches(image, names, indices, rank);

% query by shape and color
[indices,rank] = query_index(combined_index, combined, [query.hog, query.colors]', num_query);
figure(3), display_matches(image, names, indices, rank);

