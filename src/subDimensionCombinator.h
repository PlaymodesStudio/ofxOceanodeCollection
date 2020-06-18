//
//  subDimensionCombinator.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 25/10/2017.
//
//

#ifndef subDimensionCombinator_h
#define subDimensionCombinator_h

#include "ofxOceanodeNodeModel.h"

class subDimensionCombinator : public ofxOceanodeNodeModel{
public:
    subDimensionCombinator();
    ~subDimensionCombinator(){};
    
private:
    void paramListener(vector<float> &vf);
    
    ofEventListener listener;
    ofParameter<vector<float>>  original;
    ofParameter<vector<float>>  subDimension;
    
    ofParameter<vector<float>>  output;
};


#endif /* subDimensionCombinator_h */

