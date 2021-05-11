#include "Draw.h"

class Texture
{
protected:
    int size;
    std::vector<ImVec2> objectPoints;
    glm::mat3 patchMatrix;
    ImVec2 center;
    int length;
    int breadth;
   std::vector<PlObject> texedObjs;

   public:
    void addPathPoint(ImVec2 p){
        objectPoints.push_back(p);
    }

    ImVec2 getPathPoint(int i){
        return objectPoints[i];
    }



    void translate(float x, float y)
    {
        x = x * 480;
        y = y * 360; 


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


    void scale(int anchorX, int anchorY, int x, int y)
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
        glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3((float)x,(float)y,1.0));
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

