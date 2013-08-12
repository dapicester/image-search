function [index, data] = build_index(category, input)

data = cat(1, input.histograms.hog)';
index = vl_kdtreebuild(data, 'numtrees', 4, 'verbose');
index.category = category;