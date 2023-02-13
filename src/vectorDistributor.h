//
//  vectorDistributor.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 19/09/2017.
//
//

#ifndef vectorDistributor_h
#define vectorDistributor_h

#include "ofxOceanodeNodeModel.h"

class vectorDistributor : public ofxOceanodeNodeModel{
public:
    vectorDistributor() : ofxOceanodeNodeModel("Vector Distributor"){
        addParameter(input.set("Input", {0}, {-FLT_MAX}, {FLT_MAX}));
        addParameter(indexs.set("Indexs", {0}, {0}, {INT_MAX}));
        addParameter(size.set("Size", 1, 1, INT_MAX));
        addOutputParameter(output.set("Output", {0}, {-FLT_MAX}, {FLT_MAX}));

        listener = input.newListener(this, &vectorDistributor::inputListener);
    }
    
private:
    void inputListener(vector<float> &vf){
        if(input->size() == indexs->size()){
            vector<float> tempOut(size);
            for(int i = 0; i < indexs->size(); i++){
                int index = indexs->at(i);
                if(index < size){
                    tempOut[index] = input->at(i);
                }
            }
            output = tempOut;
        }
    }
    
    ofEventListener listener;
    
    ofParameter<vector<float>>  input;
    ofParameter<vector<int>>    indexs;
    ofParameter<int>  size;
    ofParameter<vector<float>>  output;
};

#endif /* vectorDistributor_h */
