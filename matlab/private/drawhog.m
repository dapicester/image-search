function drawhog(hog, cellsize, bins, edges)
    hog_image = vl_hog('render', hog, 'NumOrientations', bins);
    imagesc(hog_image)
    colormap bone
    axis image
    axis off
    title(sprintf('HOG cellsize=%d\norientations=%d edges=%s', ...
                  cellsize, bins, tif(edges, 'true', 'false')))
end