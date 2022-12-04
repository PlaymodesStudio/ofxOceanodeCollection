//
//  vectorOperations.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 19/09/2017.
//
//

#ifndef vectorOperations_h
#define vectorOperations_h

#include "ofxOceanodeNodeModel.h"

enum operations{
    MIN_OP = 1,
    MAX_OP = 2,
    SUM_OP = 3,
    MEAN_OP = 4,
    DIFF_OP = 5,
    DEV_OP = 6,
    SIZE_OP = 7
};

class vectorOperations : public ofxOceanodeNodeModel{
public:
    vectorOperations() : ofxOceanodeNodeModel("Vector Operations"){
        addParameter(input.set("Input", {0}, {-FLT_MAX}, {FLT_MAX}));
        addParameterDropdown(operationSelector, "Op.", 0, {"Min", "Max", "Sum", "Mean", "Diff", "Dev", "Size"});
        addParameter(output.set("Output", 0, -FLT_MAX, FLT_MAX));

        listener = input.newListener(this, &vectorOperations::inputListener);
    }
    
private:
    void inputListener(vector<float> &vf){
        
        switch (static_cast<operations>(operationSelector+1)) {
            case MIN_OP:
            {
                float minVal = 1;
                for(auto f : vf){
                    if(f < minVal) minVal = f;
                }
                output = minVal;
                break;
            }
            case MAX_OP:
            {
                float maxVal = 0;
                for(auto f : vf){
                    if(f > maxVal) maxVal = f;
                }
                output = maxVal;
                break;
            }
            case SUM_OP:
            {
                float sum = 0;
                for(auto f : vf){
                    sum += f;
                }
                sum = ofClamp(sum, 0, 1);
                output = sum;
                break;
            }
            case MEAN_OP:
            {
                float sum = 0;
                for(auto f : vf){
                    sum += f;
                }
                output = sum / (vf.size());
                break;
            }
            case DIFF_OP:
            {
                output = 0;
                break;
            }
            case DEV_OP:
            {
                float mean = 0;
                for(auto f : vf){
                    mean += f;
                }
                mean /= vf.size();
                float dev = 0;
                for(auto f : vf){
                    dev += abs(f-mean);
                }
                dev /= vf.size();
                output = dev;
                break;
            }
            case SIZE_OP:
                output = vf.size();
                break;
            default:
                break;
        }
    }
    
    ofEventListener listener;
    
    ofParameter<vector<float>>  input;
    ofParameter<int>    operationSelector;
    ofParameter<float>  output;
};

#endif /* vectorOperations_h */
