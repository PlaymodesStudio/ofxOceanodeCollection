//
//  thresholdBang.h
//  PLedNodes
//
//  Created by Eduard Frigola Bagué on 07/08/2019.
//

#ifndef thresholdBang_h
#define thresholdBang_h

#include "ofxOceanodeNodeModel.h"

class threshold : public ofxOceanodeNodeModel{
public:
    threshold() : ofxOceanodeNodeModel("Threshold"){};
    
    void setup(){
        addParameter(input.set("Input", {0}, {0}, {1}));
        addParameter(thresholdVal.set("Thrs.", {1}, {0}, {1}));
        addParameter(output.set("Change", {0}, {0}, {1}));
        addParameter(toggleOut.set("State", {0}, {0}, {1}));
		
		addInspectorParameter(frameMode.set("Frame", false));
		addInspectorParameter(phasorMode.set("Phasor Mode", false));
        
        listener = input.newListener([this](vector<float> &vf){
            if(vf.size() != lastVal.size()){
                lastVal = vf;
            }
            vector<float> tempOutput(input->size(), 0);
            vector<float> tempToggleOutput(input->size(), 0);
            for(int i = 0; i < input->size(); i++){
				if(phasorMode){
					if(vf[i] < 0.5 && lastVal[i] > 0.5){
						tempToggleOutput[i] = 1;
						tempOutput[i] = 1;
					}
				}else{
					float indexThreshold;
					if(thresholdVal->size() == vf.size()){
						indexThreshold = thresholdVal.get()[i];
					}else{
						indexThreshold = thresholdVal.get()[0];
					}
					if(vf[i] >= indexThreshold){
						tempToggleOutput[i] = 1;
						if(lastVal[i] < indexThreshold){
							tempOutput[i] = 1;
						}
					}
				}
            }
            lastVal = vf;
            output = tempOutput;
            toggleOut = tempToggleOutput;
            if(!frameMode){
                tempOutput = vector<float>(tempOutput.size(), 0);
                output = tempOutput;
            }
        });
    }
    
private:
    ofParameter<vector<float>> input;
    ofParameter<vector<float>> thresholdVal;
    ofParameter<bool>   frameMode;
	ofParameter<bool>	phasorMode;
    ofParameter<vector<float>> output;
    ofParameter<vector<float>> toggleOut;
    
    vector<float> lastVal;
    
    ofEventListener listener;
};

#endif /* thresholdBang_h */
