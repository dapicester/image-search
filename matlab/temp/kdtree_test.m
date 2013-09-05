function kdtree_test(varargin)

opts.dimensions = 166 + 300;
opts.query = 1;
opts.neighbors = 10;
opts = vl_argparse(opts, varargin);

num_samples = [10^5 10^6];
num_trees = [1 2 4 10];
max_num_comparisons = [0 10000 100];

for n = 1:numel(num_samples);
    [data, query] = generate_data(num_samples(n), opts.dimensions, opts.query);

    build_time = zeros(numel(num_trees),  numel(max_num_comparisons));
    query_time = zeros(numel(num_trees),  numel(max_num_comparisons));
    for t = 1:numel(num_trees)
        for c = 1:numel(max_num_comparisons)
            [b,q] = do_test(num_trees(t), max_num_comparisons(c));
            build_time(t,c) = b;
            query_time(t,c) = q;
        end
    end
end


function [data, query] = generate_data(num_samples, num_dimensions, num_query)
    disp('Generating random data ...')
    tic
    randn('state', 0);
    data = randn(num_dimensions, num_samples);
    query = randn(num_dimensions, num_query);
    toc
end

function [build_time, query_time] = do_test(num_trees, max_num_comparisons)
    fprintf('Building index with %d trees ...\n', num_trees);
    tic
    tree = vl_kdtreebuild(data, 'verbose', 'NumTrees', num_trees);
    build_time = toc;

    fprintf('Querying the index with %d max comparisons ...\n', max_num_comparisons);
    tic
    vl_kdtreequery(tree, data, query, 'verbose', ...
                           'NumNeighbors', opts.neighbors, ...
                           'MaxNumComparisons', max_num_comparisons);
    query_time = toc;
end

end % kdtree_test

