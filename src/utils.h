// #include "PlObject.h"
#include "Timeline.h"
// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>            // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE

int pixelObjectMap[961][721];
int selected = 0, hovered = 0;

static int ObjectCount = -1;
static float transX = 0.0f, transY = 0.0f, ScaleX = 1.0f, ScaleY = 1.0f, Rotate = 0.0f;
static float prevTransX = -1.0f, prevTransY = -1.0f, prevScaleX = -1.0f, prevScaleY = -1.0f, prevRotate = -1.0f;

bool leftMouseDown = false, sampleLeftMouseDown = false;

double prevX,prevY;

std::vector<PlObject> PlObjects;
PlObject selectedPlObj;

std::vector<ImVec2> samplePoints;
int sampleSize = 0,sampleSize1=0;
int sampleY = 720;
long sampleX=0;
float timeframe;
int startX=-1, startY=-1;

int eText = 0;

// Oscillation Texture Variable part
int osText = 0; // flag for oscillation texture
int initialX, finalX, currentX;
int initialY, finalY, currentY;
std::vector<ImVec2> oscillationObject;   
int counter = 0;
int choose_final = 0;
int oscillationObjectid=0;

std::vector<PlObject> PresentPlObjects;

std::vector<ImVec2> directions;


// TimeLine Object
Timeline TL;

static int getObjectid(int x, int y){
	int i = x-2, s=0,j=y-2;
	for(;i<=x+2;i++){
		for(j=y-2;j<=y+2;j++)
			if(pixelObjectMap[i][j]>s)
				s=pixelObjectMap[i][j];
	}
    return s;
}
static void selectCurve(ImGuiIO io)
{
    int x,y;
    x = (int)(io.MousePos.x);
    y = (int)(io.MousePos.y);
    if(ImGui::GetIO().MouseClicked[0]){
        if(x>0 && x<960 && y>0 && y<720){
        	// printf("%d %d\n", x, y);
        	int objid = getObjectid(x,y);
        	if(objid!=0){
                if(selected!=objid){
                    transX = 0;
                    transY = 0;
                    ScaleX = 1;
                    ScaleY = 1;
                    Rotate = 0;
                }
            	selected = objid;
                prevTransX = -1.0f;
                prevTransY = -1.0f;
                prevScaleX = -1.0f;
                prevScaleY = -1.0f;
                prevRotate = -1.0f;
        	}
    	}
    }
    hovered = 0;
    if(x>0 && x<960 && y>0 && y<720){
    	int objid = getObjectid(x,y);
    	if(objid!=0){
        	hovered = objid;
    	}
	}
}
static void selectCurveOnTime(ImGuiIO io, int Time){
    int x,y;
    x = (int)(io.MousePos.x);
    y = (int)(io.MousePos.y);
    if(ImGui::GetIO().MouseClicked[0]){
        if(x>0 && x<960 && y>0 && y<720){
            // printf("%d %d\n", x, y);
            int objid = TL.getObjectidonTime(x,y,Time);
            if(objid!=0){
                if(selected!=objid){
                    transX = 0;
                    transY = 0;
                    ScaleX = 1;
                    ScaleY = 1;
                    Rotate = 0;
                }
                selected = objid;
                prevTransX = -1.0f;
                prevTransY = -1.0f;
                prevScaleX = -1.0f;
                prevScaleY = -1.0f;
                prevRotate = -1.0f;
            }
        }
    }
    hovered = 0;
    if(x>0 && x<960 && y>0 && y<720){
        int objid = TL.getObjectidonTime(x,y,Time);
        if(objid!=0){
            hovered = objid;
        }
    }
}
void UpdatePresentation(float x, float y){
    if(!ImGui::GetIO().MouseDown[0]){
        startX = -1;
        startY = -1;
        return;
    }
    if(startX>0 && startY>0){
        float meg = (directions[selected-1].x*directions[selected-1].x)+(directions[selected-1].y*directions[selected-1].y);
        if(meg!=0){
            float state = ((directions[selected-1].x*(x - startX)) + (directions[selected-1].y*(y - startY)))/meg;
            //std::cout<<directions[selected-1].x<<" "<<directions[selected-1].y<<" "<<x<<" "<<y<<"\n";
            if(state>=0 && state<=1)
                PresentPlObjects=TL.getAtTimeFrame(state);
        }
    }
}
void selectPresentationCurve(){
    int x,y;
    ImVec2 v = ImGui::GetMousePos();
    x = (int)(v.x);
    y = (int)(v.y);
    if(ImGui::GetIO().MouseClicked[0]){
        if(x>0 && x<960 && y>0 && y<720){
            // printf("%d %d\n", x, y);
            int objid = getObjectid(x,y);
            if(objid!=0){
                selected = objid;
                startX=x;
                startY=y;
                //std::cout<<startX<<" "<<startY<<"\n";
            }
        }
    }
    hovered = 0;
    if(x>0 && x<960 && y>0 && y<720){
        int objid = getObjectid(x,y);
        if(objid!=0){
            hovered = objid;
        }
    }
}
std::vector<ImVec2> translate(float x, float y)
{
    std::vector<ImVec2> sampleEmitPoints;
    int t = -1*((int)(sampleX/sampleSize)), s = -1*sampleY;

    //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(t, s, 0.0f));
    trans = glm::translate(trans, glm::vec3(x, y, 0.0f));
   // vec = trans * vec;
    //std::cout << vec.x  << " " << vec.y  << " " << vec.z << std::endl;

    // translate Every point of our object
    for(int i=0;i<sampleSize;i++)
    {
        float x0 = samplePoints[i].x;
        float y0 = samplePoints[i].y;
       // glm::vec4 vec(x0, y0, 0.0f);

        glm::vec4 vec(x0, y0, 0.0f,1.0f);

        vec = trans * vec;
        if(vec.x!=0 || vec.y!=0){
            sampleEmitPoints.push_back(ImVec2(vec.x, vec.y));
            sampleSize1++;
        }
    }
    //printf("%d\n", sampleEmitPoints.size());
    if(sampleSize!=sampleEmitPoints.size()){
        printf("%d %d\n",sampleEmitPoints.size(), sampleSize);
    }
    return sampleEmitPoints;
}




 // Make Global Variable : 
 // std::vector<ImVec2>  :  oscillationObject , oscillationSize, counter
 // 2 function : oscillationUpdate(), oscillationCreate()
 // create(objectid) : long sum = 0; global variable :  initialX, finalX, currentX ; 
 //  

void oscillationCreate(int objectid)
{   int oscillationSize = PlObjects[objectid-1].getSize();
   
   long midX = 0; // to store average values of X  and Y in vector of Points(x,y) 
   //long midY = 0
    // copying object into another object : copy vector elements
   for(int i=0; i<oscillationSize; i++)
   {
    oscillationObject.push_back(ImVec2(PlObjects[objectid-1].getPoint(i).x, PlObjects[objectid-1].getPoint(i).y));
    midX = midX + PlObjects[objectid-1].getPoint(i).x;
    //midY = midY + PlObjects[objectid-1][i].y;
   }

   //midX = midX / oscillationSize;
   //midY = midY / oscillationSize;

   initialX = (int)(midX / oscillationSize);
   currentX = initialX;
  //

   // std::cout << finalX << " " << initialX <<" " << counter << "\n";
}

void oscillationUpdate()
{

    if(currentX >= finalX || currentX <= initialX)
    {
         // swapping initialX and finalX
        // int temp = finalX;
        // finalX = initialX;
        // initialX = finalX;
        counter = counter * -1; // counter is set to negative  : starts in opposite direction
    }

    currentX = currentX + counter;

    for(int i=0; i<oscillationObject.size(); i++)
    {   

        oscillationObject[i].x = oscillationObject[i].x +  counter;
        oscillationObject[i].y = oscillationObject[i].y + 0;
    }    


}

/*
PlObject Oscillate(ImGuiIO io)
{
    PlObject Obj; 

    for(int i=0; i<PlObjects.size(); i++)
    {
        if(PlObjects[i].getObjectid() == selected)
        {
            obj(PlObjects[i]);
        }
    } 

     ImVec2 distFromClick(io.MouseClickedPos[0]);
     int finalX = (int)(distFromClick.x);
     int y = (int)(distFromClick.y);

     int oscillateX = finalX / 16 ;
      
    

    



    for(int i=0; i<obj.getsize(); i++)
    {   

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(oscillateX, oscillateY, 0.0f));
        
        float x0 = oscillationObject[i].x;
        float y0 = oscillationObject[i].y;
        // glm::vec4 vec(x0, y0, 0.0f);

        glm::vec4 vec(x0, y0, 0.0f,1.0f);

        vec = trans * vec;
        obj[i].x = vec.x;
        obj[i].y = vec.y;

         x = x + 16;

    }    






}
*/


void transformSelectedPlObj(int anchorX, int anchorY){
    // if(transX != prevTransX || transY != prevTransY || ScaleX != prevScaleX || ScaleY != prevScaleY || Rotate != prevRotate){
        int n = PlObjects[selected-1].getSize();
        if(!selectedPlObj.clearPoints())
            std::cout<<"Not able to clear object points.";
        
        for(int i=0; i<n; i++){
            ImVec2 t;
            t.x = PlObjects[selected-1].getPoint(i).x;
            t.y = PlObjects[selected-1].getPoint(i).y;
            selectedPlObj.addPoint(t);
        }

        selectedPlObj.translate(transX, transY);
        selectedPlObj.scale(anchorX, anchorY, ScaleX, ScaleY);
        selectedPlObj.rotate(anchorX, anchorY, Rotate);
    // }
    
    
}
void transformSelectedOnTime(int Time){
    // if(transX != prevTransX || transY != prevTransY || ScaleX != prevScaleX || ScaleY != prevScaleY || Rotate != prevRotate){
        std::vector<PlObject> PlObs = TL.getTimeFrame(Time);
        int n = PlObs[selected-1].getSize();
        if(!selectedPlObj.clearPoints())
            std::cout<<"Not able to clear object points.";
        
        for(int i=0; i<n; i++){
            ImVec2 t;
            t.x = PlObs[selected-1].getPoint(i).x;
            t.y = PlObs[selected-1].getPoint(i).y;
            selectedPlObj.addPoint(t);
        }

        selectedPlObj.translate(transX, transY);
    // }
    
    
}

void saveSelectedPlObj(){
    int n = selectedPlObj.getSize();
    for(int i=0; i<PlObjects[selected-1].getSize(); i++){
        ImVec2 t = PlObjects[selected-1].getPoint(i);
        if(pixelObjectMap[(int)(t.x)][(int)(t.y)] == selected){
            pixelObjectMap[(int)(t.x)][(int)(t.y)] = 0;
        }
        
    }

    if(!PlObjects[selected-1].clearPoints())
        std::cout<<"Not able to clear object points.";
    
    for(int i=0; i<n; i++){
        ImVec2 t;
        t.x = selectedPlObj.getPoint(i).x;
        t.y = selectedPlObj.getPoint(i).y;
        PlObjects[selected-1].addPoint(t);
        pixelObjectMap[(int)(t.x)][(int)(t.y)] = selected;
    }
}

bool load_texture( const char *file_name, GLuint *tex ) {
    int x, y, n;
    int force_channels = 4;
    unsigned char *image_data = stbi_load( file_name, &x, &y, &n, force_channels );
    if ( !image_data ) {
        fprintf( stderr, "ERROR: could not load %s\n", file_name );
        return false;
    }
    // NPOT check
    if ( ( x & ( x - 1 ) ) != 0 || ( y & ( y - 1 ) ) != 0 ) {
        // fprintf( stderr, "WARNING: texture %s is not power-of-2 dimensions\n",
        //                  file_name );
    }
    int width_in_bytes = x * 4;
    unsigned char *top = NULL;
    unsigned char *bottom = NULL;
    unsigned char temp = 0;
    int half_height = y / 2;

    for ( int row = 0; row < half_height; row++ ) {
        top = image_data + row * width_in_bytes;
        bottom = image_data + ( y - row - 1 ) * width_in_bytes;
        for ( int col = 0; col < width_in_bytes; col++ ) {
            temp = *top;
            *top = *bottom;
            *bottom = temp;
            top++;
            bottom++;
        }
    }
    glGenTextures( 1, tex );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, *tex );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                                image_data );
    glGenerateMipmap( GL_TEXTURE_2D );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    GLfloat max_aniso = 0.0f;
    glGetFloatv( GL_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso );
    // set the maximum!
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso );
    return true;
}