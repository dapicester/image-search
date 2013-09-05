function colorSegmentation(image)
figure(2)
subplot(231), imshow(image,[]);

%% Lab colorspace
cform = makecform('srgb2lab');
lab_image = applycform(image,cform);

%% Classify the Colors in 'a*b*' Space Using K-Means Clustering
ab = double(lab_image(:,:,2:3));
nrows = size(ab,1);
ncols = size(ab,2);
ab = reshape(ab,nrows*ncols,2);

nColors = 3;
% repeat the clustering 3 times to avoid local minima
[cluster_idx, cluster_center] = kmeans(ab,nColors, ...
                                       'distance','sqEuclidean', ...
                                       'emptyaction','drop', ...
                                       'start', 'uniform', ...
                                       'Replicates',3);

%% label pixels
pixel_labels = reshape(cluster_idx,nrows,ncols);
subplot(232), imshow(pixel_labels,[]), title('image labeled by cluster index');

%% segment
segmented_images = cell(1,nColors);
rgb_label = repmat(pixel_labels,[1 1 3]);

for k = 1:nColors
    color = image;
    color(rgb_label ~= k) = 0;
    segmented_images{k} = color;
end

for k = 1:nColors
    subplot(2,3,2+k), imshow(segmented_images{k}), title(['objects in cluster ' num2str(k)]);
end

%% segment with brightness
