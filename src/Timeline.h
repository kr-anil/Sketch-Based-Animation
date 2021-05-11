#include <vector>
#include "PlObject.h"
#include<algorithm> // for copy() and assign() 
#include<iterator> // for back_inserter

class Timeline
{
private:
    std::vector<PlObject> timeFrames[2];
    bool isTfFilled[2];
    int pixelObjectMap[2][961][721];
    std::vector<ImVec2> directions;

public:
    Timeline(){
        for (int i = 0; i < 2; i++){
            isTfFilled[i] = false;
        }
    }
    bool isEmpty(int pos){
        return !isTfFilled[pos];
    }

    void AddTimeFrame(int pos, std::vector<PlObject> PlObjs){
        if(pos>=2 || pos<0){
            std::cout<<"Invalid position to add time frame.\n";
            return;
        }
        timeFrames[pos].clear();
        copy(PlObjs.begin(), PlObjs.end(), back_inserter(timeFrames[pos]));
        
        if(timeFrames[pos].size() > 0){
            isTfFilled[pos] = true;
            // std::cout<<"TimeFrame added at position "<<pos<<"\n";
        }
    }
    void setPixelBuffer(int Time){
        std::vector<PlObject> PlObjects = timeFrames[Time];
        for(int j=0; j < PlObjects.size(); j++){
            for(int i=0; i < PlObjects[j].getSize(); i++){
                ImVec2 t;
                t.x = PlObjects[j].getPoint(i).x;
                t.y = PlObjects[j].getPoint(i).y;
                pixelObjectMap[Time][(int)(t.x)][(int)(t.y)] = j+1;
            }
        }
    }
    int getObjectidonTime(int x, int y,int Time){
        int i = x-2, s=0,j=y-2;
        for(;i<=x+2;i++){
            for(j=y-2;j<=y+2;j++)
                if(pixelObjectMap[Time][i][j]>s)
                    s=pixelObjectMap[Time][i][j];
        }
        return s;
    }
    std::vector<PlObject> getTimeFrame(int pos){
        int p = pos;
        if(isTfFilled[pos]){
            return timeFrames[pos];
        }
        while(p>0 && !isTfFilled[p-1]){
            p--;
        }
        AddTimeFrame(pos, timeFrames[p-1]);
        setPixelBuffer(pos);
        return timeFrames[pos];
    }
    void saveSelectedOnTime(int Time, int selected, PlObject selectedPlObj){
        int n = selectedPlObj.getSize();
        std::vector<PlObject> PlObjects = timeFrames[Time];
        for(int i=0; i<PlObjects[selected-1].getSize(); i++){
            ImVec2 t = PlObjects[selected-1].getPoint(i);
            if(pixelObjectMap[Time][(int)(t.x)][(int)(t.y)] == selected){
                pixelObjectMap[Time][(int)(t.x)][(int)(t.y)] = 0;
            }
            
        }

        if(!PlObjects[selected-1].clearPoints())
            std::cout<<"Not able to clear object points.";
        
        for(int i=0; i<n; i++){
            ImVec2 t;
            t.x = selectedPlObj.getPoint(i).x;
            t.y = selectedPlObj.getPoint(i).y;
            PlObjects[selected-1].addPoint(t);
            pixelObjectMap[Time][(int)(t.x)][(int)(t.y)] = selected;
        }
        timeFrames[Time] = PlObjects;
    }
    bool setFrameDirections(){
        if(timeFrames[1].size()==0 || timeFrames[0].size()==0){
            return false;
        }
        directions.clear();
        int n = timeFrames[1].size();
        for(int i=0;i<n;i++){
            ImVec2 e = timeFrames[1][i].getPoint(0);
            ImVec2 s = timeFrames[0][i].getPoint(0);
            float x = e.x - s.x;
            float y = e.y - s.y;
            directions.push_back(ImVec2(x,y));
        }
        return true;
    }
    std::vector<ImVec2> getFrameDirections(){
        return directions;
    }
    std::vector<PlObject> getAtTimeFrame(float pos){
        std::vector<PlObject> PlObjs;
        copy(timeFrames[0].begin(), timeFrames[0].end(), back_inserter(PlObjs));
        int n = directions.size();
        float x, y;
        for(int i=0;i<n;i++){
            x = directions[i].x*pos;
            y = directions[i].y*pos;
            // std::cout<<i<<" "<<x<<" "<<y<<"\n";
            if(x!=0 || y!=0){
                PlObjs[i].translatePixel(x,y);
            }
        }
        return PlObjs;
    }
};

