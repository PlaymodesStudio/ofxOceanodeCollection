//
//  Created by Eduard Frigola Bagué on 05/07/2018.
//

#ifndef histogram_h
#define histogram_h

#include "ofxOceanodeNodeModel.h"

class histogram : public ofxOceanodeNodeModel{
public:
	histogram() : ofxOceanodeNodeModel("Histogram"){}
	
	void setup(){
		addParameter(input.set("Input", {0}, {-FLT_MAX}, {FLT_MAX}));
		addParameter(size.set("Size", 100, 1, INT_MAX));
		addParameter(speed.set("Speed", 1, 0, FLT_MAX));
		addParameter(min.set("Min", 0, -FLT_MIN, FLT_MAX));
		addParameter(max.set("Max", 1, -FLT_MIN, FLT_MAX));
		addOutputParameter(output.set("Output", {{0}}, {{0}}, {{1}}));
		
		listeners.push(min.newListener([this](float &f){
			output.setMin(vector<vector<float>>(1, vector<float>(1, f)));
		}));
		listeners.push(max.newListener([this](float &f){
			output.setMax(vector<vector<float>>(1, vector<float>(1, f)));
		}));
	}
	
	void update(ofEventArgs &a){
		if(cue.size() != size){
			cue.resize(size, input.get());
		}
		cue.push_back(input);
		cue.pop_front();
		output = vector<vector<float>>(cue.begin(), cue.end());
	}
	
private:
	ofParameter<vector<float>> input;
	ofParameter<float> speed;
	ofParameter<int> size;
	ofParameter<float> min, max;
	
	ofEventListeners listeners;
	
	deque<vector<float>> cue;
	ofParameter<vector<vector<float>>> output;
};


#endif /* histogram_h */
