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
combined = [hog; hsv]/2;
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
[combined_indices,combined_rank] = query_index(combined_index, combined, [query.hog query.hsvcolors]'/2, num_query);
figure(5), display_matches(image, names, combined_indices, combined_rank);

% weighted (weights must sum to 1)
weight_color = 0.5;
weight_shape = 1.0 - weight_color;

[w1_indices, w1_rank] = query_index(hsv_index, hsv, query.hsvcolors', 4*num_query);
[w2_indices, w2_rank] = query_index(hog_index, hog, query.hog', 4*num_query);
[weighted_indices, weighted_rank] = composed_distance({w1_indices; w2_indices}, ...
    {w1_rank; w2_rank}, [weight_color, weight_shape], num_query);
figure(6), display_matches(image, names, weighted_indices, weighted_rank);
