#ifndef Event_h
#define Event_h

#include "ParticleBlock.hh"
#include "VtxBlock.hh"
#include "TObject.h"
#include "ParticleBlock.hh"


class Event : public TObject
{
    private:
        VtxBlock*   fVtx;
        ParticleBlock*  fParticles;


    public:
        Event();
        ~Event();
        void Clean();

        VtxBlock* GetVtxBlock() const {return fVtx;}
        ParticleBlock* GetParticleBlock() const {return fParticles;}

        ClassDef(Event,1)
};



#endif
