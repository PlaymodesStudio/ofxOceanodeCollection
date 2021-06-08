//
//  stepSequencer.h
//  PLedNodes
//
//  Created by Eduard Frigola Bagué on 07/08/2019.
//

#ifndef stepSequencer_h
#define stepSequencer_h

class stepSequencer : public ofxOceanodeNodeModel{
public:
    stepSequencer() : ofxOceanodeNodeModel("Step Sequencer"){};
    ~stepSequencer(){};
    
    void setup(){
        addParameter(index.set("Index", 0, 0, 1));
        addParameter(stepsVec.set("Steps[]", {0}, {0}, {1}));
        addParameter(output.set("Output", {0}, {0}, {1}));
        
		stepsVecListener = stepsVec.newListener([this](vector<float> &steps){
			index.setMax(steps.size()-1);
			stepsVec.setMax(vector<float>(1, *std::max_element(steps.begin(), steps.end())));
			output.setMax(stepsVec.getMax());
		});
		
        indexListener = index.newListener([this](int &ind){
            ind %= stepsVec->size();
            vector<float> tempOut(stepsVec->size(), 0);
            tempOut[ind] = stepsVec.get()[ind];
            output = tempOut;
        });
        
        index = 0;
        
    }
    
    void presetHasLoaded(){
//        vector<float> tempOut(numSteps, 0);
//        for(int i = 0 ; i < numSteps; i++){
//            tempOut[i] = steps[i];
//        }
//        stepsVec = tempOut;
//        tempOut = vector<float>(numSteps, 0);
//        tempOut[index] = steps[index];
//        output = tempOut;
    }
    
private:
    ofParameter<int> index;
    ofParameter<vector<float>> output;
    ofParameter<vector<float>> stepsVec;
    
	ofEventListener stepsVecListener;
    ofEventListener indexListener;
};

#endif /* stepSequencer_h */
