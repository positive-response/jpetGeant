#ifndef VtxBlock_h
#define VtxBlock_h

#include "globals.hh"
#include "TObject.h"
#include "G4ThreeVector.hh"
#include <vector>


class VtxBlock : public TObject 
{

    public:
        VtxBlock();
        ~VtxBlock();
        void SetMotherId(G4int val) { fMotherId = val; }
        void SetTime(G4double val) { fTime = val; }


    private:
        G4int           fVtxNum;
        G4int           fMotherId;
        //G4int         fProgeniesId[ some conts];
//        G4ThreeVector   fPosition;
//        G4ThreeVector   fMomentum;
        G4double        fTime;
        G4int           fMaterial;


        ClassDef(VtxBlock,1)

};

#endif
