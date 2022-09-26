//
//  vectorChain.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 06/09/2017.
//
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
    
    ofEventListeners listeners;
    vector<ofParameter<vector<float>>>  inputs;
    ofParameter<float>  offset;
    ofParameter<vector<float>>  output;
    ofParameter<bool> interleave;
    ofParameter<int> numInputs;
};

#endif /* vectorChain_h */

