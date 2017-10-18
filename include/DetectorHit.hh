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
    void SetTrackPDG(G4int i) {fTrackPDG = i;}
    void SetScinID(G4int i) {fScinID = i;}
    void SetPosition(G4ThreeVector xyz) {fPos =xyz;}
    void SetInteractionNumber() {fNumInteractions = 1;}
    void AddInteraction() { fNumInteractions += 1;}
    void AddEdep(G4double de) { fEdep += de;}

    void SetPolarizationIn(G4ThreeVector xyz) {fPolarizationIn =xyz;}
    void SetPolarizationOut(G4ThreeVector xyz) {fPolarizationOut =xyz;}
    void SetMomentumIn(G4ThreeVector xyz)  {fMomentumIn =xyz;}
    void SetMomentumOut(G4ThreeVector xyz) {fMomentumOut =xyz;}
    void SetProcessName(G4String str) {fName = str;}


    G4int GetScinID() {return fScinID;}
    G4int GetTrackID() {return fTrackID;}
    G4double GetEdep() {return fEdep;}
    G4double GetTime() {return fTime;}
    G4ThreeVector GetPosition() {return fPos;}
    G4int GetTrackPDG() {return fTrackPDG;}
    G4ThreeVector GetPolarizationIn()  {return fPolarizationIn;}
    G4ThreeVector GetPolarizationOut() {return fPolarizationOut;}
    G4ThreeVector GetMomentumIn()  {return fPolarizationIn;}
    G4ThreeVector GetMomentumOut() {return fPolarizationOut;}
    G4int GetNumInteractions() {return fNumInteractions;}
    G4String GetProcessName() {return fName;}

    private:
    G4int fScinID;
    G4int fTrackID;
    G4int fTrackPDG;
    G4double fEdep;
    G4double fTime;
    G4ThreeVector fPos;
    G4int fNumInteractions;
    G4ThreeVector fPolarizationIn;
    G4ThreeVector fPolarizationOut;
    G4ThreeVector fMomentumIn;
    G4ThreeVector fMomentumOut;
    G4String fName;


};

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

#endif
