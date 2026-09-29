//
//  delta.cpp
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 14/07/2017.
//
//

#include "delta.h"

delta::delta() : ofxOceanodeNodeModel("Delta"){
    addParameter(gain.set("Gain", 1, 0, FLT_MAX));
    addParameter(invert.set("Invert", false));
    addParameter(input.set("Input", {0}, {0}, {1}));
    addOutputParameter(output.set("Output", {0}, {0}, {1}));
    addOutputParameter(outputPositive.set("Output +", {0}, {0}, {1}));
    addOutputParameter(outputNegative.set("Output -", {0}, {0}, {1}));
    
    color = ofColor::green;
}

void delta::update(ofEventArgs &args){
    // A number of vector nodes update their storage in place, which does not
    // necessarily emit an ofParameter change event. Delta is temporal by
    // definition, so sampling once per frame also gives it consistent
    // behaviour regardless of how the upstream node publishes its values.
    computeOutput(input.get());
}

void delta::computeOutput(const vector<float> &in){
    vector<float> tempOut(in.size(), 0.0f);
    vector<float> tempOutP(in.size(), 0.0f);
    vector<float> tempOutN(in.size(), 0.0f);

    if(inputStore.size() == in.size()){
        const float gainValue = gain.get();
        for(size_t i = 0; i < in.size(); i++){
            tempOutP[i] = ofClamp((in[i] - inputStore[i]) * gainValue, 0.0f, 1.0f);
            tempOutN[i] = ofClamp((inputStore[i] - in[i]) * gainValue, 0.0f, 1.0f);
            tempOut[i] = tempOutP[i] + tempOutN[i];
        }
    }

    // Always publish a vector matching the input size. The first sample (and
    // the first sample after a size change) has no previous value, so its
    // delta is correctly initialized to zero instead of leaving stale outputs.
    inputStore = in;
    output = tempOut;
    outputPositive = tempOutP;
    outputNegative = tempOutN;
}
