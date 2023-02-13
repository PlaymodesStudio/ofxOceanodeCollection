//
//  vectorSelector.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 19/09/2017.
//
//

#ifndef vectorSelector_h
#define vectorSelector_h

#include "ofxOceanodeNodeModel.h"

class vectorSelector : public ofxOceanodeNodeModel{
public:
    vectorSelector() : ofxOceanodeNodeModel("Vector Selector"){
        addParameter(input.set("Input", {0}, {-FLT_MAX}, {FLT_MAX}));
        addParameter(selection.set("Selection", {0}, {0}, {1}));
        addParameter(preservePosition.set("Preserve", 0, 0, INT_MAX));
        addParameter(outputIndexs.set("Indexs", {0}, {0}, {INT_MAX}));
        addOutputParameter(output.set("Output", {0}, {-FLT_MAX}, {FLT_MAX}));

        listener = input.newListener(this, &vectorSelector::inputListener);
        listener2 = preservePosition.newListener([this](int &i){
            if(preservePosition != 0){
                preservedPositions.resize(preservePosition, -1);
                usedPositions.resize(preservePosition);
                std::iota(usedPositions.begin(), usedPositions.end(), 0);
            }
        });
        
    }
    
private:
    void inputListener(vector<float> &vf){
        if(input->size() > 1){
            vector<float> tempOut;
            vector<int> tempIdxs;
            vector<float> sel = input.get();
            if(input->size() == input->size()) sel = selection.get();
            
            
            if(preservePosition != 0){
                tempOut.resize(preservePosition);
                tempIdxs.resize(preservePosition);
            }
            for(int i = 0; i < input->size(); i++){
                if(sel[i] > 0){
                    if(preservePosition != 0){
                        auto foundVal = std::find(preservedPositions.begin(), preservedPositions.end(), i);
                        if(foundVal != preservedPositions.end()){
                            int index = foundVal - preservedPositions.begin();
                            tempOut[index] = input->at(i);
                            tempIdxs[index] = i;
                        }else if(std::find(removedPositions.begin(), removedPositions.end(), i) == removedPositions.end()) {
                            int index = usedPositions[0];
                            tempOut[index] = input->at(i);
                            tempIdxs[index] = i;
                            preservedPositions[index] = i;
                            usedPositions.push_back(index);
                            if(usedPositions.size() > preservePosition){
                                removedPositions.push_back(index);
                                usedPositions.pop_front();
                            }
                            
                        }
                    }else{
                        tempOut.push_back(input->at(i));
                        tempIdxs.push_back(i);
                    }
                }else{
                    std::remove(removedPositions.begin(), removedPositions.end(), i);
//                    if(preservePosition != 0){
//                        auto foundVal = std::find(preservedPositions.begin(), preservedPositions.end(), i);
//                        if(foundVal != preservedPositions.end()){
//                            *foundVal = -1;
//                        }
//                    }
                }
            }
            
            
            if(tempOut.size() > 0){
                outputIndexs = tempIdxs;
                output = tempOut;
            }else{
                outputIndexs = {0};
                output = {0};
            }
        }
    }
    
    ofEventListener listener;
    ofEventListener listener2;
    
    ofParameter<vector<float>>  input;
    ofParameter<vector<float>>    selection;
    ofParameter<int> preservePosition;
    ofParameter<vector<float>>  output;
    ofParameter<vector<int>>  outputIndexs;
    
    vector<int> preservedPositions;
    deque<int> usedPositions;
    vector<int> removedPositions;
    
};

#endif /* vectorSelector_h */
