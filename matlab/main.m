function varargout = main(varargin)
% MAIN MATLAB code for main.fig
%      MAIN, by itself, creates a new MAIN or raises the existing
%      singleton*.
%
%      H = MAIN returns the handle to a new MAIN or the handle to
%      the existing singleton*.
%
%      MAIN('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MAIN.M with the given input arguments.
%
%      MAIN('Property','Value',...) creates a new MAIN or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before main_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to main_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help main

% Last Modified by GUIDE v2.5 30-Aug-2013 12:19:46

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @main_OpeningFcn, ...
                   'gui_OutputFcn',  @main_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before main is made visible.
function main_OpeningFcn(hObject, ~, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to main (see VARARGIN)

% dependencies and global variables
setup('all');
environment;

% Choose default command line output for main
handles.output = hObject;

% Store file contents
handles.query_names = load_query_names;

% Update handles structure
guidata(hObject, handles);

% initializations
init_gui(handles)


% UIWAIT makes main wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = main_OutputFcn(~, ~, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% === My functions begin ===

% --- initializes gui objects.
function init_gui(handles)
set_query_names(handles);


% --- loads query image names from 'text.txt' files.
function names = load_query_names()
global DATA_DIR
filename = fullfile(DATA_DIR, 'test.txt');
fid = fopen(filename);
names = textscan(fid, '%s');
names = names{:};
fclose(fid);


% --- gets the currently selected category.
function category = get_category(handles)
selected = get(handles.category_panel, 'SelectedObject');
category = get(selected, 'String');


% --- gets the currently selected query_type.
function type = get_query_type(handles)
persistent query_types
query_types = { 'rgb', 'hsv', 'shape', 'rgb_shape', 'hsv_shape', 'weighted' };
type = query_types{get(handles.query_menu, 'Value')};


% --- filters query image names for the selected category.
function names = get_query_names(category, handles)
names = handles.query_names;
names = names(strncmp(names, category, length(category)));


% --- sets the image query names according to the currently selected category.
function set_query_names(handles)
category = get_category(handles);
names = get_query_names(category, handles);
set(handles.query_list, 'String', names);
show_query_image(handles.query_list, handles);


% --- displays all images beloging to the given category.
function show_all(category)
global DATA_DIR
names = readFileNames(category, DATA_DIR);
figure(2), imdisp(names, 'Size', 6);
set(2, 'Name', sprintf('Category: %s (%d)', category, length(names)), ...
       'NumberTitle', 'off', 'MenuBar', 'none');

   
% --- gets the query image filename.
function name = get_query_filename(handle)
global DATA_DIR
contents = cellstr(get(handle, 'String'));
selected = contents{get(handle, 'Value')};
name = fullfile(DATA_DIR, 'test', selected);


% --- displays the currently selected image query.
function show_query_image(handle, handles)
image = imread(get_query_filename(handle));
axes(handles.query_axes)
subimage(image), set(handles.query_axes, 'Visible', 'off')


% --- displays query results.
function show_results(names, ranks, handles)
for i = 1:16
    handle = handles.(['axes' num2str(i)]);
    imshow(names{i}, 'Parent', handle);
    if get(handles.rank_checkbox, 'Value') > 0
        text(5, 20, num2str(ranks(i)), 'Parent', handle)
    end
end


% -- loads or computes vocabulary and histograms.
function [vocabulary, histograms, names] = get_data(category, fv, fh, h)
if nargin < 2, fv = false; end
if nargin < 3, fh = false; end
if nargin < 4, h = []; end
vocabulary = buildVocabulary(category, 'force', fv);
if ~isempty(h), waitbar(.6, h, 'Computing histograms ...'); end
[histograms, names] = buildHistograms(category, vocabulary, 'force', fh);


% --- gets the index according to the query_type.
function index = get_index(category, query_type, data, force)
if nargin < 4, force = false; end
index = build_index(category, data, query_type, 'force', force);


% --- loads index data.
function [data, index] = get_index_data(category, query_type, histograms)
data = get_histograms(query_type, histograms);
index = get_index(category, query_type, data);


% --- performs search.
function [indices,rank,names] = do_search(category, query_type, num_query, handles)
[vocabulary, histograms, names] = get_data(category);
[data, index] = get_index_data(category, query_type, histograms);

% query
image = imread(get_query_filename(handles.query_list));
query = computeHistogramFromImage(vocabulary, image);

query_data = get_histograms(query_type, query);
[indices,rank] = query_index(index, data, query_data, num_query);

% === My functions end ===


% --- Executes on button press in search_button.
function search_button_Callback(~, ~, handles) %#ok<DEFNU>
category = get_category(handles);
query_type = get_query_type(handles);
num_query = 16;
try
    if ~strcmp(query_type, 'weighted')
        [indices,rank,names] = do_search(category, query_type, num_query, handles);
    else
        % XXX work in progress
        msgbox('Not supported yet', 'Query', 'warn', 'modal');
        
        k = 10;
        [indices_c, rank_c] = do_search(category, 'hsv', k*num_query, handles);
        [indices_s, rank_s, names] = do_search(category, 'shape', k*num_query, handles);
    
        % weights
        val = get(handles.weighted_slider, 'Value');
        max = get(handles.weighted_slider, 'Max');
        weight_c = val;
        weight_s = max - val;

        [indices,rank] = composed_distance(...
                {indices_c; indices_s}, {rank_c; rank_s}, ...
                [weight_c, weight_s], num_query);
    end
    
    % show matches
    matches = names(indices(indices~=0)); % discard any 0
    show_results(matches, rank, handles);

catch err
    disp(err.message)
    getReport(err)
    uiwait(msgbox('Index need to be rebuilt!', 'Index', 'warn', 'modal'));
end


% --- Executes on button press in show_all_button.
function show_all_button_Callback(~, ~, handles) %#ok<DEFNU>
show_all(get_category(handles));


% --- Executes on selection change in query_list.
function query_list_Callback(hObject, ~, handles) %#ok<DEFNU>
show_query_image(hObject, handles)


% --- Executes during object creation, after setting all properties.
function query_list_CreateFcn(hObject, ~, ~) %#ok<DEFNU>
% listbox controls usually have a white background on Windows.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function query_menu_CreateFcn(hObject, ~, ~) %#ok<DEFNU>
% popupmenu controls usually have a white background on Windows.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes when selected object is changed in category_panel.
function category_panel_SelectionChangeFcn(~, ~, handles) %#ok<DEFNU>
set_query_names(handles);


% --- Executes on button press in index_button.
function index_button_Callback(~, ~, handles) %#ok<DEFNU>
category = get_category(handles);
query_type = get_query_type(handles);

h = waitbar(.25, 'Indexing ...');
[~, histograms] = get_data(category);

waitbar(.5, h);
data = get_histograms(query_type, histograms);

waitbar(.75, h);
get_index(category, query_type, data, true);

delete(h);


% --- Executes on button press in histogram_button.
function histogram_button_Callback(~, ~, handles) %#ok<DEFNU>
category = get_category(handles);
h = waitbar(.3, 'Computing Histograms ...');
set(h, 'WindowStyle','modal');
t = tic;
get_data(category, false, true, h);
waitbar(1, h, sprintf('Done in %.2f sec', toc(t)));
waitfor(h)


% --- Executes on button press in vocabulary_button.
function vocabulary_button_Callback(~, ~, handles) %#ok<DEFNU>
category = get_category(handles);
h = waitbar(.3, 'Building vocabulary ...');
set(h, 'WindowStyle','modal');
t = tic;
get_data(category, true, true, h);
waitbar(1, h, sprintf('Done in %.2f sec', toc(t)));
waitfor(h)


% --- Executes on selection change in query_menu.
function query_menu_Callback(hObject, ~, handles) %#ok<DEFNU>
value = get(hObject, 'Value');
set(handles.weighted_slider, 'Enable', tif(value == 6, 'on', 'off'))
