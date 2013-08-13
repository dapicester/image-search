clc
clear all

setup()
environment()

global DATA_DIR

force_hist = false;
force_index = true;

%category = 'bag';
category = 'shoe'; 

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
%image = imread(fullfile(DATA_DIR, 'test', 'T11oxTBsDT1RCvBVdK.jpg'));
image = imread(fullfile(DATA_DIR, 'test', '6jw0_kqytgwkfm5bgu2dwgfjeg5sckzsew_1211x1032.jpg_468x468.jpg.jpg'));
%image = imread(fullfile(DATA_DIR, 'test', 'T1kXaoXdFBXXc2GD.8_072102.jpg_310x310.jpg'));
query = computeHistogramFromImage(vocabulary, image);
figure(4), imshow(image)

% query by shape
[indices,rank] = query_index(hog_index, hog, query.hog');
figure(1), display_matches(image, names, indices, rank);

% query by color
[indices,rank] = query_index(colors_index, colors, query.colors');
figure(2), display_matches(image, names, indices, rank);

% query by shape and color
[indices,rank] = query_index(combined_index, combined, [query.hog, query.colors]');
figure(3), display_matches(image, names, indices, rank);

