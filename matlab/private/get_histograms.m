function data = get_histograms(query_type, histograms)
% GET_HISTOGRAMS  Select the proper histogram according to the query type.

% Author: Paolo D'Apice

switch query_type
    case 'rgb'
        data = cat(1, histograms.colors)';
    case 'shape'
        data = cat(1, histograms.hog)';
    case 'hsv'
        data = cat(1, histograms.hsvcolors)';
    case 'rgb_shape'
        data = [cat(1, histograms.hog)'; cat(1, histograms.colors)']/2;
    case 'hsv_shape'
        data = [cat(1, histograms.hog)'; cat(1, histograms.hsvcolors)']/2;
    otherwise
        % TODO
        data = [];
end
