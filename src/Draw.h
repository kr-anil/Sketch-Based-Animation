#include "utils.h"

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

#include <array>

// #include <glm/vec3.hpp> // glm::vec3
// #include <glm/vec4.hpp> // glm::vec4
// #include <glm/mat4x4.hpp> // glm::mat4
// #include <glm/gtc/matrix_transform.hpp> 
// #include <glm/glm.hpp>
// #include <glm/gtc/type_ptr.hpp>
// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.


double x,y;

void draw(GLFWwindow *canvas){
    glfwGetCursorPos(canvas, &x, &y);
    // printf("%s\n", leftMouseDown ? "true" : "false");
    if(sampleLeftMouseDown){
        if(y<sampleY)
            sampleY = y;
        sampleX = sampleX+x;
        ImVec2 s(x,y);
        samplePoints.push_back(s);
        sampleSize++;
    }
    if(leftMouseDown){
        ImVec2 s(x,y);
        if(x == prevX && y == prevY)
            return;
        PlObjects[ObjectCount].addPoint(s);
        // pixelObjectMap[(int)(x)][(int)(y)] = ObjectCount+1;
    }
}
void setleftmouseDown(ImGuiIO io){
    if(ImGui::GetIO().MouseDown[0]){
        if(io.MousePos.x>0 && io.MousePos.x<960 && io.MousePos.y>0 && io.MousePos.y<720){
            if(!leftMouseDown){
                //printf("%d %d\n", Objects.size(), Objectcount);
                ObjectCount++;
                PlObjects.push_back(PlObject(ObjectCount));
            }
            prevX = io.MousePos.x;
            prevY = io.MousePos.y;
            leftMouseDown = true;
        }
        else{
            leftMouseDown = false;
        }



        if(io.MousePos.x>980 && io.MousePos.x<1260 && io.MousePos.y>535 && io.MousePos.y<775){
            if(sampleLeftMouseDown){
                //printf("%d %d\n", Objects.size(), Objectcount);
                sampleLeftMouseDown = true;
            }
            else if(sampleSize==0 && eText==0){
                sampleLeftMouseDown = true;
            }
            else{
                sampleLeftMouseDown = false;
            }
        }
        else{
            sampleLeftMouseDown = false;
        }
    }
    else{
        leftMouseDown = false;
        sampleLeftMouseDown = false;
    }
}