#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <vector>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/mat3x3.hpp> // glm::mat3
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class PlObject
{
private:
    std::vector<ImVec2> objectPoints;
    int size;
    int Objectid;
public:
    PlObject(){
        size = 0;
        Objectid = -1;

    }

    PlObject(int ObjectCount){
        size = 0;
        Objectid = ObjectCount+1;

    }
    
    void addPoint(ImVec2 p){
        objectPoints.push_back(p);
        size++;
        // printf("%d %d\n", (int)p.x, (int)p.y);
        //glm::mat4 trans = glm::mat4(1.0f);
    }
    
    bool clearPoints(){
        objectPoints.clear();
        size = 0;
        // while (!objectPoints.empty())
        // {
        //     objectPoints.pop_back();
        // }
        return objectPoints.empty();
    }
    
    ImVec2* getPoints(){
        return &objectPoints[0];
    }

    ImVec2 getPoint(int i){
        return objectPoints[i];
    }

    int getSize(){
        return size;
    }

    void translate(float x, float y)
    {
        x = x * 960;
        y = y * 720; 


        //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(x, y, 0.0f));
       // vec = trans * vec;
        //std::cout << vec.x  << " " << vec.y  << " " << vec.z << std::endl;

        // translate Every point of our object
        for(int i=0;i<size;i++)
        {
            float x0 = objectPoints[i].x;
            float y0 = objectPoints[i].y;
           // glm::vec4 vec(x0, y0, 0.0f);

            glm::vec4 vec(x0, y0, 0.0f,1.0f);

            vec = trans * vec;
            objectPoints[i].x = vec.x;
            objectPoints[i].y = vec.y;
        }
    }
    void translatePixel(float x, float y)
    {


        //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(x, y, 0.0f));
       // vec = trans * vec;
        //std::cout << vec.x  << " " << vec.y  << " " << vec.z << std::endl;

        // translate Every point of our object
        for(int i=0;i<size;i++)
        {
            float x0 = objectPoints[i].x;
            float y0 = objectPoints[i].y;
           // glm::vec4 vec(x0, y0, 0.0f);

            glm::vec4 vec(x0, y0, 0.0f,1.0f);

            vec = trans * vec;
            objectPoints[i].x = vec.x;
            objectPoints[i].y = vec.y;
        }
    }

    void rotate( int X, int Y, float r)
    {

        // Rotation Matrix
        glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), r, glm::vec3(0.0, 0.0,1.0));

        
         X = X * -1;
         Y = Y * -1;
        // Translation Matrix
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 trans_mat = glm::translate(trans, glm::vec3(X, Y, 0.0f));


         X = X * -1;
         Y = Y * -1;
        // Invers Translation Matrix
        glm::mat4 invtrans_mat = glm::translate(trans, glm::vec3(X, Y, 0.0f));
        glm::mat4 result_mat = invtrans_mat *  rot_mat * trans_mat;


        // rotate Every point of our object
        for(int i=0;i<size;i++)
        {
            float x0 = objectPoints[i].x;
            float y0 = objectPoints[i].y;
            

            glm::vec4 vec(x0, y0, 0.0f,1.0f);

            // Traslate * rotate * transalte
            vec = result_mat * vec;
            //  objectPoints[i].x = vec.x;
           // objectPoints[i].y = vec.y;

            
            objectPoints[i].x = vec.x;
            objectPoints[i].y = vec.y;

        }
        
    }   


    void scale(int anchorX, int anchorY, float x, float y)
    {
        // Scaling Matrix
        anchorX = anchorX * -1;
        anchorY = anchorY * -1;
        // Translation Matrix
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 trans_mat = glm::translate(trans, glm::vec3(anchorX, anchorY, 0.0f));


        anchorX = anchorX * -1;
        anchorY = anchorY * -1;
        // Invers Translation Matrix
        glm::mat4 invtrans_mat = glm::translate(trans, glm::vec3(anchorX, anchorY, 0.0f));
        glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(x,y,1.0));
        glm::mat4 result_mat = invtrans_mat *  scale_mat * trans_mat;

         for(int i=0;i<size;i++)
        {
            float x0 = objectPoints[i].x;
            float y0 = objectPoints[i].y;
           // glm::vec4 vec(x0, y0, 0.0f);

            glm::vec4 vec(x0, y0, 0.0f,1.0f);

            vec = result_mat * vec;
            objectPoints[i].x = vec.x;
            objectPoints[i].y = vec.y;
        }

    }
};