#include "Event.hh"


Event::Event()
{
    fVtx = new VtxBlock(); 
    fParticles = new ParticleBlock();
}

Event::~Event()
{}

void Event::Clean()
{
//    fVtx.Clean();
//    fParticles.Clean();
}


