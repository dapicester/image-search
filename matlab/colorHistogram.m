function [histogram, colors, freqs, topk] = colorHistogram(image, colormap, varargin)
% COLORHISTOGRAM  Computes the color histogram.
%
%   [HISTOGRAM, COLORS, FREQS] = COLORHISTOGRAM(IMAGE, COLORMAP)  Computes
%   color histogram on the COLORMAP and return the color indexes and their
%   normalized frequency.
%
%   [..., TOPK] = COLORHISTOGRAM(..., 'topk', K)  Returns the top-K colors.
%
%   The function accepts the following options:
%
%   K:: [0] 
%     How many TOP-K, set to 0 skip (it returns NaN).

% Author: Paolo D'Apice

    opts.topk = 0;
    opts.display = false;
    [opts, ~] = vl_argparse(opts, varargin);
    
    quant = reduce_colors(image, colormap);
    [colors, freqs] = count_colors(quant);
    histogram = make_histogram(colors, freqs, length(colormap));
    if opts.topk > 0
        [~, topk, fk] = topk_colors(colors, freqs, opts.topk);
    else
        topk = NaN;
    end
    
    if opts.display
        figure(1)
        set(gcf, 'Units', 'Normalized', ...
                 'Position', [0 0 1 1], ...
                 'PaperPositionMode', 'auto')
        subplot(221), drawimage(image, 'Original')
        subplot(222), drawimage(quant, colormap, 'Quantized')
        if opts.topk > 0
            subplot(223), coloredbar(ind2col(colormap, colors), freqs)
            subplot(224), drawtopk(ind2col(colormap, topk), fk);
        else
            subplot(2,2,[3 4]), coloredbar(ind2col(colormap, colors), freqs)
        end
    end
end


function colors = ind2col(map, idx)
% IND2COL Convert indexed colors into RGB triplets.
    colors = map(idx + 1, :); % index 1-based
end

function coloredbar(colors, freqs)
    freqs = freqs./max(freqs); % normalize to 1
    num_val = length(freqs);
    
    cla
    axis([0 num_val 0 1])
    for i = 1:num_val
        color = colors(i,:);
        patch([i-1 i-1 i i], [0 freqs(i) freqs(i) 0], ...
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

