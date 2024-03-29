//
//  manualOscillatorBank.cpp
//  MIRABCN_Generator
//
//  Created by Eduard Frigola on 30/08/2017.
//
//

#include "manualOscillatorBank.h"

void manualOscillatorBank::setup(){
    
    addParameter(phasorIn.set("Phase", 0, 0, 1));
    addParameter(indexs.set("Index", {0}, {0}, {1}));
    addParameter(manualInput.set("Input", 0, 0, 1));
    addParameter(damping.set("Damp.", 0, 0, 1));
    addParameter(dampingPow.set("Damp.Pow", 0, -40, 40));
    addOutputParameter(output.set("Output", {0}, {0}, {1}));
    
    phasorInEvent = phasorIn.newListener(this, &manualOscillatorBank::computeValues);
    
    bufferIndex = 0;
    oldPhasor = 0;
    bufferOverflow = 0;
    
    color = ofColor(0, 128, 255);
}


void manualOscillatorBank::computeValues(float &f){
    if(oldPhasor > f) bufferOverflow++;
    oldPhasor = f;
    
    indexedBuffer.push_front(std::make_pair(f+bufferOverflow, manualInput));
    
    vector<float>   tempOut;
    tempOut.resize(indexs->size(), 0);
    
    int minBufferOverflow = 1000;
    for(int i = 0; i < indexs->size(); i++){
        float newBuffIndex = f+bufferOverflow - float(1.0f-indexs->at(i));
        for(int j = 1; j < indexedBuffer.size(); j++){
            if(signbit(indexedBuffer[j-1].first - newBuffIndex) != signbit(indexedBuffer[j].first - newBuffIndex)){
                //Average tempOut[i] = (indexedBuffer[j-1].second + indexedBuffer[j].second)*0.5;
                
                if (abs(indexedBuffer[j-1].first - newBuffIndex) <= abs(indexedBuffer[j].first - newBuffIndex)) {
                    tempOut[i] = indexedBuffer[j-1].second;
                }else{
                    tempOut[i] = indexedBuffer[j].second;
                }
                    
                //Damping
                if(dampingPow < 0){
                    tempOut[i] *= pow((1-(indexs->at(i)*damping)), 1/(float)(-dampingPow + 1));
                }else{
                    tempOut[i] *= pow((1-(indexs->at(i)*damping)), (dampingPow + 1));
                }
                j = indexedBuffer.size();
            }
        }

        minBufferOverflow = std::min((int)newBuffIndex, minBufferOverflow);
    }
    if((int)indexedBuffer.back().first < minBufferOverflow){
        bufferOverflow--;
        while((int)indexedBuffer.back().first < minBufferOverflow){
            indexedBuffer.pop_back();
        }
        for(auto &bufPos : indexedBuffer){
            bufPos.first -= 1;
        }
    }
    
    output = tempOut;
}
