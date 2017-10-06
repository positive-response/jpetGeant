#ifndef ParticleBlock_h
#define ParticleBlock_h

#include "globals.hh"
#include "TObject.h"
#include "G4ThreeVector.hh"
#include <vector> 


class ParticleBlock : public TObject
{
    public:
        ParticleBlock();
        ~ParticleBlock();

    private:
        G4int fParticlePDGId;

    ClassDef(ParticleBlock,1)     
};


#endif
