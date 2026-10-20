//
//  ofxOceanodeCollection.cpp
//  
//
//  Created by Eduard Frigola Bagué on 17/06/2020.
//

#include "ofxOceanodeCollection.h"
#include "delta.h"
#include "dummySliders.h"
#include "dummySlidersDynamic.h"
#include "envelopeGenerator.h"
#include "euclideanGenerator.h"
#include "manualOscillatorBank.h"
#include "sequentialAnalyzer.h"
#include "stepSequencer.h"
#include "subDimensionCombinator.h"
#include "thresholdBang.h"
#include "valueEaser.h"
#include "vectorChain.h"
#include "vectorGetter.h"
#include "vectorItemOperations.h"
#include "vectorModulator.h"
#include "vectorOperations.h"
#include "vectorPresetEaser.h"
#include "manualVector.h"
#include "chance.h"
#include "markovChain.h"
#include "transformations.h"
#include "statistics.h"
#include "vectorResize.h"
#include "vectorSelector.h"
#include "vectorReorder.h"
#include "vectorDistributor.h"
#include "vectorRotator.h"
#include "slotEaser.h"
#include "valueModifier.h"
#include "histogram.h"
#include "modTime.h"


#include "ofxOceanode.h"

namespace ofxOceanodeCollection{
void registerModels(ofxOceanode &o){
    o.registerModel<delta>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused); 
    o.registerModel<dummySliders>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
	o.registerModel<dummySlidersDynamic>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<envelopeGenerator>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<euclideanGenerator>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<manualOscillatorBank>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<sequentialAnalyzer>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<stepSequencer>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<subDimensionCombinator>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<threshold>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<valueEaser>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<vectorChain>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<vectorGetter>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<vectorItemOperations>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<vectorModulator>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<vectorOperations>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<vectorPresetEaser>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<manualVector>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<chance>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<markovChain>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
	o.registerModel<transformations>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
	o.registerModel<statistics>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
	o.registerModel<vectorResize>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<vectorSelector>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<vectorReorder>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<vectorDistributor>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<vectorRotator>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<slotEaser>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<valueModifier>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
    o.registerModel<histogram>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
	o.registerModel<modTime>("Collection", ofxOceanodeNodeRegistrationFlags::CanRemoveWhenUnused);
	}

void registerCollection(ofxOceanode &o){
    registerModels(o);
}

}
