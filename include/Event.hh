#ifndef Event_h
#define Event_h

#include "ParticleBlock.hh"
#include "VtxBlock.hh"
#include "TObject.h"

//class VtxBlock;
//class ParticleBlock;

class Event : public TObject
{
    private:
        //VtxBlock    fVtx;
        G4int   someConst;


    public:
        Event();
        ~Event();

        ClassDef(Event,1)
};



#endif
