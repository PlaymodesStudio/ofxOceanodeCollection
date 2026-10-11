//
//  modTime.h
//  Genesis
//
//  Created by Eduard Frigola Bagué on 9/5/22.
//

#ifndef modTime_h
#define modTime_h

#include "ofxOceanodeNodeModel.h"

class modTime : public ofxOceanodeNodeModel{
public:
    modTime() : ofxOceanodeNodeModel("Mod Time"){};
    
    void setup(){
        addParameter(input.set("Input", {0}, {0}, {FLT_MAX}));
        addParameter(speed.set("Speed", {1}, {0}, {FLT_MAX}));
        addOutputParameter(output.set("Output", {0}, {0}, {FLT_MAX}));
        
        
        listeners.push(input.newListener([this](vector<float> &vf){
            tempOut.resize(max(vf.size(), speed->size()));
            offset.resize(max(vf.size(), speed->size()));
            for(int i = 0; i < vf.size(); i++){
                tempOut[i] = vf[i] * getValueForPosition(speed.get(), i);
                if(getValueForPosition(speedChanged, i) != 0){
                    offset[i] -= getValueForPosition(vf, i) * getValueForPosition(speedChanged, i);
                }
                tempOut[i] += offset[i];
            }
            std::fill(speedChanged.begin(), speedChanged.end(), 0.0);
            output = tempOut;
            
        }));
        
        listeners.push(speed.newListener([this](vector<float> &vf){
            speedChanged.resize(max(oldSpeed.size(), vf.size()));
            for(int i = 0; i < speedChanged.size(); i++){
                speedChanged[i] = getValueForPosition(vf, i) - getValueForPosition(oldSpeed, i);
            }
            oldSpeed = vf;
        }));
        
        speedChanged = {0};
        oldSpeed = {1};
        tempOut = {0};
    }
    
    void resetPhase() override{
        std::fill(offset.begin(), offset.end(), 0.0);
    }

private:
    
    template <typename T>
    auto getValueForPosition(const vector<T> &param, int index) -> T{
        if(param.size() == 1 || param.size() <= index){
            return param[0];
        }
        else{
            return param[index];
        }
    };
    
    ofParameter<vector<float>> input;
    ofParameter<vector<float>> speed;
    ofParameter<vector<float>> output;
    vector<float> tempOut;
    vector<float> offset;
    
    vector<float> speedChanged;
    vector<float> oldSpeed;

    ofEventListeners listeners;
};

#endif /* modTime_h */
