# SketchBasedAnimation
This tool lets user draw and animate its sketches in real time.

## How to Run
Clone Repo to your system and open terminal and run make file and run build file using following commands

~~~~~ bash
make

./SketchBasedAnimation
~~~~~ 

## Checking for Milestone
![picture alt](https://github.com/surajrathore4/MyWork/blob/master/Sample.jpeg)
![picture alt](https://github.com/surajrathore4/MyWork/blob/master/Sample2.jpeg)


### Evaluation 1

**Milestone 1:**  ***User-Interface: Main Canvas, Global Toolbar*** - Shown in Image

**Milestone 2:** ***Enabling Users to draw on Canvas*** - Draw mode opens as default just press right mouse button on canvas(Section 1 of Image) and drag then release to draw.

**Milestone 3:**  ***Select object and add Basic Transformation to Object*** - choose Select Mode from Section 2 of Image then Hover and click to select Object. for basic Transformation use tools of section 3 (just afer selecting object for first time overall or after pressing reset button click on canvas to provide an Anchor Point to use Scale and Rotate Transformation). Use Button Transform to Save current state and Reset Button to Reset the object to previous saved state.


### Evaluation 2
**Milestone 1:**  ***Emitting Texturing*** - In Select Mode, select an Object and click on Emitting Texture button (in Section 4) to add Emitting Texture to that object.

**Milestone 2:**  ***Tool for creating animated Objects.*** - In Draw Mode, Draw on mini Canvas on Section 5 to create your own animated object which will animate as Emitting objecs.

**Milestone 3:**  ***Oscillating Texturing*** - In Select Mode, select an Object and click on Oscillating Texture button (in Section 4) and click on canvas to provide to add final position of oscillation(only X coordinate).


### Final Evaluation
**Milestone 1:**  ***Graph Mode*** - Click on Graph Mode Button to Activate Graph mode now Select and Translate Objects in Graph Mode.

**Milestone 2:**  ***Cause and Effect Animations*** - In Graph Mode Select Final State in Timer and Translate Objects, see effects in Presentation Mode.

**Milestone 3:**  ***Presentation Mode*** - click on Presentation mode, click on on object to select an object then drag that object to see cause and effect animation. 

## Libraries Used
1) Dear Imgui - For Creating graphical user interface

2) GLM - For Matrix Calculations

3) STB Image - For Image Texturing.
