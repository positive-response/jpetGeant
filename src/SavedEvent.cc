#include "SavedEvent.hh"


SavedEvent::SavedEvent()
{
    fTrk = new TrkBlock(); 
    fScin = new ScinBlock();
}

SavedEvent::~SavedEvent()
{}

void SavedEvent::Clean()
{
//    fTrk.Clean();
//    fParticles.Clean();
}


