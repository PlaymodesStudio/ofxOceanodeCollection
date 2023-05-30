#ifndef sequentialAnalyzer_h
#define sequentialAnalyzer_h

#include "ofxOceanodeNodeModel.h"

class sequentialAnalyzer : public ofxOceanodeNodeModel{
public:
    sequentialAnalyzer() : ofxOceanodeNodeModel("Sequential Analyzer") {};
    ~sequentialAnalyzer() {};

    void setup() override {
        addParameter(input.set("Input", {0}, {0}, {1}));
        addParameter(threshold.set("Threshold", {0.5}, {0}, {1}));
        addParameter(outputSize.set("Out.Size", 1, 1, 1000));
        addParameter(step.set("Step", 1, 1, 12));
        addParameter(resetCount.set("Reset", false));  // Add resetCount parameter
        addOutputParameter(floatOutput.set("Output", 0, 0, 1));
        addOutputParameter(pulseOutput.set("Pulse", {0}, {0}, {1}));
        addOutputParameter(countOutput.set("Count", 0, 0, INT_MAX));

        currentPosition = 0;
        fallingEdgeCount = 0;

//        listeners.push(outputSize.newListener([this](int &i){
//            currentPosition = 0;
//            floatOutput = 0;
//            vector<float> tempOut(i * step, 0);
//            tempOut[0] = 1;
//            pulseOutput = tempOut;
//        }));
//
        listeners.push(resetCount.newListener([this](bool &b){
            if(b) {
                fallingEdgeCount = -1;
                resetCount = false;
            }
        }));
//
//        listeners.push(step.newListener([this](int &i){
//            currentPosition = 0;
//            floatOutput = 0;
//            vector<float> tempOut(outputSize * i, 0);
//            tempOut[0] = 1;
//            pulseOutput = tempOut;
//        }));

        listeners.push(input.newListener([this](vector<float> &vf){
            bool inputTriggered = false;
            if(vf.size() != lastInput.size()) lastInput = vector<float>(vf.size(), 0);

            if(resetCount.get()){   // Check if resetCount is true
                fallingEdgeCount = 0;
                resetCount = false;
            }

            for(int i = 0; i < vf.size(); i++){
                if(vf[i] >= threshold && lastInput[i] < threshold){
                    inputTriggered = true;
                }
                if(vf[i] < threshold && lastInput[i] >= threshold){
                    fallingEdgeCount++;
                    countOutput = fallingEdgeCount;
                }
            }
            lastInput = vf;

            if(inputTriggered){
                currentPosition += step;
                currentPosition %= (outputSize * step);
                floatOutput = (float)currentPosition/(float)(outputSize * step);
                vector<float> tempPulseOutput(outputSize * step, 0);
                tempPulseOutput[currentPosition] = 1;
                pulseOutput = tempPulseOutput;
            }
        }));
    }

    void resetPhase() override {
        currentPosition = 0;
        floatOutput = (float)currentPosition/(float)(outputSize * step);
        vector<float> tempPulseOutput(outputSize * step, 0);
        tempPulseOutput[currentPosition] = 1;
        pulseOutput = tempPulseOutput;
    }

private:
    ofParameter<vector<float>> input;
    ofParameter<float> threshold;
    ofParameter<int> outputSize;
    ofParameter<int> step;
    ofParameter<bool> resetCount;  // resetCount parameter declaration
    ofParameter<float> floatOutput;
    ofParameter<vector<float>> pulseOutput;
    ofParameter<int> countOutput;

    int currentPosition;
    vector<float> lastInput;
    int fallingEdgeCount;

    ofEventListeners listeners;
};

#endif /* sequentialAnalyzer_h */

