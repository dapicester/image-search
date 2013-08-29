function hsvHistogramDemo(varargin)
% HSVHISTOGRAMDEMO Extract HSV color histograms from sample images.
%
% Options:
%
%   Levels:: [[18 3 3]]
%     Quantization levels for each channel.
%
%   TopK:: [10]
%     The top-K colors, set to 0 to disable.
%
%   Save:: ['no']
%     Set the save format (available 'eps', 'png')

% Author: Paolo D'Apice

clc
setup('all');

opts.levels = [18 3 3];
opts.topk = 10;
opts.save = 'no';
opts = vl_argparse(opts, varargin);

image_dir = fullfile(fileparts(pwd), 'images');

images = { ... 'peppers.png', 'saturn.png', 'pears.png', ...
           ... 'coloredChips.png', 'gantrycrane.png', ...
           ... 'bag1.jpg', 'shoe1.jpg', 'shoe2.jpg', ...
           'bag1.jpg', 'shoe1.jpg', 'peppers.png' ...
         };

for str = images
    name = char(str);
    image = load_image(fullfile(image_dir, name));
    
    image = standardizeImage(image, 'height', 128);
    [histogram,quant] = hsvHistogram(image);
    [idx, topk, fk] = topk_colors(histogram, opts.topk);
    
    display_results
    
    disp('Press a key to continue or CTRL-C to stop.')
    pause
end
close


function display_results
% DISPLAY_RESULTS  Nested function to display results.
    figure(1)
    set(gcf, 'Units', 'Normalized', ...
             'Position', [0 0 1 1], ...
             'PaperPositionMode', 'auto')
    subplot(221), drawimage(image, 'Original')
    subplot(222), drawimage(quant, 'Quantized')
    
    if opts.topk > 0
        subplot(223), coloredbar(histogram, opts.levels)
        subplot(224), drawtopk(topk, fk, idx, opts.levels);
    else
        subplot(2,2,[3 4]), coloredbar(histogram, opts.levels)
    end
    
    switch opts.save
        case 'eps'
            print(sprintf('%s-hsvhist.eps', name), '-depsc2', '-f1')
        case 'png'
            print(sprintf('%s-hsvhist.png', name), '-dpng', '-f1')
    end
end
end % colorHistogramDemo


function color = rgb_color(levels,index)
% RGB_COLOR  Get RGB from HSv histogram index.
    [h,s,v] = ind2sub(levels, index);
    color = hsv2rgb([h/levels(1),s/levels(2),v/levels(3)]);
end


function coloredbar(histogram, levels)
% COLOREDBAR  Show histogram of colors using colored bars.
    histogram = histogram./max(histogram); % normalize to 1
    num_val = length(histogram);
    
    cla
    axis([0 num_val 0 1])
    for i = 1:num_val
        color = rgb_color(levels, i);
        patch([i-1 i-1 i i], [0 histogram(i) histogram(i) 0], ...
              color, 'edgecolor', color);
    end
    set(gca, 'xticklabel', '', 'yticklabel','')
    set(gca, 'visible', 'off')
    title('Color Histogram')
    set(findall(gca, 'type', 'text'), 'visible', 'on')
end


function [idx,topk,fk] = topk_colors(histogram, k)
% TOPK_COLORS  Retrieves the top-k colors according to their occurence.
    [~, idx] = sort(histogram, 'descend');
    idx = idx(1:k);
    topk = histogram(idx);
    fk = histogram(idx)./sum(histogram);
end


function drawtopk(colors, fk, indices, levels)
% DRAWTOPK  Shows the top-k colors and their frequency.
    cla
    k = length(colors);
    for i = 1:k
        color = rgb_color(levels, indices(i));
        patch([i-1 i-1 i i], [0 1 1 0], color)
        text(i-1 + 0.2, 0.5, sprintf('%.2f%%', fk(i)*100))
    end
    set(gca, 'xticklabel', '', 'yticklabel', '')
    title(sprintf('Top-%d Colors',k))
end
