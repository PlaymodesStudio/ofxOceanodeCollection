// vectorChain.cpp
// MIRABCN_Generator
//
// Created by Eduard Frigola on 06/09/2017.
//

#include "vectorChain.h"

void vectorChain::setup(){
    
    addInspectorParameter(numInputs.set("Inputs", 2, 2, INT_MAX));
    addInspectorParameter(interleave.set("Interleave", false));
    addInspectorParameter(listenAll.set("Listen All", false));
    addParameter(offset.set("Offset", 0, -1, 1));
    addOutputParameter(output.set("Output", {0}, {-FLT_MAX}, {FLT_MAX}));
    
    inputs.resize(2);
    inputListeners.resize(2);
    for(int i = 0; i < inputs.size() ; i++){
        addParameter(inputs[i].set("Input " + ofToString(i), {0}, {-FLT_MAX}, {FLT_MAX}));
    }
    
    listeners.push(numInputs.newListener([this](int &i){
        if(inputs.size() != i){
            int oldSize = inputs.size();
            inputs.resize(i);
            inputListeners.resize(i);
            
            if(oldSize > i){
                for(int j = oldSize-1; j >= i; j--){
                    removeParameter("Input " + ofToString(j));
                }
            }else{
                for(int j = oldSize; j < i; j++){
                    addParameter(inputs[j].set("Input " + ofToString(j), {0}, {-FLT_MAX}, {FLT_MAX}));
                }
            }

            updateListeners();
        }
    }));

    listeners.push(listenAll.newListener([this](bool &b){
        updateListeners();
    }));

    updateListeners();
}

void vectorChain::updateListeners() {
    for(int i = 0; i < inputs.size(); i++){
        if(listenAll || i == 0){
            if(!inputListeners[i]){
                inputListeners[i] = std::make_unique<ofEventListener>(inputs[i].newListener([this](vector<float> &v){
                    inputListener(v);
                }));
            }
        }else{
            if(inputListeners[i]){
                inputListeners[i].reset();
            }
        }
    }
}

void vectorChain::inputListener(vector<float> &v){
    vector<float>   outChain;
    if(interleave){
        int accumSize = 0;
               int maxSize = 0;
               for(auto in : inputs){
                   accumSize += in.get().size();
                   if(in.get().size() > maxSize) maxSize = in.get().size();
               }
               outChain.resize(accumSize);
               int index = 0;
               for(int i = 0 ; i < maxSize ; i++){
                   for(int j = 0; j < inputs.size(); j++){
                       if(i < inputs[j]->size()){
                           outChain[index] = inputs[j]->at(i);
                           index++;
                       }
                   }
               }
    }else{
        for(auto in : inputs){
            outChain.insert(outChain.end(), in.get().begin(), in.get().end());
        }
        //        for(int i = 0; i < round(outChain.size()*(abs(offset))); i++){
        //            if(offset < 0){
        //                float firstValue = outChain[0];
        //                for(int j = 1 ; j < outChain.size() ; j++){
        //                    outChain[j - 1] = outChain[j];
        //                }
        //                outChain.back() = firstValue;
        //            }
        //            else if(offset > 0){
        //                float lastValue = outChain.back();
        //                for(int j = outChain.size()-1 ; j > 0 ; j--){
        //                    outChain[j] = outChain[j - 1];
        //                }
        //                outChain[0] = lastValue;
        //            }
        //        }

    }
    output = outChain;
}
