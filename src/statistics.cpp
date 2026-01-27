//
//  statistics.cpp
//  Laser_Generator
//
//  Created by Eduard Frigola Bagué on 06/07/2021.
//

#include "statistics.h"

void statistics::setup(){
    addParameter(gain.set("Gain", 1, 0, FLT_MAX));
	addParameter(size.set("Size", 2, 2, INT_MAX));
	addParameter(weights.set("Weights", {0}, {0}, {1}));
    addParameter(input.set("Input", {0}, {0}, {1}));
    addOutputParameter(variance.set("Variance", {0}, {0}, {1}));
    addOutputParameter(mean.set("Mean", {0}, {0}, {1}));
    color = ofColor::green;
}

void statistics::update(ofEventArgs &a){
    if(inputStore.size() == 0 || inputStore[0].size() != input->size()){
		inputStore.clear();
		inputStore.push_back(input);
	}
	
	vector<float> currentWeights(size, 1);
	if(weights->size() == size){
		if(std::accumulate(weights->begin(), weights->end(), 0.0) != 0.0){
			currentWeights = weights;
		}
	}
	
	inputStore.push_back(input);
	while(inputStore.size() > size) inputStore.pop_front();
	vector<float> tempVar(input->size());
	vector<float> tempMean(input->size());
	for(int i = 0; i < input->size(); i++){
		vector<float> tempCalc(inputStore.size());
		for(int j = 0; j < inputStore.size(); j++){
			tempCalc[j] = inputStore[j][i];
		}
		float itemMean = std::accumulate(tempCalc.begin(), tempCalc.end(), 0.0f) / float(tempCalc.size());
		vector<float> tempCalc2 = tempCalc;
		std::transform(tempCalc.begin(), tempCalc.end(), tempCalc.begin(), [itemMean](float &f){
			return pow(f - itemMean, 2);
		});
		
		for(int k = 0; k < tempCalc2.size(); k++) tempCalc2[k] *= currentWeights[k];
		tempMean[i] = std::accumulate(tempCalc2.begin(), tempCalc2.end(), 0.0f) / std::accumulate(currentWeights.begin(), currentWeights.end(), 0.0f);
		
		for(int k = 0; k < tempCalc.size(); k++) tempCalc[k] *= currentWeights[k];
		tempVar[i] = std::accumulate(tempCalc.begin(), tempCalc.end(), 0.0f) / float(tempCalc.size()-1);
		tempVar[i]*= gain;
	}
	variance = tempVar;
	mean = tempMean;
}
