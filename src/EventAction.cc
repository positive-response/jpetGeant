#include "EventAction.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"

#include "Trajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4EventManager.hh"


EventAction::EventAction(HistoManager* histo )
     :G4UserEventAction(), 
     fScinCollID(-1)
{

    fHisto = histo;
}

EventAction::~EventAction()
{
}


void EventAction::BeginOfEventAction(const G4Event*)
{
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
//     G4PrimaryParticle* primary = anEvent->GetPrimaryVertex(0)->GetPrimary(0);

     G4int id =  anEvent->GetEventID();
     
     // save information about all primary particles 

     G4TrajectoryContainer* trajectoryContainer = anEvent->GetTrajectoryContainer();
     G4int trackNum = 0; 
     if (trajectoryContainer) trackNum = trajectoryContainer->entries();
     G4cout << G4endl;
     G4cout << "Trajectories in tracker "<< 
        "-------------------------------------------------" << G4endl;
     G4cout << trackNum <<  G4endl;

     for (G4int i=0; i<trackNum; i++)
     {

         Trajectory* vec = (Trajectory*)((*( anEvent->GetTrajectoryContainer()))[i]); 
         fHisto->GetTrkBlock()->Fill(id,vec);
         fHisto->FillTrk();
     }

//     Trajectory* vec = (Trajectory*)((*( anEvent->GetTrajectoryContainer()))[0]); 
//     pdgNumber = vec->GetTrackID();



}
