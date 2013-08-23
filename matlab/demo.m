% DEMO  Quick demo.

% Author: Paolo D'Apice

clc
clear all

setup()
environment()

global DATA_DIR

force_vocab = false;
force_hist = false || force_vocab;
force_index = false || force_hist;
num_query = 25;

% category = 'bag';
% category = 'shoe';
category = 'woman_shoe';

vocabulary = buildVocabulary(category, 'force', force_vocab);
[histograms, names] = buildHistograms(category, vocabulary, 'force', force_hist);

% hog
hog = cat(1, histograms.hog)';
hog_index = build_index(category, hog, 'hog', 'force', force_index);

% colors
colors = cat(1, histograms.colors)';
colors_index = build_index(category, colors, 'colors', 'force', force_index);

% hsv-colors
hsv = cat(1, histograms.hsvcolors)';
hsv_index = build_index(category, hsv, 'hsv', 'force', force_index);

% both
combined = [hog; colors];
combined_index = build_index(category, combined, 'combined', 'force', force_index);

% query image
images = dir(fullfile(DATA_DIR, 'test', [category '*.jpg']));
images = {images.name}';
image = imread(fullfile(DATA_DIR, 'test', images{1}));

query = computeHistogramFromImage(vocabulary, image);
figure(1), imshow(image), title('Query image')

% by shape
[shape_indices,shape_rank] = query_index(hog_index, hog, query.hog', num_query);
figure(2), display_matches(image, names, shape_indices, shape_rank);

% by color
[color_indices,color_rank] = query_index(colors_index, colors, query.colors', num_query);
figure(3), display_matches(image, names, color_indices, color_rank);

% by hsv-color
[hsv_indices,hsv_rank] = query_index(hsv_index, hsv, query.hsvcolors', num_query);
figure(4), display_matches(image, names, hsv_indices, hsv_rank);

% query by shape and color
[combined_indices,combined_rank] = query_index(combined_index, combined, [query.hog query.colors]', num_query);
figure(5), display_matches(image, names, combined_indices, combined_rank);

% cleanup
clear force_vocab force_hist force_index num_query 
