function [topk,fk] = topk_colors(colors, freqs, k)
% TOPK_COLORS  Retrieves the top-k colors according to their occurence.
%
%  TOPK = TOPK_COLORS(COLORS, FREQS, K)
%
%  [TOPK, REL_FREQ] = TOPK_COLORS(...)  returns also the relative frequency.

% Author: Paolo D'Apice

[~, idx] = sort(freqs, 'descend');
idx = idx(1:k);
topk = colors(idx,:);
fk = freqs(idx,:)./sum(freqs);