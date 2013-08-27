function [idx,distance] = composed_distance(indices, ranks, weights, num_query)
% input is cellarray { input1; input2; ...; inputN }
% weight is array [w1,..,wN]

[keys,~,ind] = unique(cat(1,indices{:}));

% full outer join
ind1 = ind(1:end/2,1);
ind2 = ind(end/2+1:end,1);

full_join = zeros(numel(keys),4);
full_join(:,1) = keys;
full_join(ind1,2) = ranks{1}/sum(ranks{1});
full_join(ind2,3) = ranks{2}/sum(ranks{2});

% weighted euclidean distance function (without sqrt)
full_join(:,4) = sqrt(weights(1) * full_join(:,2).^2 + weights(2) * full_join(:,3).^2);
%full_join(:,4) = weights(1) * full_join(:,2) + weights(2) * full_join(:,3);
[distance,idx] = sort(full_join(:,4));

% at most num_query results
n = min(num_query, length(distance));
distance = distance(1:n);
idx = idx(1:n);
