function display_matches(image, filenames, indexes, scores)

multi = true;

matches = filenames(indexes(indexes~=0)); % discard any 0

if multi
    imdisp(matches, 'size', 5);
else
    subplot(121), subimage(image), axis off, title('query image');
    for j = 1:length(matches);
        img = imread(char(matches{j}));
        subplot(122), subimage(img), axis off, title(sprintf('rank: %d',scores(j)));
        pause
    end
    close
end
