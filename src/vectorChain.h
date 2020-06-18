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
    vectorChain();
    ~vectorChain(){};
    
private:
    void inputListener(vector<float> &v);
    
    ofEventListener listener;
    vector<ofParameter<vector<float>>>  inputs;
    ofParameter<float>  offset;
    ofParameter<vector<float>>  output;
};

#endif /* vectorChain_h */

