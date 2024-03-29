//
//  transformations.h
//  ofxOceanodeCollection
//
//  Created by Eduard Frigola Bagué on 14/06/2021.
//

#ifndef transformations_h
#define transformations_h

#include "ofxOceanodeNodeModel.h"

class transformations : public ofxOceanodeNodeModel{
public:
	transformations() : ofxOceanodeNodeModel("Transformations"){};
	
	void setup(){
		addParameter(input.set("Input", {0}, {-FLT_MAX}, {FLT_MAX}));
		addParameterDropdown(transformation, "Trans", 0, {"Bypass", "Invert", "Reciprocal", "Neg", "Abs", "Floor", "Round", "Ceil"});
        addOutputParameter(output.set("Output", {0}, {-FLT_MAX}, {FLT_MAX}));
		
		listener = input.newListener([this](vector<float> &vf){
			vector<float> tempOut = vf;
			std::transform(tempOut.begin(),
						   tempOut.end(),
						   tempOut.begin(),
						   [this](float& f){
				switch (transformation) {
					case 0:
						return f;
						break;
					case 1:
						return 1-f;
						break;
					case 2:
						return 1/f;
						break;
					case 3:
						return -f;
						break;
					case 4:
						return abs(f);
						break;
                    case 5:
                        return floor(f);
                        break;
                    case 6:
                        return round(f);
                        break;
                    case 7:
                        return ceil(f);
                        break;
					default:
                        return f;
						break;
				}
			});
			output = tempOut;
		});
	}
	
private:
	ofParameter<vector<float>> input;
	ofParameter<int>   transformation;
	ofParameter<vector<float>> output;
	
	ofEventListener listener;
};


#endif /* transformations_h */
