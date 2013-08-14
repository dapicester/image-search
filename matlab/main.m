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

% Last Modified by GUIDE v2.5 14-Aug-2013 15:39:40

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

% Choose default command line output for main
handles.output = hObject;

% Store file contents
handles.query_names = load_query_names;

% Update handles structure
guidata(hObject, handles);

setup;
environment;
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
names = textread(filename, '%s');


% --- gets the currently selected category.
function category = get_category(handles)
selected = get(handles.category_panel, 'SelectedObject');
category = get(selected, 'String');


% --- get the currently selected query_type.
function type = get_query_type(handles)
selected = get(handles.query_panel, 'SelectedObject');
type = get(selected, 'String');
    

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


% --- Displays all images beloging to the given category.
function show_all(category)
global DATA_DIR
names = readFileNames(category, DATA_DIR);
figure(2), imdisp(names, 'Size', 4);


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


% --- selects histograms according to the query_type.
function data = get_histograms(query_type, histograms)
switch query_type
    case 'color'
        data = cat(1, histograms.colors)';
    case 'shape'
        data = cat(1, histograms.hog)';
    case 'combined'
        data = [cat(1, histograms.hog)'; cat(1, histograms.colors)'];
end


% --- gets the indesx according to the query_type.
function index = get_index(category, query_type, data)
index = build_index(category, data, query_type, 'force', false);


% --- display results
function show_results(names, handles)
for i = 1:16
    handle = handles.(['axes' num2str(i)]);
    imshow(names{i}, 'Parent', handle);
end

% === My functions end ===


% --- Executes on button press in search_button.
function search_button_Callback(~, ~, handles) %#ok<DEFNU>
% corpus
category = get_category(handles);
vocabulary = buildVocabulary(category, 'force', false);
[histograms, names] = buildHistograms(category, vocabulary, 'force', false);
query_type = get_query_type(handles);
data = get_histograms(query_type, histograms);
index = get_index(category, query_type, data);

% query image
image = imread(get_query_filename(handles.query_list));
query = computeHistogramFromImage(vocabulary, image);
query_data = get_histograms(query_type, query);
[indices,rank] = query_index(index, data, query_data, 16);
matches = names(indices(indices~=0)); % discard any 0
show_results(matches, handles);


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


% --- Executes when selected object is changed in category_panel.
function category_panel_SelectionChangeFcn(~, ~, handles) %#ok<DEFNU>
set_query_names(handles);


% --- Executes on button press in build_index_button.
function build_index_button_Callback(~, ~, handles) %#ok<DEFNU>


% --- Executes on button press in histogram_button.
function histogram_button_Callback(~, ~, handles) %#ok<DEFNU>
