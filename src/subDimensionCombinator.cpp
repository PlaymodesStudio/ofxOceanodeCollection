//
//  subDimensionCombinator.cpp
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 25/10/2017.
//
//

#include "subDimensionCombinator.h"

subDimensionCombinator::subDimensionCombinator() : ofxOceanodeNodeModel("SubDimension Combinator"){
    addParameter(original.set("Original", {0}, {0}, {1}));
    addParameter(subDimension.set("Sub.Dim", {0}, {0}, {1}));
    
    addParameter(output.set("Output", {0}, {0}, {1}));
    
    listener = original.newListener(this, &subDimensionCombinator::paramListener);
}

void subDimensionCombinator::paramListener(vector<float> &vf){
    int originSize = original.get().size();
    int subDimSize = subDimension.get().size();
    if(originSize == 0 || subDimSize == 0) return;
    if(subDimSize % originSize == 0){
        vector<float> tempOut(subDimSize, 0);
        int dimStep = subDimSize / originSize;
        for(int i = 0 ; i < subDimSize; i++){
            int dimStep_ind = ceil(i / dimStep);
            tempOut[i] = original.get()[dimStep_ind] * subDimension.get()[i];
        }
        output = tempOut;
    }
    else{
        int size = subDimSize * originSize;
        vector<float> tempOut(size);
        for(int i = 0 ; i < size; i++){
            int dimStep_ind = ceil(i / subDimSize);
            tempOut[i] = original.get()[dimStep_ind] * subDimension.get()[i % subDimSize];
        }
        output = tempOut;
    }
    
}

