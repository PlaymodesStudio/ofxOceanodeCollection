//
//  vectorItemOperations.cpp
//  Espills_controller
//
//  Created by Eduard Frigola on 01/08/2018.
//

#include "vectorItemOperations.h"

vectorItemOperations::vectorItemOperations() : ofxOceanodeNodeModel("Vector Item Operations"){
    addParameter(triggerIndex.set("Trig.In", 0, 0, 2));
    addParameter(in1.set("Input.1", {0}, {-FLT_MAX}, {FLT_MAX}));
    addParameter(in2.set("Input.2", {0}, {-FLT_MAX}, {FLT_MAX}));
	addParameterDropdown(operation, "Op.", 0, {"Bypass", "Sum", "Multiply", "Mean", "Diff", "Max", "Min", "Modulo", "Div", "Subs", "Exp", "Log"});
	addOutputParameter(output.set("Output", {0}, {FLT_MIN}, {FLT_MAX}));
    

    
    listeners.push(in1.newListener([this](vector<float> &in){
        if(triggerIndex == 0 || triggerIndex == 2){
            computeOutput(in);
        }
    }));
    listeners.push(in2.newListener([this](vector<float> &in){
        if(triggerIndex == 1 || triggerIndex == 2){
            computeOutput(in);
        }
    }));
}

void vectorItemOperations::computeOutput(vector<float> &in){
	auto getValueForIndex = [](const vector<float> &vf, int i) ->float {
        if(i < vf.size()){
            return vf[i];
        }else{
            return vf[0];
        }
	};
	
	
	int maxSize = std::max(in1->size(), in2->size());
    if(maxSize > 0){
        vector<float> tempOut;
        tempOut.resize(maxSize, 0);
        switch(operation){
            case 0:
            {
//                if(triggerIndex == 0){
                    tempOut = in;
//                }else{
//                    tempOut = in;
//                }
                break;
            }
            case 1:
            {
                for(int i = 0; i < tempOut.size(); i++){
                    tempOut[i] = (getValueForIndex(in1, i) + getValueForIndex(in2, i));
                }
                break;
            }
            case 2:
            {
                for(int i = 0; i < tempOut.size(); i++){
                    tempOut[i] = (getValueForIndex(in1, i) * getValueForIndex(in2, i));
                }
                break;
            }
            case 3:
            {
                for(int i = 0; i < tempOut.size(); i++){
                    tempOut[i] = ((getValueForIndex(in1, i) + getValueForIndex(in2, i)) / 2);
                }
                break;
            }
            case 4:
            {
                for(int i = 0; i < tempOut.size(); i++){
                    tempOut[i] = abs(getValueForIndex(in1, i) - getValueForIndex(in2, i));
                }
                break;
            }
            case 5:
            {
                for(int i = 0; i < tempOut.size(); i++){
                    tempOut[i] = max(getValueForIndex(in1, i), getValueForIndex(in2, i));
                }
                break;
            }
            case 6:
            {
                for(int i = 0; i < tempOut.size(); i++){
                    tempOut[i] = std::min(getValueForIndex(in1, i), getValueForIndex(in2, i));
                }
                break;
            }
			case 7: //Modulo
			{
				for(int i = 0; i < tempOut.size(); i++){
					tempOut[i] = fmod(getValueForIndex(in1, i), getValueForIndex(in2, i));
				}
				break;
			}
			case 8: //Div
			{
				for(int i = 0; i < tempOut.size(); i++){
                    if(getValueForIndex(in2, i) == 0){
                        tempOut[i] = 0;
                    }else{
                        tempOut[i] = (getValueForIndex(in1, i) / getValueForIndex(in2, i));
                    }
				}
				break;
			}
			case 9: //Subs
			{
				for(int i = 0; i < tempOut.size(); i++){
					tempOut[i] = (getValueForIndex(in1, i) - getValueForIndex(in2, i));
				}
				break;
			}
			case 10: //Exp
			{
				for(int i = 0; i < tempOut.size(); i++){
					tempOut[i] = pow(getValueForIndex(in1, i), getValueForIndex(in2, i));
				}
				break;
			}
			case 11: //Log
			{
				for(int i = 0; i < tempOut.size(); i++){
					tempOut[i] = log(getValueForIndex(in1, i)) / log(getValueForIndex(in2, i));
				}
				break;
			}
            default:
            {
                tempOut = in1;
                break;
            }
        }
        output = tempOut;
    }
    
}
