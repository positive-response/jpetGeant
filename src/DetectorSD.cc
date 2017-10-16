#include "DetectorSD.hh"
#include "G4VProcess.hh"

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

    previousScinID = -1;
    previousTime = -1;
    lastHitID = -1;
}


G4bool DetectorSD::ProcessHits(G4Step* aStep, G4TouchableHistory* )
{
    G4double edep = aStep->GetTotalEnergyDeposit()/CLHEP::keV;
    if(edep==0.) return false;

    G4TouchableHistory* theTouchable  = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()); 
    G4VPhysicalVolume* physVol = theTouchable->GetVolume();
    G4int   currentScinID = physVol->GetCopyNo();
    G4double currentTime = aStep->GetPreStepPoint()->GetGlobalTime()/CLHEP::ns;


    if( (currentScinID == previousScinID) && ( abs(previousTime-currentTime) < timeIntervals) ) 
    {
        // update track
        (*fDetectorCollection)[lastHitID]->AddEdep(edep);
        (*fDetectorCollection)[lastHitID]->AddInteraction();

    } else {
        // new hit
        DetectorHit* newHit = new DetectorHit();
        newHit->SetEdep( edep );
        newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
        newHit->SetTrackPDG(aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
        //newHit->SetProcessName(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
        newHit->SetInteractionNumber();
        newHit->SetPosition(aStep->GetPreStepPoint()->GetPosition()/CLHEP::cm);
        newHit->SetTime(currentTime);

        newHit->SetScinID(physVol->GetCopyNo());

        newHit->SetPolarizationIn(aStep->GetPreStepPoint()->GetPolarization());
        newHit->SetPolarizationOut(aStep->GetPostStepPoint()->GetPolarization());
        newHit->SetMomentumIn(aStep->GetPreStepPoint()->GetMomentum()/CLHEP::keV);
        newHit->SetMomentumOut(aStep->GetPostStepPoint()->GetMomentum()/CLHEP::keV);

        G4int id = fDetectorCollection->insert(newHit);
        lastHitID = id -1;
    }

    previousScinID = currentScinID;
    previousTime = currentTime;





    return true;
}


