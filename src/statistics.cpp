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
	addOutputParameter(smoothChange.set("MeanVariation", {0}, {0}, {1}));
	color = ofColor::green;
}

void statistics::update(ofEventArgs &a){
	vector<float> inputCopy = input.get();
	
	if(inputStore.empty() || inputStore.back().size() != inputCopy.size()){
		inputStore.clear();
	}

	inputStore.push_back(inputCopy);
	while(inputStore.size() > size) inputStore.pop_front();
	
	if(inputStore.size() < 2) {
		variance = vector<float>(inputCopy.size(), 0.0f);
		mean = vector<float>(inputCopy.size(), 0.0f);
		smoothChange = vector<float>(inputCopy.size(), 0.0f);
		return;
	}

	vector<float> currentWeights(inputStore.size(), 1);
	if(weights->size() == inputStore.size()){
		if(std::accumulate(weights->begin(), weights->end(), 0.0) != 0.0){
			currentWeights = weights.get();
		}
	}

	vector<float> tempVar(inputCopy.size());
	vector<float> tempMean(inputCopy.size());
	vector<float> tempSmoothChange(inputCopy.size());
	
	for(int i = 0; i < inputCopy.size(); i++){
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
		tempVar[i] *= gain;
		
		// Calculate smoothed absolute change (frame-to-frame differences)
		float totalChange = 0.0f;
		for(int j = 1; j < inputStore.size(); j++){
			totalChange += fabs(inputStore[j][i] - inputStore[j-1][i]);
		}
		tempSmoothChange[i] = (totalChange / float(inputStore.size() - 1)) * gain;
	}
	
	variance = tempVar;
	mean = tempMean;
	smoothChange = tempSmoothChange;
}
