# IMGUI

## GUI IN OPENGL
1. Using GUI framework.
construct the GUI screen and make openGL surface.
-> win32, MFC, QT, Cocoa, android...

2. making GUI component in inside of openGL screen. Then act on event by using library.
-> ImGui !

---

github.com/ocornut/imgui
open source library made by Omar Cornut
Immediate-Mode GUI.
Directly drawing the GUI components by using graphics API.

## Feature of IMGUI
- Immediate Mode GUI: 
make ui components every rendering loops.
this feature is easy to use. but it might be mixed with other code treating the graphics objects.

- separating Rendering backend 
: It can be used with several graphics API.
if you want to implement directly, you can code rendering backend.

- There is a GUI component for graphics programming.
: vector editor, color picker. 

- Nearly zero-dependency
: easy to build.

## Install IMGUI
There is no makefile and cmkae in IMGUI repo.
so, download the sourcefiles and include it in your repo. menually.

When you run the exe, UI component by ImGui is overlapping on openGL screen.
Also it can be resized and moved by mouse.

- ImGui_ImplGlfw_NewFrame() implemented in main loop updates the screen size and mouse status from GLFWwindow.

- So, We don't need to connect callback function. 

## UI / PARAMETER BINDING
Add the camera parameters, camers reset button and clear color in UI.

One function responed to one UI component.
and the function returns boolean type data.
if the return is true, the UI component has been changed.
the action logic about UI event can be coded by if statement.

## IMGUI CALLBACK SETTING
the callback function we already made can include the ImGUi callback.
Include the ImGui callback action in these callback function or there is no callback function, you can write it like opengl callback function.

## How to study ImGui?
There is no documents for ImGui guide. so all the information about UI component is in `imgui.h`.
`imgui_demo.cpp` has an examples of imgui functions. refer it for using.

