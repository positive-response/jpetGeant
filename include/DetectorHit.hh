#ifndef DetectorHit_h
#define DetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Types.hh"


class DetectorHit : public G4VHit
{
    public:
    DetectorHit();
    virtual ~DetectorHit();
    void SetEdep(G4double de)  { fEdep = de; }

    private:
    G4int fTrackID;
    G4ThreeVector fPos;
    G4double fEdep;

};

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

#endif
