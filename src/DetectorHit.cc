#include "DetectorHit.hh"

DetectorHit::DetectorHit()
    : G4VHit(), fEdep(0.0), fPos(0), fTrackID(-1) 
{}


DetectorHit::~DetectorHit()
{}

