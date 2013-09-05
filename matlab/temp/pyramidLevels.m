function output = pyramidLevels(input, level, fhandle)
% PYRAMIDLEVELS  todo

[width, height] = size(input);

output = [];
for i = 0:level
    l = 2^i;
    w = floor(width/l) .* ones(1, l);
    h = floor(height/l) .* ones(1, l);
    blocks = mat2cell(input(1:sum(w), 1:sum(h)), w, h); % crop to size
    partials = cellfun(fhandle, blocks, 'UniformOutput', false);
    output = cat(1, output, partials(:));
end