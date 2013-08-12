clc
clear all

global DATA_DIR
DATA_DIR = fullfile(fileparts(pwd), 'data');

category = 'bag';

vocabulary = buildVocabulary(category);
histograms = buildHistograms(category, vocabulary);

[index, data] = build_index(category, histograms);

%image = imread(fullfile(DATA_DIR, 'test', 'T11oxTBsDT1RCvBVdK.jpg'));
%image = imread(fullfile(DATA_DIR, 'test', '6jw0_kqytgwkfm5bgu2dwgfjeg5sckzsew_1211x1032.jpg_468x468.jpg.jpg'));
image = imread(fullfile(DATA_DIR, 'test', 'T1kXaoXdFBXXc2GD.8_072102.jpg_310x310.jpg'));
query = computeHistogramFromImage(vocabulary, image);
query = query.hog';

[indices,rank] = query_index(index, data, query);

matches = histograms.names(indices(indices~=0));
figure(1)
subplot(121), subimage(image), axis off, title('query');
for j = 1:length(matches)
    img = imread(char(matches{j}));
    subplot(122), subimage(img), axis off, title(sprintf('rank: %d',rank(j)));
    pause
end
close

