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
#include <vector>

class vectorGetter : public ofxOceanodeNodeModel{
public:
    vectorGetter() : ofxOceanodeNodeModel("Vector Getter"){
        addParameter(input.set("Input", {0, 0}, {-FLT_MAX}, {FLT_MAX}));
        addParameter(index.set("Index", {0}, {0}, {1}));
        addOutputParameter(output.set("Output", {0}, {-FLT_MAX}, {FLT_MAX}));
        
        listener = input.newListener([this](vector<float> &v){
            inputListener(v);
        });
        
        indexListener = index.newListener([this](vector<int> &idx){
            indexListenerFunc(idx);
        });
    };

    ~vectorGetter(){};

private:
    void inputListener(vector<float> &v){
        std::vector<float> result;
        auto idx = index.get();
        for(const auto& i : idx){
            if(i < v.size()){
                result.push_back(v[i]);
            }
        }
        output = result;
    }

    void indexListenerFunc(vector<int> &idx){
        std::vector<float> result;
        auto inputValue = input.get();
        for(const auto& i : idx){
            if(i < inputValue.size()){
                result.push_back(inputValue[i]);
            }
        }
        output = result;
    }

    ofEventListener listener;
    ofEventListener indexListener;

    ofParameter<vector<float>>  input;
    ofParameter<vector<int>>    index;
    ofParameter<vector<float>>  output;
};

#endif /* vectorGetter_h */


