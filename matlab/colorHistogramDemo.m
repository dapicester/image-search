function colorHistogramDemo(varargin)
% COLORHISTOGRAMDEMO Extract color histograms from sample images.
%
% Options:
%
%   Colors:: [256]
%     The number of colors on which compute the histogram.
%
%   TopK:: [10]
%     The top-K colors, set to 0 to disable.
%
%   Save:: ['no']
%     Set the save format (available 'eps', 'png')

% Author: Paolo D'Apice

clc
setup;

opts.colors = 256;
opts.topk = 10;
opts.save = 'no';
opts = vl_argparse(opts, varargin);

image_dir = fullfile(fileparts(pwd), 'images');

images = { ... 'peppers.png', 'saturn.png', 'pears.png', ...
           ... 'coloredChips.png', 'gantrycrane.png', ...
           ... 'bag1.jpg', 'shoe1.jpg', 'shoe2.jpg', ...
           'bag1.jpg', 'shoe1.jpg', 'peppers.png' ...
         };

colormap = colorcube(opts.colors);

for str = images
    name = char(str);
    image = load_image(fullfile(image_dir, name));
    
    [~, colors, freqs, quant] = colorHistogram(image, colormap);
    [topk, fk] = topk_colors(colors, freqs, opts.topk);
    
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
    subplot(222), drawimage(quant, colormap, 'Quantized')
    
    ind2col = @(idx) colormap(idx + 1, :); % index 1-based

    if opts.topk > 0
        subplot(223), coloredbar(ind2col(colors), freqs)
        subplot(224), drawtopk(ind2col(topk), fk);
    else
        subplot(2,2,[3 4]), coloredbar(ind2col(colormap, colors), freqs)
    end
    
    switch opts.save
        case 'eps'
            print(sprintf('%s-chist.eps', name), '-depsc2', '-f1')
        case 'png'
            print(sprintf('%s-chist.png', name), '-dpng', '-f1')
    end
end
end % colorHistogramDemo


function coloredbar(colors, freqs)
% COLOREDBAR  Show histogram of colors using colored bars.
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


function [topk,fk] = topk_colors(colors, freqs, k)
% TOPK_COLORS  Retrieves the top-k colors according to their occurence.
    [~, idx] = sort(freqs, 'descend');
    idx = idx(1:k);
    topk = colors(idx,:);
    fk = freqs(idx,:)./sum(freqs);
end

function drawtopk(colors, fk)
% DRAWTOPK  Shows the top-k colors and their frequency.
    cla
    k = length(colors);
    for i = 1:k
        patch([i-1 i-1 i i], [0 1 1 0], colors(i,:))
        text(i-1 + 0.2, 0.5, sprintf('%.2f%%', fk(i)*100))
    end
    set(gca, 'xticklabel', '', 'yticklabel', '')
    title(sprintf('Top-%d Colors',k))
end
