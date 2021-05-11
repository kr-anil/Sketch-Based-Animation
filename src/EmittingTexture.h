#include "Texture.h"
#include <stdlib.h>

class EmittingTexture : Texture
{
private:
    /* data */
    std::vector<std::vector<ImVec2>> Patchs;
    PlObject source;
    int max_height;
    int objectcount;

public:
    EmittingTexture(PlObject s);
    ~EmittingTexture();
    void update();
    void createSampleObject();
    int getObjectCount();
    ImVec2 getPathPoint(int i){
        return objectPoints[i];
    }

};

EmittingTexture::EmittingTexture(PlObject s)
{
	source = s;
	size = s.getSize();
	max_height = 1200;
	objectcount=0;
}
void EmittingTexture::createSampleObject(){
	int p = rand()%size;
	ImVec2 pos = source.getPoint(p);
	objectPoints.push_back(pos);
	objectcount++;
}
void EmittingTexture::update(){
	int i=0,x,y;
	for(i=0;i<objectcount;i++){
		objectPoints[i].y +=5;
		if(objectPoints[i].y>720){
			x = objectPoints[i].x;
			y = objectPoints[i].y;
			objectPoints[i].x = objectPoints[objectcount-1].x;
			objectPoints[i].y = objectPoints[objectcount-1].y;
			objectPoints[objectcount-1].x = x;
			objectPoints[objectcount-1].y = y;
			objectPoints.pop_back();
			i--;
			objectcount--;
		}
	}
}
int EmittingTexture::getObjectCount(){
	return objectcount;
}
EmittingTexture::~EmittingTexture()
{
}
