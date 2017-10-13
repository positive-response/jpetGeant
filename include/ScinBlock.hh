#ifndef ScinBlock_h
#define ScinBlock_h

#include "globals.hh"
#include "TObject.h"
#include "G4ThreeVector.hh"
#include <vector> 
#include "DetectorHit.hh"


class ScinBlock : public TObject
{
    public:
        ScinBlock();
        ~ScinBlock();
        void Clean();
        void Fill(G4int iD, DetectorHit* hit);

    private:
        G4int evtID;
        G4int scinID;
        G4int trackID;
        G4double eDep;
        G4double time;
        G4ThreeVector position;

    ClassDef(ScinBlock,1)     
};


#endif
