function BWfinal=segmentation(Im)
figure(1)
I = rgb2gray(Im);

%% edge detection
method = 'sobel';
[~, threshold] = edge(I,method);
fudgeFactor = .5;
BWs = edge(I,method, threshold * fudgeFactor);
subplot(231), imshow(BWs), title('binary gradient mask');

%% dilation
se90 = strel('line', 3, 90);
se0 = strel('line', 3, 0);
BWsdil = imdilate(BWs, [se90 se0]);
subplot(232), imshow(BWsdil), title('dilated gradient mask');

%% filling
BWdfill = imfill(BWsdil, 'holes');
subplot(233), imshow(BWdfill), title('binary image with filled holes');

%% clear border (XXX no)
% BWnobord = imclearborder(BWdfill, 4);
BWnobord = BWdfill;
subplot(234), imshow(BWnobord), title('cleared border image');

%% smoothen
seD = strel('diamond',1);
BWfinal = imerode(BWnobord,seD);
BWfinal = imerode(BWfinal,seD);
subplot(235), imshow(BWfinal), title('segmented image');

%% superimpose
BWoutline = bwperim(BWfinal);
Segout = Im;
Segout(BWoutline) = 255;
subplot(236), imshow(Segout), title('outlined original image');

%% final
figure(2)
subplot(121), imshow(Im), title('input');
subplot(122), imshow(imoverlay(Im, ~BWfinal, [.8 .8 .8])), title('segmented');

