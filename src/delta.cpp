//
//  delta.cpp
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 14/07/2017.
//
//

#include "delta.h"

delta::delta() : ofxOceanodeNodeModel("Delta"){
    addParameter(gain.set("Gain", 1, 0, 100));
    addParameter(invert.set("Invert", false));
    addParameter(input.set("Input", {0}, {0}, {1}));
    addParameter(output.set("Output", {0}, {0}, {1}));
    addParameter(outputPositive.set("Output +", {0}, {0}, {1}));
    addParameter(outputNegative.set("Output -", {0}, {0}, {1}));
    
    listener = input.newListener(this, &delta::computeOutput);
    color = ofColor::green;
}

void delta::computeOutput(vector<float> &in){
    if(inputStore.size() != in.size()){
        inputStore = in;
    }
    else{
        vector<float> tempOut(in.size());
        vector<float> tempOutP(in.size());
        vector<float> tempOutN((in.size()));
        for(int i = 0; i < in.size(); i++){
            tempOutP[i] = ofClamp((in[i] - inputStore[i])*gain, 0, 1);
            tempOutN[i] = ofClamp((inputStore[i] - in[i])*gain, 0, 1);
            tempOut[i] = tempOutP[i] + tempOutN[i];
        }
        inputStore = in;
        output = tempOut;
        outputPositive = tempOutP;
        outputNegative = tempOutN;
    }
    
}
