function demo(varargin)
% DEMO  Quick demo.
%
%   DEMO()  Start search demo.
%
%   The function accepts the following options:
%
%   Type:: ['hsv']
%     Query type, valid values are: 'rgb', 'hsv', 'shape', 'rgb_shape',
%     'hsv_shape', 'weighted'.
%
%   Category:: ['bag']
%     Query category, valid values are: 'bag', 'shoe', 'woman_shoe'.
%
%   Query:: [1]
%     Index of the image to used as a query. Ranges from 1 to 5.
%
%   NumResults:: [25]
%     Number of images to be retrieved from the query.

% Author: Paolo D'Apice

setup('all');
environment();

opts.type = 'hsv';
opts.category = 'bag';
opts.query = 1;
opts.numResults = 25;

opts = vl_argparse(opts, varargin);

global DATA_DIR

force_vocab = false;
force_hist = false || force_vocab;
force_index = false || force_hist;


vocabulary = buildVocabulary(opts.category, 'force', force_vocab);
[histograms, names] = buildHistograms(opts.category, vocabulary, 'force', force_hist);

% query image
images = dir(fullfile(DATA_DIR, 'test', [opts.category '*.jpg']));
images = {images.name}';
image = imread(fullfile(DATA_DIR, 'test', images{opts.query}));

query = computeHistogramFromImage(vocabulary, image);
query_data = get_histograms(opts.type, query);
figure(1), imshow(image), title('Query image')

% do query
data = get_histograms(opts.type, histograms);
index = build_index(opts.category, data, opts.type, 'force', force_index);
[indices, rank] = query_index(index, data, query_data, opts.numResults);
figure(2), display_matches(image, names, indices, rank);
set(2, 'Name', sprintf('Query %s by %s', opts.category, opts.type), ...
       'NumberTitle', 'off', 'MenuBar', 'none');

% TODO weighted (weights must sum to 1)
% weight_color = 0.5;
% weight_shape = 1.0 - weight_color;
% 
% [w1_indices, w1_rank] = query_index(hsv_index, hsv, query.hsvcolors', 4*num_query);
% [w2_indices, w2_rank] = query_index(hog_index, hog, query.hog', 4*num_query);
% [weighted_indices, weighted_rank] = composed_distance({w1_indices; w2_indices}, ...
%     {w1_rank; w2_rank}, [weight_color, weight_shape], num_query);
% figure(6), display_matches(image, names, weighted_indices, weighted_rank);
