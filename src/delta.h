//
//  delta.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 14/07/2017.
//
//

#ifndef delta_h
#define delta_h

#include "ofxOceanodeNodeModel.h"

class delta : public ofxOceanodeNodeModel{
public:
    delta();
    ~delta(){};
    void update(ofEventArgs &args) override;
    
private:
    void computeOutput(const vector<float> &in);
    
    ofParameter<float>  gain;
    ofParameter<bool>   invert;
    ofParameter<vector<float>>   input;
    ofParameter<vector<float>>   output;
    ofParameter<vector<float>>   outputPositive;
    ofParameter<vector<float>>   outputNegative;
    
    vector<float>   inputStore;
};

#endif /* delta_h */
