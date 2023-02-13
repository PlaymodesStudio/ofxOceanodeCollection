//
//  vectorRotator.h
//  ofxOceanodeCollection
//
//  Created by Eduard Frigola on 05/01/2023.
//
//

#ifndef vectorRotator_h
#define vectorRotator_h

#include "ofxOceanodeNodeModel.h"

class vectorRotator : public ofxOceanodeNodeModel{
public:
    vectorRotator() : ofxOceanodeNodeModel("Vector Rotator"){}
    
    void setup(){
        addParameter(input.set("Input", {0}, {-FLT_MAX}, {FLT_MAX}));
        addParameter(rotation.set("Rotation", 0, 0, 0));
        addOutputParameter(output.set("Output", {0}, {-FLT_MAX}, {FLT_MAX}));

        listener = input.newListener([this](vector<float> &vf){
            if(size != vf.size()){
                rotation.setMin(-(vf.size()/2.0f));
                rotation.setMax(vf.size()/2.0f);
                rotation = 0;
                size = vf.size();
            }
            
            vector<float> tempOutput(size);
            for (int i = 0; i < size ; i++){
                float index = i - rotation;
                int indexL = floor(index) - size * floor((floor(index)) / size);
                int indexH = ceil(index) - size * floor((ceil(index)) / size);
                float lowVal = vf[indexL];
                float highVal = vf[indexH];
                tempOutput[i] = lowVal + (highVal-lowVal) * (index - floor(index));
            }
            output = tempOutput;
        });
        
        size = 1;
    }
    
private:
    
    ofEventListener listener;
    
    ofParameter<vector<float>>  input;
    ofParameter<float> rotation;
    ofParameter<vector<float>>  output;
    
    int size;
    
};

#endif /* vectorRotator_h */
