#include "EventAction.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"

#include "Trajectory.hh"
#include "G4TrajectoryContainer.hh"


EventAction::EventAction(HistoManager* histo )
{

    fHisto = histo;
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fEvent->Clean();
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
//     G4PrimaryParticle* primary = anEvent->GetPrimaryVertex(0)->GetPrimary(0);

     // save information about all primary particles 
     const G4int vtxNum  = anEvent->GetNumberOfPrimaryVertex();
     

     G4PrimaryVertex* primVtx = anEvent->GetPrimaryVertex(0);
     G4int particlesNum =  primVtx->GetNumberOfParticle();

     G4PrimaryParticle* particle = primVtx->GetPrimary(0);
     G4int pdgNumber = particle->GetG4code()->GetPDGEncoding();
     
     G4double x =  primVtx->GetX0();

     G4TrajectoryContainer* trajectoryContainer = anEvent->GetTrajectoryContainer();
     G4int trackNum = 0; 
     if (trajectoryContainer) trackNum = trajectoryContainer->entries();
     G4cout << G4endl;
     G4cout << "Trajectories in tracker "<< 
        "-------------------------------------------------" << G4endl;
     G4cout << trackNum <<  G4endl;



//     Trajectory* vec = (Trajectory*)((*( anEvent->GetTrajectoryContainer()))[0]); 
//     pdgNumber = vec->GetTrackID();


     fHisto->GetEvent()->GetVtxBlock()->SetMotherId(pdgNumber);
     fHisto->GetEvent()->GetVtxBlock()->SetTime(30.2);


     fHisto->FillTree();

}
