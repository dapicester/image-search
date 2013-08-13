function [index, rank] = query_index(index, data, query, num)

if nargin < 4, num = 10; end

[index, rank] = vl_kdtreequery(index, data, query, ...
                               'numneighbors', num, ...
                               'maxcomparisons', 0);
