//
//  statistics.h
//  ofxOceanodeCollection
//
//  Created by Eduard Frigola Bagué on 06/07/2021.
//

#ifndef statistics_h
#define statistics_h

#include "ofxOceanodeNodeModel.h"

class statistics : public ofxOceanodeNodeModel{
public:
	statistics() : ofxOceanodeNodeModel("Statistics"){};
    ~statistics(){};
	
	void setup();
	void update(ofEventArgs &a);
    
private:
    void computeOutput(vector<float> &in);
    ofEventListener listener;
    
    ofParameter<float>  gain;
	ofParameter<int> 	size;
    ofParameter<vector<float>>   input;
	ofParameter<vector<float>>	weights;
    ofParameter<vector<float>>   variance;
	ofParameter<vector<float>> mean;
    
    deque<vector<float>>   inputStore;
};

#endif /* statistics_h */
