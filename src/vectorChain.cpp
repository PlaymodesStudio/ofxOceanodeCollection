//
//  vectorChain.cpp
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 06/09/2017.
//
//

#include "vectorChain.h"

vectorChain::vectorChain() : ofxOceanodeNodeModel("Vector Chain"){
    inputs.resize(2);
    for(int i = 0; i < inputs.size() ; i++){
        addParameter(inputs[i].set("Input " + ofToString(i), {0}, {0}, {1}));
    }
    addParameter(offset.set("Offset", 0, -1, 1));
    addParameter(output.set("Output", {0}, {0}, {1}));
    
    
    listener = inputs[0].newListener(this, &vectorChain::inputListener);
}

void vectorChain::inputListener(vector<float> &v){
    vector<float>   outChain;
    for(auto in : inputs)
        outChain.insert(outChain.end(), in.get().begin(), in.get().end());
    for(int i = 0; i < round(outChain.size()*(abs(offset))); i++){
        if(offset < 0){
            float firstValue = outChain[0];
            for(int j = 1 ; j < outChain.size() ; j++){
                outChain[j - 1] = outChain[j];
            }
            outChain.back() = firstValue;
        }
        else if(offset > 0){
            float lastValue = outChain.back();
            for(int j = outChain.size()-1 ; j > 0 ; j--){
                outChain[j] = outChain[j - 1];
            }
            outChain[0] = lastValue;
        }
    }
    output = outChain;
}

