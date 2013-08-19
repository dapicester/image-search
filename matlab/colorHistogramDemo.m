function colorHistogramDemo(varargin)
% COLORHISTOGRAMDEMO Extract color histograms from sample images.
%
% Options:
%
%   Colors:: [256]
%     The number of colors on which compute the histogram.
%
%   Level:: [1]
%     Pyramidal level, set to 0 to disable.
%
%   TopK:: [10]
%     The top-K colors, set to 0 to disable.
%
%   Save:: ['no']
%     Set the save format (available 'eps', 'png')

% Author: Paolo D'Apice

clc
setup('all');

opts.colors = 256;
opts.level = 1;
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
    image = standardizeImage(image, 'height', 128);
    [~, colors, freqs, quant] = colorHistogram(image, colormap, 'level', opts.level);
    
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
    rows = opts.level + 2;
    subplot(rows,2,1), drawimage(image, 'Original')
    subplot(rows,2,2), drawimage(quant, colormap, 'Quantized')
    
    ind2col = @(idx) colormap(idx + 1, :); % index 1-based
    
    % TODO refactor code
    
    if opts.topk > 0
        % level 0
        [topk, fk] = topk_colors(colors{1}, freqs{1}, opts.topk);
        subplot(rows,2,3), coloredbar(ind2col(colors{1}), freqs{1})
        subplot(rows,2,4), drawtopk(ind2col(topk), fk);
   
        if opts.level > 0
            % show only level 1
            subplot(6,4,17), coloredbar(ind2col(colors{2}), freqs{2})
            subplot(6,4,18), coloredbar(ind2col(colors{3}), freqs{3})
            subplot(6,4,21), coloredbar(ind2col(colors{4}), freqs{4})
            subplot(6,4,22), coloredbar(ind2col(colors{5}), freqs{5})

            [topk, fk] = topk_colors(colors{2}, freqs{2}, opts.topk);
            subplot(6,4,19), drawtopk(ind2col(topk), fk);
            [topk, fk] = topk_colors(colors{3}, freqs{3}, opts.topk);
            subplot(6,4,20), drawtopk(ind2col(topk), fk);
            [topk, fk] = topk_colors(colors{4}, freqs{4}, opts.topk);
            subplot(6,4,23), drawtopk(ind2col(topk), fk);
            [topk, fk] = topk_colors(colors{5}, freqs{5}, opts.topk);
            subplot(6,4,24), drawtopk(ind2col(topk), fk);
        end
    else
        % level 0
        subplot(rows,2,[3 4]), coloredbar(ind2col(colors{1}), freqs{1})
        
        if opts.level > 0
            % show only level 1
            subplot(6,4,[17 18]), coloredbar(ind2col(colors{2}), freqs{2})
            subplot(6,4,[19 20]), coloredbar(ind2col(colors{3}), freqs{3})
            subplot(6,4,[21 22]), coloredbar(ind2col(colors{4}), freqs{4})
            subplot(6,4,[23 24]), coloredbar(ind2col(colors{5}), freqs{5})
        end
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
