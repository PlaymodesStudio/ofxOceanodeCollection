//
//  ofxOceanodeCollection.h
//  
//
//  Created by Eduard Frigola Bagué on 17/06/2020.
//

#ifndef ofxOceanodeCollection_h
#define ofxOceanodeCollection_h

// Declarations only: the node headers and the bodies live in ofxOceanodeCollection.cpp, so
// including this header does not compile every node of the addon again.

class ofxOceanode;

namespace ofxOceanodeCollection{

void registerModels(ofxOceanode &o);
void registerCollection(ofxOceanode &o);

}

#endif /* ofxOceanodeCollection_h */
