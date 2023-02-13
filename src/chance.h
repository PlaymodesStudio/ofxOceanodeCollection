//
//  chance.h
//  Oceanode
//
//  Created by Eduard Frigola Bagué on 27/10/2020.
//

#ifndef chance_h
#define chance_h

#include "ofxOceanodeNodeModel.h"
#include <random>

class chance : public ofxOceanodeNodeModel {
public:
    chance() : ofxOceanodeNodeModel("Chance"){};
    
    void setup(){
        addParameter(phaseIn.set("Phase", {0}, {0}, {1}));
        addParameter(seed.set("Seed", {0}, {(INT_MIN+1)/2}, {(INT_MAX-1)/2}));
        addParameter(probability.set("Prob", {0.5}, {0}, {1}));
        addParameter(retrigger.set("Retrig", false));
        addOutputParameter(output.set("Output", {0}, {0}, {1}));
        
        dist = std::uniform_real_distribution<float>(0.0, 1.0);
        oldPhasor.push_back(-1);
        highInNextFrame.push_back(false);
        mt.resize(1);
        setSeed();
        
        listeners.push(phaseIn.newListener([this](vector<float> &vf){
            vector<float> tempOut(output);
            if(oldPhasor.size() != vf.size()){
                //Resize everithing
                oldPhasor.resize(vf.size(), -1);
                highInNextFrame.resize(vf.size(), false);
                mt.resize(vf.size());
                tempOut.resize(vf.size());
                setSeed();
            }
            for(int i = 0; i < vf.size(); i++){
                if(highInNextFrame[i]){
                    tempOut[i] = 1;
                    highInNextFrame[i] = false;
                }
                if(vf[i] < oldPhasor[i]){ //Phasor has been reseted
                    if(dist(mt[i]) < (probability->size() == vf.size() ? probability->at(i) : probability->at(0))){
                        if(retrigger && tempOut[i] == 1){
                            tempOut[i] = 0;
                            highInNextFrame[i] = true;
                        }else{
                            tempOut[i] = 1;
                        }
                    }else{
                        tempOut[i] = 0;
                    }
                }
            }
            oldPhasor = vf;
            output = tempOut;
        }));
        
        listeners.push(seed.newListener([this](vector<int> &i){
            setSeed();
        }));
    }
	
	void resetPhase(){
		setSeed();
	}
    
private:
    
    void setSeed(){
        for(int i = 0; i < mt.size(); i++){
			if(seed->size() == mt.size()){
				mt[i].seed(seed->at(i));
			}
			else{
				if(seed->at(0) == 0){
					std::random_device rd;
					mt[i].seed(rd());
				}else{
					mt[i].seed(seed->at(0) + i);
				}
			}
        }
    }
    
    ofEventListeners listeners;
    
    ofParameter<vector<float>> phaseIn;
    ofParameter<vector<int>> seed;
    ofParameter<vector<float>> probability;
    ofParameter<bool> retrigger;
    ofParameter<vector<float>> output;
    
    vector<float> oldPhasor;
    vector<bool> highInNextFrame;
    
    vector<std::mt19937> mt;
    std::uniform_real_distribution<float> dist;
};

#endif /* chance_h */
