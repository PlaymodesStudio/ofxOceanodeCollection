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


#include "ofxOceanode.h"

namespace ofxOceanodeCollection{
static void registerModels(ofxOceanode &o){
    o.registerModel<delta>("Collection"); 
    o.registerModel<dummySliders>("Collection");
    o.registerModel<envelopeGenerator>("Collection");
    o.registerModel<euclideanGenerator>("Collection");
    o.registerModel<manualOscillatorBank>("Collection");
    o.registerModel<sequentialAnalyzer>("Collection");
    o.registerModel<stepSequencer>("Collection");
    o.registerModel<subDimensionCombinator>("Collection");
    o.registerModel<threshold>("Collection");
    o.registerModel<valueEaser>("Collection");
    o.registerModel<vectorChain>("Collection");
    o.registerModel<vectorGetter>("Collection");
    o.registerModel<vectorItemOperations>("Collection");
    o.registerModel<vectorModulator>("Collection");
    o.registerModel<vectorOperations>("Collection");
    o.registerModel<vectorPresetEaser>("Collection");
    o.registerModel<manualVector>("Collection");
}
}


#endif /* ofxOceanodeCollection_h */
