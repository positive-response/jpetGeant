#include "ScinBlock.hh"

ScinBlock::ScinBlock()
     :evtID(0), scinID(0), trackID(-1), eDep(0), time(0), position(0)
{
}

ScinBlock::~ScinBlock()
{}

void ScinBlock::Clean()
{}

void ScinBlock::Fill(G4int iD, DetectorHit* hit)
{
    evtID = iD;
    scinID = hit->GetScinID();
    trackID = hit->GetTrackID();
    eDep = hit->GetEdep();
    time = hit->GetTime();
    position = hit->GetPosition();
}
