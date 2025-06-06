
//
//  vectorResize.h
//  ofxOceanodeCollection
//
//  Created by Eduard Frigola Bagué on 14/09/2021.
//

#ifndef vectorResize_h
#define vectorResize_h

#include "ofxOceanodeNodeModel.h"

class vectorResize : public ofxOceanodeNodeModel{
public:
    vectorResize() : ofxOceanodeNodeModel("Vector Resize"){
        addParameter(input.set("Input", {0}, {0}, {1}));
        addParameter(size.set("Size", 1, 1, INT_MAX));
        addParameter(resample.set("Resample", true));
		addParameterDropdown(interp, "Interp", 0, {"None", "Avg", "Min", "Max"});
		addOutputParameter(output.set("Output", {0}, {0}, {1}));
        
        addInspectorParameter(fill.set("Fill", false));
        
        listener = input.newListener(this, &vectorResize::inputListener);
        listener2 = size.newListener([this](int &i){
            vector<float> v = input.get();
            inputListener(v);
        });
    };
    
    void loadBeforeConnections(ofJson &json){
        deserializeParameter(json, size);
    }
    
private:
    void inputListener(vector<float> &v){
        int _size = size;
        if(_size < 1) _size = 1;
        if(v.size() > 0){
            if(!resample){
                vector<float> tempOut = v;
                if(fill && _size > v.size()){
                    while(tempOut.size() < _size){
                        tempOut.insert(tempOut.end(), v.begin(), v.end());
                    }
                }
                tempOut.resize(_size, 0);
                
                output = tempOut;
            }else{
			if(v.size() == _size){
				output = v;
			}else if(v.size() < _size){
				vector<float> tempOut(_size, 0);
				for(int i = 0; i < _size; i++){
					tempOut[i] = v[(int)(((float)i / (float)_size) * v.size())];
				}
				output = tempOut;
			}else{
				vector<float> tempOut(_size, 0);
				for(int i = 0; i < _size; i++){
					float index = (((float)i / (float)_size) * v.size());
					switch(interp){
						case 0:
							tempOut[i] = v[(int)index];
							break;
						case 1:{
							float tempSum = 0;
							int steps = 0;
							for(int j = floor(index); j < floor(index + (v.size()/_size)); j++){
								tempSum += v[j];
								steps++;
							}
							tempOut[i] = tempSum/steps;
							break;
						}
							case 2:{
								float minVal = FLT_MAX;
								for(int j = floor(index); j < floor(index + (v.size()/_size)); j++){
									if(v[j] < minVal){
										minVal = v[j];
									}
								}
								tempOut[i] = minVal;
								break;
							}
							case 3:{
								float maxVal = -FLT_MAX;
								for(int j = floor(index); j < floor(index + (v.size()/_size)); j++){
									if(v[j] > maxVal){
										maxVal = v[j];
									}
								}
								tempOut[i] = maxVal;
								break;
							}
						default:
							break;
					}
				}
				output = tempOut;
			}
            }
        }
        else if (v.size() == 0){
            vector<float> tempOut(0, 0);
            output = tempOut;
        }
            
    }
    
    ofEventListener listener;
    ofEventListener listener2;
    
    ofParameter<vector<float>>  input;
    ofParameter<int> size;
    ofParameter<bool> resample;
    ofParameter<bool> fill;
	ofParameter<int> interp;
    ofParameter<vector<float>>  output;
};

#endif /* vectorResize_h */
