//
//  vectorReorder.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 19/09/2017.
//
//

#ifndef vectorReorder_h
#define vectorReorder_h

#include "ofxOceanodeNodeModel.h"

enum func{
    SORT_OP = 1,
    INVERT_OP = 2
};

class vectorReorder : public ofxOceanodeNodeModel{
public:
    vectorReorder() : ofxOceanodeNodeModel("Vector Reorder"){
        addParameter(input.set("Input", {0}, {-FLT_MAX}, {FLT_MAX}));
        addParameterDropdown(funcSelector, "Op.", 0, {"Sort", "Invert"});
        addOutputParameter(output.set("Output", {0}, {-FLT_MAX}, {FLT_MAX}));

        listener = input.newListener(this, &vectorReorder::inputListener);
    }
    
private:
    void inputListener(vector<float> &vf){
        
        switch (static_cast<func>(funcSelector+1)) {
            case SORT_OP:
            {
                vector<float> cpy(input.get());
                std::sort(cpy.begin(), cpy.end());
                output = cpy;
                break;
            }
            case INVERT_OP:
            {
                vector<float> cpy(input.get());
                std::reverse(cpy.begin(), cpy.end());
                output = cpy;
                break;
            }
            default:
                break;
        }
    }
    
    ofEventListener listener;
    
    ofParameter<vector<float>>  input;
    ofParameter<int>    funcSelector;
    ofParameter<vector<float>>  output;
};

#endif /* vectorReorder_h */
