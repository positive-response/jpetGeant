#ifndef ScinBlock_h
#define ScinBlock_h

#include "globals.hh"
#include "TObject.h"
#include "G4ThreeVector.hh"
#include <vector> 


class ScinBlock : public TObject
{
    public:
        ScinBlock();
        ~ScinBlock();
        void Clean();

    private:
        G4int fScinPDGId;

    ClassDef(ScinBlock,1)     
};


#endif
