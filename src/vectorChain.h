// vectorChain.h
// MIRABCN_Generator
//
// Created by Eduard Frigola on 06/09/2017.
//

#ifndef vectorChain_h
#define vectorChain_h

#include "ofxOceanodeNodeModel.h"

class vectorChain : public ofxOceanodeNodeModel{
public:
    vectorChain() : ofxOceanodeNodeModel("Vector Chain"){};
    ~vectorChain(){};
    
    void setup();
    
    void loadBeforeConnections(ofJson &json){
        deserializeParameter(json, numInputs);
    }
    
private:
    void inputListener(vector<float> &v);
    void updateListeners();
    
    ofEventListeners listeners;
    vector<ofParameter<vector<float>>>  inputs;
    vector<std::unique_ptr<ofEventListener>> inputListeners;
    ofParameter<float>  offset;
    ofParameter<vector<float>>  output;
    ofParameter<bool> interleave;
    ofParameter<int> numInputs;
    ofParameter<bool> listenAll;

};

#endif /* vectorChain_h */
