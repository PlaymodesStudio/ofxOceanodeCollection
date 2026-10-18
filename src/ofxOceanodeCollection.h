//
//  ofxOceanodeCollection.h
//  
//
//  Created by Eduard Frigola Bagué on 17/06/2020.
//

#ifndef ofxOceanodeCollection_h
#define ofxOceanodeCollection_h


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
static void registerModels(ofxOceanode &o){
    o.registerProcessingModel<delta>("Collection");
    o.registerProcessingModel<dummySliders>("Collection");
    o.registerProcessingModel<dummySlidersDynamic>("Collection");
    o.registerProcessingModel<envelopeGenerator>("Collection");
    o.registerProcessingModel<euclideanGenerator>("Collection");
    o.registerProcessingModel<manualOscillatorBank>("Collection");
    o.registerProcessingModel<sequentialAnalyzer>("Collection");
    o.registerProcessingModel<stepSequencer>("Collection");
    o.registerProcessingModel<subDimensionCombinator>("Collection");
    o.registerProcessingModel<threshold>("Collection");
    o.registerProcessingModel<valueEaser>("Collection");
    o.registerProcessingModel<vectorChain>("Collection");
    o.registerProcessingModel<vectorGetter>("Collection");
    o.registerProcessingModel<vectorItemOperations>("Collection");
    o.registerProcessingModel<vectorModulator>("Collection");
    o.registerProcessingModel<vectorOperations>("Collection");
    o.registerProcessingModel<vectorPresetEaser>("Collection");
    o.registerProcessingModel<manualVector>("Collection");
    o.registerProcessingModel<chance>("Collection");
    o.registerProcessingModel<markovChain>("Collection");
	o.registerProcessingModel<transformations>("Collection");
	o.registerProcessingModel<statistics>("Collection");
	o.registerProcessingModel<vectorResize>("Collection");
    o.registerProcessingModel<vectorSelector>("Collection");
    o.registerProcessingModel<vectorReorder>("Collection");
    o.registerProcessingModel<vectorDistributor>("Collection");
    o.registerProcessingModel<vectorRotator>("Collection");
    o.registerProcessingModel<slotEaser>("Collection");
    o.registerProcessingModel<valueModifier>("Collection");
    o.registerProcessingModel<histogram>("Collection");
	o.registerProcessingModel<modTime>("Collection");
	}
}


#endif /* ofxOceanodeCollection_h */
