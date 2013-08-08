function [colors, freqs, topk] = colorHistogram(image, bins, k)
% COLORHISTOGRAM  Compute the color histogram.
%
%   [COLORS, FREQS, TOPK] = COLORHISTOGRAM(IMAGE)  Compute color histogram
%   on the given image and return the colors, theirs frequencies and the
%   top-k colors.
%
%   ... = COLORHISTOGRAM(..., bins, k)  Specify how many color bins (defaults
%   to 256) and how many TOP-K (defaults to 10).

% Author: Paolo D'Apice

    if nargin < 2, bins = 256; end
    if nargin < 3, k = 10; end
    
    [quant, map] = reduce_colors(image, bins);
    [colors, freqs] = count_colors(quant, map);                       
    [topk,fk] = topk_colors(colors, freqs, k);
    
    display = true;
    if display
        figure(1)
        set(gcf, 'Units', 'Normalized', ...
                 'Position', [0 0 1 1], ...
                 'PaperPositionMode', 'auto')
        subplot(221), drawimage(image, 'Original')
        subplot(222), drawimage(quant, map, 'Quantized')
        subplot(223), coloredbar(colors, freqs)
        subplot(224), drawtopk(topk, fk);
    end
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
    set(gca, 'xticklabel', '', 'yticklabel','')
    set(gca, 'visible', 'off')
    title('Color Histogram')
    set(findall(gca, 'type', 'text'), 'visible', 'on')
end

function drawtopk(colors, fk)
    cla
    k = length(colors);
    for i = 1:k
        patch([i-1 i-1 i i], [0 1 1 0], colors(i,:))
        text(i-1 + 0.2, 0.5, sprintf('%.2f%%', fk(i)*100))
    end
    set(gca, 'xticklabel', '', 'yticklabel', '')
    title(sprintf('Top-%d Colors',k))
end

