#ifndef dummySlidersDynamic_h
#define dummySlidersDynamic_h

#pragma once

#include "ofxOceanodeNodeModel.h"


class dummySlidersDynamic : public ofxOceanodeNodeModel{
public:
	dummySlidersDynamic() : ofxOceanodeNodeModel("Dummy Sliders Dynamic"){};
    ~dummySlidersDynamic(){};
    void setup() override;
	
	//void update(ofEventArgs &a) override;
    
    void loadBeforeConnections(ofJson &json) override{
        deserializeParameter(json, numSliders);
    }

private:
    
    ofEventListeners listeners;
	
	ofParameter<int> numSliders;
	ofParameter<bool> normalizedRange;
    vector<ofParameter<float>>  sliders;
	ofParameter<vector<float>> output;
};


#endif /* switcher_h */
