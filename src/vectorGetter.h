//
//  vectorGetter.hpp
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 06/09/2017.
//
//

#ifndef vectorGetter_h
#define vectorGetter_h

#include "ofxOceanodeNodeModel.h"

class vectorGetter : public ofxOceanodeNodeModel{
public:
    vectorGetter() : ofxOceanodeNodeModel("Vector Getter"){
        addParameter(input.set("Input", {0, 0}, {-FLT_MAX}, {FLT_MAX}));
        addParameter(index.set("Index", 0, 0, 1));
        addOutputParameter(output.set("Output", 0, -FLT_MAX, FLT_MAX));
        
        lastSize = 1;
        wishIndex = -1;
        
        listener = input.newListener(this, &vectorGetter::inputListener);
    };
    
    void presetRecallAfterSettingParameters(ofJson &json) override{
        if(json.count("Index") == 1){
            wishIndex = ofToInt(json["Index"].get<string>());
        }
    }
    
    ~vectorGetter(){};
    
private:
    void inputListener(vector<float> &v){
        if(v.size() > 1){
            if(v.size() != lastSize){
                if(index > v.size()-1) index.set(v.size()-1);
                index.setMax(v.size() - 1);
                string indexName = "Index";
                parameterChangedMinMax.notify(this, indexName);
                if(wishIndex != -1 && wishIndex < v.size()){
                    index = wishIndex;
                }
                lastSize = v.size();
            }
            if(v.size() > index){
                output = v[index];
            }
        }
    }
    
    ofEventListener listener;
    
    ofParameter<vector<float>>  input;
    ofParameter<int> index;
    ofParameter<float>  output;
    int lastSize;
    int wishIndex;
};

#endif /* vectorGetter_h */

