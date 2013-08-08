function [colors, freqs, topk] = colorHistogram(image, bins, k)
% COLORHISTOGRAM  Compute the color histogram.
%   [COLORS, FREQS, TOPK] = COLORHISTOGRAM(IMAGE)  Compute color histogram
%   on the given image and return the colors, theirs frequencies and the
%   top-k colors.
%
%   __ = COLORHISTOGRAM(__, bins, k)  Specify on how bins quantize each
%   color channel (default 8) and how many colors include in TOPK (default 10).

% Author: Paolo D'Apice

    if nargin < 2, bins = 8; end
    if nargin < 3, k = 10; end
    
    [quant,step] = quantize(image, bins);
    words = color_words(double(quant), double(step));
    
    [words, freqs] = count_colors(words);
    colors = cell2mat(arrayfun(@(w) color_triplet(w)', words, ...
                               'UniformOutput', false));
                           
    topk = topkcolors(colors, freqs, k);
    
    figure(1)
    set(gcf, 'Units', 'Normalized', ...
             'Position', [0 0 1 1], ...
             'PaperPositionMode', 'auto')
    subplot(221), drawimage(image, 'Original')
    subplot(222), drawimage(quant, 'Quantized')
    subplot(223), coloredbar(colors, freqs)
    subplot(224), drawtopk(topk, k);
end

function [out, step, max_val] = quantize(in, num_levels)
% QUANTIZE  Quantize the input image into num_levels.

% TODO step, levels as arguments

    max_val = max(in(:));
    
    % automatic threshold with Otsu's method
    %levels = multithresh(in, num_levels - 1);
    
    % fixed levels
    step = ceil((max_val+1)/num_levels);
    levels = (step:step:max_val) - 1;    
    
    values = [levels max_val];
    out = imquantize(in, levels, values);
end

function out = color_words(in, step)
% COLOR_WORDS  Encode and sort image colors into words 
%   such that MSB|blue|green|red|LSB.
    out = step * floor(in(:,:,1)/step) ...
        + 256 * (step * floor(in(:,:,2)/step)) ...
        + 256 * 256 * (step * floor(in(:,:,3)/step));
    out = sort(out(:));
end

function out = color_triplet(in)
% COLOR_TRIPLET  Decode RGB color from color words.
    out = ([ rem(in, 256) 
             floor(rem(in, 256*256)/256)
             floor(in/(256*256))         ]/255);
end

function [col, freqs] = count_colors(colors)
% COUNT_COLORS  Get unique colors and their occurrence frequency.
    [~, first] = unique(colors, 'first');
    [col, last]  = unique(colors, 'last');
    freqs = last - first + 1;
end

function top_colors = topkcolors(colors, freqs, k)
% TOPKCOLORS  Retrieve the top-k colors according to their occurence.
    [~, idx] = sort(freqs, 'descend');
    idx = idx(1:k);
    top_colors = colors(idx,:);
end

function drawimage(image, descr)
    subimage(image)
    title(descr)
end

function coloredbar(colors, freqs)
    norm_val = max(freqs) + 1;
    num_val = length(freqs);
    
    cla
    axis([0 num_val 0 1])
    for i = 1:num_val
        color = colors(i,:);
        patch([i-1 i-1 i i], [0 freqs(i)/norm_val freqs(i)/norm_val 0], ...
              color, 'edgecolor', color);
    end
    set(gca,'xticklabel', '', 'yticklabel','')
    set(gca,'visible','off')
    title('Color Histogram')
    set(findall(gca, 'type', 'text'), 'visible', 'on')
end

function drawtopk(colors, k)
    cla
    for i = 1:length(colors)
        patch([i-1 i-1 i i], [0 1 1 0], colors(i,:))
    end
    set(gca,'xticklabel', '', 'yticklabel', '')
    title(sprintf('Top-%d Colors', k))
end

