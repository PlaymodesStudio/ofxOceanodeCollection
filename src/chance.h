//
//  chance.h
//  Oceanode
//
//  Created by Eduard Frigola Bagué on 27/10/2020.
//


#ifndef chance_h
#define chance_h

#include "ofxOceanodeNodeModel.h"
// Transport mode needs ofxOceanode's global transport (feature-globalTransport branch).
// Without it the node compiles exactly as before.
#if defined(OFX_OCEANODE_HAS_GLOBAL_TRANSPORT)
#include "ofxOceanodeDeterministicRandom.h"
#endif
#include <random>
#include <cfloat>

class chance : public ofxOceanodeNodeModel {
public:
	chance() : ofxOceanodeNodeModel("Chance"){};
	
	void setup(){
		addParameter(phaseIn.set("Phase", {0}, {0}, {1}));
		addParameter(seed.set("Seed", {0}, {(INT_MIN+1)/2}, {(INT_MAX-1)/2}));
		addParameter(probability.set("Prob", {0.5}, {0}, {1}));
		addParameter(retrigger.set("Retrig", false));
		addOutputParameter(output.set("Output", {0}, {0}, {1}));
		
		dist = std::uniform_real_distribution<float>(0.0, 1.0);
		oldPhasor.push_back(-1);
		highInNextFrame.push_back(false);
		mt.resize(1);
		setSeed();
		
		listeners.push(phaseIn.newListener([this](vector<float> &vf){
#if defined(OFX_OCEANODE_HAS_GLOBAL_TRANSPORT)
			if(syncToTransport){
				computeTransport();
				return;
			}
#endif
			vector<float> tempOut(output);
			if(oldPhasor.size() != vf.size()){
				//Resize everything
				oldPhasor.resize(vf.size(), -1);
				highInNextFrame.resize(vf.size(), false);
				mt.resize(vf.size());
				tempOut.resize(vf.size());
				setSeed();
			}
			
			// Make sure output vector is sized correctly
			if(tempOut.size() != vf.size()) {
				tempOut.resize(vf.size(), 0);
			}
			
			for(int i = 0; i < vf.size(); i++){
				if(highInNextFrame[i]){
					tempOut[i] = 1;
					highInNextFrame[i] = false;
				}
				if(vf[i] < oldPhasor[i]){ //Phasor has been reset
					// Get the correct probability for this index
					float currentProb = probability->size() == 1 ?
									   probability->at(0) :
									   (i < probability->size() ? probability->at(i) : probability->at(0));
					
					if(dist(mt[i]) < currentProb){
						if(retrigger && tempOut[i] == 1){
							tempOut[i] = 0;
							highInNextFrame[i] = true;
						}else{
							tempOut[i] = 1;
						}
					}else{
						tempOut[i] = 0;
					}
				}
			}
			oldPhasor = vf;
			output = tempOut;
		}));
		
		listeners.push(seed.newListener([this](vector<int> &i){
			setSeed();
#if defined(OFX_OCEANODE_HAS_GLOBAL_TRANSPORT)
			if(syncToTransport) computeTransport();
#endif
		}));
		
#if defined(OFX_OCEANODE_HAS_GLOBAL_TRANSPORT)
		// ---- Sync To Transport ----
		// Output is a pure function of (Seed, Step, Prob): the roll for each cycle comes from
		// a hash of the step instead of an RNG sequence, so scrubbing the timeline gives the
		// same pass/fail pattern as playback. Connect Step to a Phasor "Cycle" output.
		sessionSalt = ofxOceanodeDeterministicRandom::makeSessionSalt();
		addInspectorParameter(syncToTransport.set("Sync To Transport", false));
		listeners.push(syncToTransport.newListener([this](bool &b){
			setStepInputVisible(b);
			transportPrimed = false;
			if(b) computeTransport();
		}));
		listeners.push(stepIn.newListener([this](vector<float> &){
			// With a connected Phase, compute when the phase arrives (the Phasor sends Cycle first).
			if(syncToTransport && !getOceanodeParameter(phaseIn).hasInConnection()) computeTransport();
		}));
#endif
	}

	void resetPhase(){
#if defined(OFX_OCEANODE_HAS_GLOBAL_TRANSPORT)
		if(syncToTransport) return; // position comes from the transport
#endif
		setSeed();
	}
	
#if defined(OFX_OCEANODE_HAS_GLOBAL_TRANSPORT)
	void loadBeforeConnections(ofJson &json) override {
		// Restore the mode before connections so a saved "Step" connection finds its input.
		deserializeParameter(json, syncToTransport);
	}
#endif
	
private:
	
	// ---- transport mode ----
#if defined(OFX_OCEANODE_HAS_GLOBAL_TRANSPORT)
	ofParameter<bool> syncToTransport;
	ofParameter<vector<float>> stepIn; // only present in Sync To Transport mode
	uint64_t sessionSalt = 0;
	bool transportPrimed = false;
	vector<int64_t> lastTransportStep;
	
	void setStepInputVisible(bool visible){
		const bool present = getParameterGroup().contains("Step");
		if(visible && !present){
			addParameter(stepIn.set("Step", {0}, {0}, {FLT_MAX}));
		}else if(!visible && present){
			getOceanodeParameter(stepIn).removeAllConnections();
			removeParameter("Step");
		}
	}
	
	// Same seed rules as setSeed(): per-lane seeds, or single seed + lane offset; 0 = session-random.
	uint64_t laneKey(size_t i, size_t lanes){
		int s = 0;
		if(seed->size() == lanes) s = seed->at(i);
		else if(!seed->empty() && seed->at(0) != 0) s = seed->at(0) + (int)i;
		if(s == 0) return ofxOceanodeDeterministicRandom::mix(sessionSalt ^ (uint64_t)i);
		return ofxOceanodeDeterministicRandom::seedKey(s, sessionSalt);
	}
	
	bool passes(size_t i, size_t lanes, int64_t step){
		const float p = probability->size() == 1 ? probability->at(0)
			: (i < probability->size() ? probability->at(i) : probability->at(0));
		return ofxOceanodeDeterministicRandom::uniform(laneKey(i, lanes), step, 0) < p;
	}
	
	void computeTransport(){
		const auto &steps = stepIn.get();
		if(steps.empty()) return;
		const size_t lanes = std::max(phaseIn->size(), steps.size());
		vector<float> tempOut(lanes, 0);
		highInNextFrame.resize(lanes, false);
		lastTransportStep.resize(lanes, 0);
		for(size_t i = 0; i < lanes; i++){
			const float stepValue = i < steps.size() ? steps[i] : steps[0];
			const int64_t step = ofxOceanodeDeterministicRandom::stepFromFloat(stepValue);
			const bool pass = passes(i, lanes, step);
			if(highInNextFrame[i]){ // second half of a retrigger
				highInNextFrame[i] = false;
				tempOut[i] = pass ? 1 : 0;
			}else{
				tempOut[i] = pass ? 1 : 0;
				// Retrig: two passing cycles in a row during continuous playback -> one-frame dip.
				const bool advancedByOne = transportPrimed && step == lastTransportStep[i] + 1;
				if(retrigger && pass && advancedByOne && passes(i, lanes, step - 1)){
					tempOut[i] = 0;
					highInNextFrame[i] = true;
				}
			}
			lastTransportStep[i] = step;
		}
		transportPrimed = true;
		output = tempOut;
	}
#endif
	
	void setSeed(){
		for(int i = 0; i < mt.size(); i++){
			if(seed->size() == mt.size()){
				int s = seed->at(i);
				if(s == 0){
					std::random_device rd;
					mt[i].seed(rd());
				}else{
					mt[i].seed(s);
				}
			}
			else{
				if(seed->at(0) == 0){
					std::random_device rd;
					mt[i].seed(rd());
				}else{
					mt[i].seed(seed->at(0) + i);
				}
			}
		}
	}
	
	ofEventListeners listeners;
	
	ofParameter<vector<float>> phaseIn;
	ofParameter<vector<int>> seed;
	ofParameter<vector<float>> probability;
	ofParameter<bool> retrigger;
	ofParameter<vector<float>> output;
	
	vector<float> oldPhasor;
	vector<bool> highInNextFrame;
	
	vector<std::mt19937> mt;
	std::uniform_real_distribution<float> dist;
};

#endif /* chance_h */
