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
    void SetTime(G4double val) {fTime = val;}
    void SetTrackID(G4int i) { fTrackID =i;}
    void SetPosition(G4ThreeVector xyz) {fPos =xyz;}
    void SetScinID(G4int i) {fScinID = i;}

    G4int GetScinID() {return fScinID;}
    G4int GetTrackID() {return fTrackID;}
    G4double GetEdep() {return fEdep;}
    G4double GetTime() {return fTime;}
    G4ThreeVector GetPosition() {return fPos;}
    private:
    G4int fScinID;
    G4int fTrackID;
    G4double fEdep;
    G4double fTime;
    G4ThreeVector fPos;

};

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

#endif
