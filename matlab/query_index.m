function [index, rank] = query_index(index, data, query, num)
% QUERY_INDEX  Query the index for NUM neighbors.

% Author: Paolo D'Apice

if nargin < 4, num = 10; end

[index, rank] = vl_kdtreequery(index, data, query, ...
                               'numneighbors', num, ...
                               'maxcomparisons', 0);
