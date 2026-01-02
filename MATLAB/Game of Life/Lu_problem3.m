function varargout = Lu_problem3(varargin)
% LU_PROBLEM3 MATLAB code for Lu_problem3.fig
%      LU_PROBLEM3, by itself, creates a new LU_PROBLEM3 or raises the existing
%      singleton*.
%
%      H = LU_PROBLEM3 returns the handle to a new LU_PROBLEM3 or the handle to
%      the existing singleton*.
%
%      LU_PROBLEM3('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in LU_PROBLEM3.M with the given input arguments.
%
%      LU_PROBLEM3('Property','Value',...) creates a new LU_PROBLEM3 or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Lu_problem3_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Lu_problem3_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Lu_problem3

% Last Modified by GUIDE v2.5 15-Jan-2024 12:29:11

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Lu_problem3_OpeningFcn, ...
                   'gui_OutputFcn',  @Lu_problem3_OutputFcn, ...
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


% --- Executes just before Lu_problem3 is made visible.
function Lu_problem3_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Lu_problem3 (see VARARGIN)

% Choose default command line output for Lu_problem3
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes Lu_problem3 wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = Lu_problem3_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in import_button.
function import_button_Callback(hObject, eventdata, handles)
% hObject    handle to import_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%Gets file name entered by user from text box
input_file = get(handles.text_file, 'String');
%Gets matrix from inputted file
mat = RLE_decoder(input_file);
%Pads the array with 100 zeros on each side
mat = padarray(mat, [100, 100]);

setappdata(handles.import_button, 'matrix', mat)



function text_file_Callback(hObject, eventdata, handles)
% hObject    handle to text_file (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of text_file as text
%        str2double(get(hObject,'String')) returns contents of text_file as a double



% --- Executes during object creation, after setting all properties.
function text_file_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text_file (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in random_matrix_button.
function random_matrix_button_Callback(hObject, eventdata, handles)
% hObject    handle to random_matrix_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%Creates 50 by 50 matrix with randomly selected values of zero or one
for r=1:50
    for c=1:50
        mat(r, c) = fix(rand*2);
    end
end

%Pads array
mat = padarray(mat, [100, 100]);

setappdata(handles.import_button, 'matrix', mat)


% --- Executes on button press in load_button.
function load_button_Callback(hObject, eventdata, handles)
% hObject    handle to load_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%Always resets the current generation to zero
generation = 0;

%Displays current matrix image onto screen
mat = getappdata(handles.import_button, 'matrix');
%Displays image while storing it in i
i = imagesc(mat);
colormap(gray)
%Puts the current image's CData into a cell
frame{generation+1} = get(i,'CData');
%Gives the image the title
title(['Generation: ', num2str(generation)])

setappdata(handles.load_button, 'numGenerations', generation)
setappdata(handles.load_button, 'frames', frame)


% --- Executes on button press in step_button.
function step_button_Callback(hObject, eventdata, handles)
% hObject    handle to step_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
generation = getappdata(handles.load_button, 'numGenerations');
frame = getappdata(handles.load_button, 'frames');
mat = getappdata(handles.import_button, 'matrix');

%Increments current generation by one
generation = generation + 1;

%Gets next generation of mat
mat = GOL(mat);

i = imagesc(mat);
colormap(gray)
frame{generation+1} = get(i,'CData');
title(['Generation: ', num2str(generation)])

setappdata(handles.import_button, 'matrix', mat)
setappdata(handles.load_button, 'numGenerations', generation)
setappdata(handles.load_button, 'frames', frame)



% --- Executes on button press in play_button.
function play_button_Callback(hObject, eventdata, handles)
% hObject    handle to play_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%Checks if user has used the generation button
if getappdata(handles.generations, 'genPressed') == true
    %Sets genToRun to the current option on the pop up menu
    genToRun = getappdata(handles.generations, 'numGenToRun');
else
    %Default (if user havent used the generation button)
    genToRun = 10;
end

generation = getappdata(handles.load_button, 'numGenerations');
mat = getappdata(handles.import_button, 'matrix');
frame = getappdata(handles.load_button, 'frames');

%Plays for specified generations
for i = 1:genToRun
    %Checks if the stop button has been pressed
    button_state = getappdata(handles.stop_button, 'stopPressed');
    if (button_state == 1)
        %Terminates the playing
        return;
    end

    generation = generation + 1;
    mat = GOL(mat);
    i = imagesc(mat);
    colormap(gray)
    frame{generation+1} = get(i,'CData');
    title(['Generation: ', num2str(generation)])
    %Ensures that each frame is displayed
    drawnow;
end

setappdata(handles.import_button, 'matrix', mat)
setappdata(handles.load_button, 'numGenerations', generation)
setappdata(handles.load_button, 'frames', frame)


% --- Executes on selection change in generations.
function generations_Callback(hObject, eventdata, handles)
% hObject    handle to generations (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns generations contents as cell array
%        contents{get(hObject,'Value')} returns selected item from generations

%Gets current option in the pop up menu
contents = get(handles.generations,'String'); 
val = contents{get(handles.generations,'Value')};

%Sets genToRun to specified option
switch val
    case '10 Generations'
        genToRun = 10;
    case '100 Generations'
        genToRun = 100;
    case '1000 Generations'
        genToRun = 1000;
    case '10000 Generations'
        genToRun = 1000;
end

setappdata(handles.generations, 'numGenToRun', genToRun)
%User has used this button
setappdata(handles.generations, 'genPressed', true)


% --- Executes during object creation, after setting all properties.
function generations_CreateFcn(hObject, eventdata, handles)
% hObject    handle to generations (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in stop_button.
function stop_button_Callback(hObject, eventdata, handles)
% hObject    handle to stop_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of stop_button

%Checks and stores the state of the toggle button 
button_state = get(hObject,'Value');
setappdata(handles.stop_button, 'stopPressed', button_state)


% --- Executes on button press in movie_button.
function movie_button_Callback(hObject, eventdata, handles)
% hObject    handle to movie_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%Checks if the user has used the FPS button
if getappdata(handles.FPS, 'FPSPressed') == true
    %Sets FPS to the current option on the pop up menu
    FPS = getappdata(handles.FPS, 'numFPS');
else
    %Default (if the user haven't used the FPS button)
    FPS = 1;
end

frame = getappdata(handles.load_button, 'frames');

%Creates VideoWriter object and sets the frame rate accordingly
v = VideoWriter("Movie", 'MPEG-4');
v.FrameRate = FPS;
open(v);

%Traverses every element in frame 
 for u=1:length(frame)
     %Writes every CData into a png file
     imwrite(frame{u}, 'temp_frame.png');
     %reads the indexed image in filename into X and reads its associated
     %colormap into map
     [X, map] = imread("temp_frame.png");
     %Puts each image into the video object v
     writeVideo(v,X);
 end

%Closes the VideoWriter object
close(v);


% --- Executes on button press in frame_button.
function frame_button_Callback(hObject, eventdata, handles)
% hObject    handle to frame_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%Gets current matrix and exports its corresponding file image
mat = getappdata(handles.import_button, 'matrix');
imwrite(mat, "frame.png");



% --- Executes on button press in clear_button.
function clear_button_Callback(hObject, eventdata, handles)
% hObject    handle to clear_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%Clears current simulation
cla reset;


% --- Executes on selection change in FPS.
function FPS_Callback(hObject, eventdata, handles)
% hObject    handle to FPS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns FPS contents as cell array
%        contents{get(hObject,'Value')} returns selected item from FPS

%Gets current option in the pop up menu
contents = get(handles.FPS,'String'); 
val = contents{get(handles.FPS,'Value')};

%Sets FPS to specified option
switch val
    case '1 FPS'
        FPS = 1;
    case '5 FPS'
        FPS = 5;
    case '10 FPS'
        FPS = 10;
    case '100 FPS'
        FPS = 100;
end

setappdata(handles.FPS, 'numFPS', FPS)
%User has used this button
setappdata(handles.FPS, 'FPSPressed', true)


% --- Executes during object creation, after setting all properties.
function FPS_CreateFcn(hObject, eventdata, handles)
% hObject    handle to FPS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
