//
//  valueModifier.h
//  Genesis
//
//  Created by Eduard Frigola Bagué on 9/5/22.
//

#ifndef valueModifier_h
#define valueModifier_h

#include "ofxOceanodeNodeModel.h"

class valueModifier : public ofxOceanodeNodeModel{
public:
    valueModifier() : ofxOceanodeNodeModel("Value Mod"){};
    
    void setup(){
        addParameter(input.set("Input", {0}, {0}, {1}));
        addParameter(randomAdd_Param.set("Rnd Add", {0}, {0}, {1}));
        addParameter(randomSubs_Param.set("Rnd Sub", {0}, {0}, {1}));
        addParameter(scale_Param.set("Scale", {1}, {0}, {2}));
        addParameter(offset_Param.set("Offset", {0}, {-1}, {1}));
        addParameter(pow_Param.set("Pow", {0}, {-1}, {1}));
        addParameter(biPow_Param.set("BiPow", {0}, {-1}, {1}));
        addParameter(quant_Param.set("Quant", {0}, {0}, {INT_MAX}));
        addParameter(amplitude_Param.set("Fader", {1}, {0}, {1}));
        addParameter(invert_Param.set("Invert", {0}, {0}, {1}));
        addOutputParameter(output.set("Output", {0}, {0}, {1}));
        
        listeners.push(input.newListener([&](vector<float> &val){
            compute();
        }));
        listeners.push(randomAdd_Param.newListener([&](vector<float> &val){
            manualInput();
        }));
        listeners.push(randomSubs_Param.newListener([&](vector<float> &val){
            manualInput();
        }));
        listeners.push(scale_Param.newListener([&](vector<float> &val){
            manualInput();
        }));
        listeners.push(offset_Param.newListener([&](vector<float> &val){
            manualInput();
        }));
        listeners.push(pow_Param.newListener([&](vector<float> &val){
            manualInput();
        }));
        listeners.push(biPow_Param.newListener([&](vector<float> &val){
            manualInput();
        }));
        listeners.push(quant_Param.newListener([&](vector<int> &val){
            manualInput();
        }));
        listeners.push(amplitude_Param.newListener([&](vector<float> &val){
            manualInput();
        }));
        listeners.push(invert_Param.newListener([&](vector<float> &val){
            manualInput();
        }));
    }
    
private:
    void manualInput(){
        if(!getOceanodeParameter(input).hasInConnection()){
            compute();
        }
    }
    
    void compute(){
        vector<float> tempOut = input;
        for(int i = 0; i < tempOut.size(); i++){
            float value = tempOut[i];
            //random Add
            if(getValueForPosition(randomAdd_Param, i))
                value += getValueForPosition(randomAdd_Param, i)*ofRandomuf();
            
            //random Subs
            if(getValueForPosition(randomSubs_Param, i))
                value -= getValueForPosition(randomSubs_Param, i)*ofRandomuf();
            
            //value = ofClamp(value, 0.0, 1.0);
            value = value + ((value < 0) * (0 - value)) + ((value > 1) * (1 - value));
            
            //SCALE
            value *= getValueForPosition(scale_Param, i);
            
            //OFFSET
            value += getValueForPosition(offset_Param, i);
            
            //    value = ofClamp(value, 0.0, 1.0);
            value = value + ((value < 0) * (0 - value)) + ((value > 1) * (1 - value));
            
            //pow
            if(getValueForPosition(pow_Param, i) != 0)
                customPow(value, getValueForPosition(pow_Param, i));
            
            //bipow
            if(getValueForPosition(biPow_Param, i) != 0){
                value = (value*2) -1;
                customPow(value, getValueForPosition(biPow_Param, i));
                value = (value+1) * 0.5;
            }
            
            //    value = ofClamp(value, 0.0, 1.0);
            value = value + ((value < 0) * (0 - value)) + ((value > 1) * (1 - value));
            
            //Quantization
            //Quantization
            if(getValueForPosition(quant_Param, i) == 1){
                value = 0;
            }
            else if(getValueForPosition(quant_Param, i) > 1){
                value = (1.0/((float)getValueForPosition(quant_Param,i)-1))*float(floor(value*getValueForPosition(quant_Param, i)));
            }
            
            //    value = ofClamp(value, 0.0, 1.0);
            value = value + ((value < 0) * (0 - value)) + ((value > 1) * (1 - value));
            
            value *= getValueForPosition(amplitude_Param, i);
            
            float invertedValue = 1-value;
            float nonInvertedValue = value;
            value = getValueForPosition(invert_Param, i) * invertedValue + (1-getValueForPosition(invert_Param, i)) * nonInvertedValue;
            
            tempOut[i] = value;
        }
        output = tempOut;
    }
    
    void customPow(float & value, float pow){
        float k1 = 2*pow*0.99999;
        float k2 = (k1/((-pow*0.999999)+1));
        float k3 = k2 * abs(value) + 1;
        value = value * (k2+1) / k3;
    }
    
    template <typename T>
    auto getValueForPosition(const ofParameter<vector<T>> &param, int index) -> T{
        if(param->size() == 1 || param->size() <= index){
            return param.get()[0];
        }
        else{
            return param.get()[index];
        }
    };
    
    ofParameter<vector<float>> input;
    ofParameter<vector<int>>  quant_Param;
    ofParameter<vector<float>>  scale_Param;
    ofParameter<vector<float>>  offset_Param;
    ofParameter<vector<float>>  randomAdd_Param;
    ofParameter<vector<float>>  randomSubs_Param;
    ofParameter<vector<float>>  pow_Param;
    ofParameter<vector<float>>  biPow_Param;
    ofParameter<vector<float>>  amplitude_Param;
    ofParameter<vector<float>>  invert_Param;
    ofParameter<vector<float>>  output;
    
    ofEventListeners listeners;
};

#endif /* valueModifier_h */
