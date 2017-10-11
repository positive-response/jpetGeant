#include "DetectorSD.hh"

DetectorSD::DetectorSD(G4String name)
    :G4VSensitiveDetector(name)
{
     G4String HCname;
     collectionName.insert(HCname="detectorCollection");
}

DetectorSD::~DetectorSD()
{}

void DetectorSD::Initialize(G4HCofThisEvent* HCE)
{
    static int HCID = -1;
    fDetectorCollection = new DetectorHitsCollection(SensitiveDetectorName,collectionName[0]);

    if(HCID<0)
    { HCID = GetCollectionID(0); }
    HCE->AddHitsCollection(HCID,fDetectorCollection);
}


G4bool DetectorSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    G4double edep = aStep->GetTotalEnergyDeposit();
    if(edep==0.) return false;

    DetectorHit* newHit = new DetectorHit();
    newHit->SetEdep( edep );

    return true;
}


