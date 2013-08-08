function drawimage(image, varargin)
    switch nargin
        case 2
            subimage(image)
            title(varargin{1})
        case 3
            subimage(image, varargin{1})
            title(varargin{2})
    end
    axis off
end
