
/*
 * dummySlidersDynamic.cpp
 *
 * A dynamic dummy slider node for ofxOceanode that provides configurable number of sliders
 * with automatic output vector generation.
 *
 * FUNCTIONALITY:
 * - Creates a configurable number of individual float sliders (1 to unlimited)
 * - Outputs a vector containing all current slider values
 * - Two range modes: normalized (0-1) or unlimited (-∞ to +∞)
 * - Real-time updates: output vector changes automatically when any slider is modified
 * - Dynamic resizing: add/remove sliders on-the-fly via inspector
 *
 * INSPECTOR PARAMETERS:
 * - "Num Sliders": Sets the number of individual sliders (default: 8, min: 1, max: unlimited)
 * - "Range [0..1]": Toggle between normalized (true) and unlimited (false) range modes
 *
 * OUTPUT:
 * - "Output": Vector of float values, one for each slider, always positioned at top
 *
 * USE CASES:
 * - Manual control arrays for animations, effects, or parameter sets
 * - Testing and prototyping with multiple controllable values
 * - Creating custom control interfaces with variable parameter counts
 */

#include "dummySlidersDynamic.h"

void dummySlidersDynamic::setup(){
    description = "Dynamic dummy sliders with configurable count and range. Inspector: 'Num Sliders' sets quantity (1-∞), 'Range [0..1]' toggles between normalized (0-1) or unlimited (-∞ to +∞) ranges. Output vector updates automatically when any slider changes.";
	
	// Inspector Parameters: Configure the node behavior
	// - "Num Sliders": Controls how many individual sliders are created (1 to unlimited)
	// - "Range [0..1]": When true, all sliders use normalized 0-1 range; when false, unlimited range
	addInspectorParameter(numSliders.set("Num Sliders", 8, 1, INT_MAX));
	addInspectorParameter(normalizedRange.set("Range [0..1]", true));
	
	// Initialize and add output parameter first - it will stay at the top
	// Output is a vector containing all slider values, initially all zeros
	vector<float> initialOutput(numSliders, 0.0f);
	addOutputParameter(output.set("Output", initialOutput, vector<float>(numSliders, 0.0f), vector<float>(numSliders, 1.0f)));
	
	// Create the initial set of slider parameters
	sliders.resize(numSliders);
	for(int i = 0; i < numSliders; i++){
		// Create each slider with normalized range (0-1) by default
		addParameter(sliders[i].set(ofToString(i), 0, 0, 1));
		
		// Add listener to each slider that updates the output vector whenever any slider value changes
		listeners.push(sliders[i].newListener([this](float &value){
			// Collect all current slider values into output vector
			vector<float> outputValues;
			for(auto& slider : sliders){
				outputValues.push_back(slider.get());
			}
			output.set(outputValues);
		}));
	}
	
	// Set initial output values to match current slider values (all 0.0)
	vector<float> outputValues;
	for(auto& slider : sliders){
		outputValues.push_back(slider.get());
	}
	output.set(outputValues);
	
	// Add listener for normalizedRange toggle - updates all existing sliders when range mode changes
	listeners.push(normalizedRange.newListener([this](bool &normalized){
		// Update range and values for all existing sliders
		for(auto& slider : sliders){
			float currentValue = slider.get();
			if(normalized){
				// Switch to normalized mode: set range to 0-1 and clamp existing values
				slider.setMin(0.0f);
				slider.setMax(1.0f);
				slider.set(ofClamp(currentValue, 0.0f, 1.0f));
			}else{
				// Switch to unlimited mode: set range to full float range, keep current values
				slider.setMin(-FLT_MAX);
				slider.setMax(FLT_MAX);
				// No clamping needed since we're expanding the range
			}
		}
		
		// Update output parameter range to match the new slider ranges
		if(normalized){
			output.setMin(vector<float>(numSliders, 0.0f));
			output.setMax(vector<float>(numSliders, 1.0f));
		}else{
			output.setMin(vector<float>(numSliders, -FLT_MAX));
			output.setMax(vector<float>(numSliders, FLT_MAX));
		}
	}));
	
	// Add listener for numSliders changes - dynamically adds/removes sliders when count changes
	listeners.push(numSliders.newListener([this](int &i){
		if(sliders.size() != i){
			int oldSize = sliders.size();
			bool remove = oldSize > i; // true if we need to remove sliders, false if adding
			
			// Resize the sliders vector to match new count
			sliders.resize(i);
			
			if(remove){
				// REMOVING SLIDERS: Remove excess slider parameters from the node
				for(int j = oldSize-1; j >= i; j--){
					removeParameter(ofToString(j));
				}
				
				// Update output parameter size and range
				vector<float> currentOutput;
				for(auto& slider : sliders){
					currentOutput.push_back(slider.get());
				}
				output.set(currentOutput);
				
				// Update output parameter range based on current normalizedRange setting
				if(normalizedRange){
					output.setMin(vector<float>(i, 0.0f));
					output.setMax(vector<float>(i, 1.0f));
				}else{
					output.setMin(vector<float>(i, -FLT_MAX));
					output.setMax(vector<float>(i, FLT_MAX));
				}
			}else{
				// ADDING SLIDERS: Create new slider parameters for the additional slots
				for(int j = oldSize; j < i; j++)
				{
					// Create new slider with range based on current normalizedRange setting
					if(normalizedRange)
					{
						addParameter(sliders[j].set(ofToString(j), {0}, {0}, {1}));
					}
					else
					{
						addParameter(sliders[j].set(ofToString(j), {0}, {-FLT_MAX}, {FLT_MAX}));
					}
					
					// Add listener to new slider so it updates output when changed
					listeners.push(sliders[j].newListener([this](float &value){
						vector<float> outputValues;
						for(auto& slider : sliders){
							outputValues.push_back(slider.get());
						}
						output.set(outputValues);
					}));
				}
				
				// Update output parameter size and range
				vector<float> currentOutput;
				for(auto& slider : sliders){
					currentOutput.push_back(slider.get());
				}
				output.set(currentOutput);
				
				// Update output parameter range based on current normalizedRange setting
				if(normalizedRange){
					output.setMin(vector<float>(i, 0.0f));
					output.setMax(vector<float>(i, 1.0f));
				}else{
					output.setMin(vector<float>(i, -FLT_MAX));
					output.setMax(vector<float>(i, FLT_MAX));
				}
			}
		}
	}));
}
