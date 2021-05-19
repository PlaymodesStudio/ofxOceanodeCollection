//
//  manualOscillatorBank.h
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 30/08/2017.
//
//

#ifndef manualOscillatorBank_h
#define manualOscillatorBank_h

#include "ofxOceanodeNodeModel.h"

class manualOscillatorBank: public ofxOceanodeNodeModel{
public:
    manualOscillatorBank() : ofxOceanodeNodeModel("Manual Oscillator Bank"){};
    ~manualOscillatorBank(){};
    
    void setup();
        
    void presetHasLoaded() override{
        output = {0};
    }
    
private:
    void computeValues(float &f);
    
    ofEventListener phasorInEvent;
    
    ofParameter<float>  manualInput;
    deque<float>        buffer;
    deque<pair<float, float>>    indexedBuffer;
    int bufferOverflow;
    float               bufferIndex;
    ofParameter<float>  phasorIn;
    ofParameter<vector<float>>  indexs;
    float               oldPhasor;
    ofParameter<float>  damping;
    ofParameter<float>  dampingPow;
    ofParameter<vector<float>>  output;
};

#endif /* manualOscillatorBank_h */
