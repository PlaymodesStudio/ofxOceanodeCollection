//
//  slotEaser.h
//  Genesis
//
//  Created by Eduard Frigola Bagué on 17/5/22.
//

#ifndef slotEaser_h
#define slotEaser_h

#include "ofxOceanodeNodeModel.h"

class slotEaser : public ofxOceanodeNodeModel{
public:
    slotEaser() : ofxOceanodeNodeModel("Slot Easer"){
        addParameter(phasor.set("Phase", 0, 0, 1));
        addParameter(input.set("Input", 0, 0, INT_MAX));
        addParameter(pow.set("Pow", {0}, {-1}, {1}));
        addParameter(bipow.set("BiPow", {0}, {-1}, {1}));
        addOutputParameter(output.set("Output", {0}, {-FLT_MAX}, {FLT_MAX}));
        addInspectorParameter(numInputs.set("Num Inputs", 3, 2, INT_MAX));
        
        slots.resize(numInputs);
        
        input.setMax(numInputs-1);
        
        for(int i = 0; i < numInputs; i++){
            addParameter(slots[i].set("Slot " + ofToString(i), {0}, {-FLT_MAX}, {FLT_MAX}));
        }
        
        color = ofColor::green;
        lastSize = 1;
        lastOutput = slots[0];
        
        listeners.push(numInputs.newListener([this](int &i){
            if(slots.size() != i){
                int oldSize = slots.size();
                bool remove = oldSize > i;
                
                slots.resize(i);
                
                if(remove){
                    for(int j = oldSize-1; j >= i; j--){
                        removeParameter("Slot " + ofToString(j));
                    }
                }else{
                    for(int j = oldSize; j < i; j++){
                        addParameter(slots[j].set("Slot " + ofToString(j), {0}, {-FLT_MAX}, {FLT_MAX}));
                    }
                }
                input.setMax(numInputs-1);
            }
        }));
        
        listeners.push(phasor.newListener([this](float &vf){
            int maxSize = 0;
            for(int i = 1; i < numInputs; i++){
                if(slots[i]->size() > maxSize){
                    maxSize = slots[i]->size();
                }
            }
            if(maxSize != lastSize){
                vector<float> tempOutput = vector<float>(slots[input]->size());
                for(int i = 0; i < slots[input].get().size(); i++){
                    tempOutput[i] = slots[input]->at(i);
                }
                lastInput = input;
                lastChangedInput = input;
                phasorValueOnValueChange = vf;
                lastPhase = 0;
                reachedMax = false;
                lastSize = maxSize;
                lastOutput = tempOutput;
            }else{
                int inputSize = maxSize;
                if(lastInput != input){
                    phasorValueOnValueChange = vf;
                    reachedMax = false;
                    lastPhase = 0;
                    lastOutput = output;
                    lastChangedInput = lastInput;
                }
                vector<float> tempOutput = vector<float>(maxSize);
                if(maxSize == slots[input]->size() && maxSize == slots[lastInput]->size() && maxSize == lastOutput.size()){
                    for(int i = 0; i < inputSize; i++){
                        float phase = vf - phasorValueOnValueChange;
                        if(phase < 0) phase = 1+phase;
                        if(getValueForPosition(pow.get(), i) != 0){
                            customPow(phase, getValueForPosition(pow.get(), i));
                        }
                        if(getValueForPosition(bipow.get(), i) != 0){
                            phase = (phase*2) - 1;
                            customPow(phase, getValueForPosition(bipow.get(), i));
                            phase = (phase + 1) / 2.0;
                        }
                        if(phase < lastPhase) reachedMax = true;
                        else lastPhase = phase;
                        if(!reachedMax){
                            tempOutput[i] = smoothinterpolate(lastOutput.at(i), slots[input]->at(i), phase);
                        }else{
                            tempOutput[i] = slots[input]->at(i);
                        }
                    }
                    output = tempOutput;
                }else{
                    lastOutput = slots[input];
                    output = lastOutput;
                }
                lastInput = input;
            }
        }));
    }
    
    ~slotEaser(){};
    
    void loadBeforeConnections(ofJson &json){
        deserializeParameter(json, numInputs);
    }
    
private:
    void customPow(float & value, float pow){
        float k1 = 2*pow*0.99999;
        float k2 = (k1/((-pow*0.999999)+1));
        float k3 = k2 * abs(value) + 1;
        value = value * (k2+1) / k3;
    }
        
    float smoothinterpolate(float start, float end, float pos){
        float oldRandom = start;
        float pastRandom = start;
        float newRandom = end;
        float futureRandom  = end;
        float L0 = (newRandom - pastRandom) * 0.5;
        float L1 = L0 + (oldRandom-newRandom);
        float L2 = L1 + ((futureRandom - oldRandom)*0.5) + (oldRandom - newRandom);
        return oldRandom + (pos * (L0 + (pos * ((pos * L2) - (L1 + L2)))));
        
    }
    
    template<typename T>
    auto getValueForPosition(const vector<T> &param, int index) -> T{
        if(param.size() == 1 || param.size() <= index){
            return param[0];
        }
        else{
            return param[index];
        }
    }
    
    ofParameter<int> input;
    ofParameter<float> phasor;
    ofParameter<vector<float>> pow;
    ofParameter<vector<float>> bipow;
    ofParameter<vector<float>> output;
    ofParameter<int> numInputs;
    
    vector<ofParameter<vector<float>>> slots;
    
    ofEventListeners listeners;
    int lastInput;
    float  lastChangedInput;
    float phasorValueOnValueChange;
    float lastPhase;
    bool reachedMax;
    int lastSize;
    vector<float> lastOutput;
};

#endif /* slotEaser_h */
