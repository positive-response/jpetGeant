#ifndef ScinBlock_h
#define ScinBlock_h

#include "globals.hh"
#include "TObject.h"
#include "G4ThreeVector.hh"
#include <vector> 
#include "DetectorHit.hh"
#include "TVector3.h"


/**
 * \class ScinBlock
 * \brief class directly written into ROOT files; translates  
 *
 */
class ScinBlock : public TObject
{
    public:
        ScinBlock();
        ~ScinBlock();
        void Clean();
        void Fill(G4int hitTNo, G4int hitNo, G4int iD, DetectorHit* hit);
        G4double GetEdep() {return eDep;}
        G4int GetScinID() {return scinID;}
        G4int GetTotHitNumber() {return hitTotNumber;}
        G4int GetHitNumber() {return hitNumber;}
        TVector3 GetPositon() {return position;}
        G4double GetTime() {return time;}

    private:
        G4int hitTotNumber;
        G4int hitNumber;
        G4int evtID;
        G4int numInteractions;
        G4int scinID;
        G4int trackID;
        G4int trackPDGencoding;
        G4double eDep;
        G4double time;
        TVector3 position;
        TVector3 polarizationIn;
        TVector3 polarizationOut;
        TVector3 momentumIn;
        TVector3 momentumOut;

    ClassDef(ScinBlock,1)     
};


#endif
