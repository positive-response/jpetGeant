#include "EventAction.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"

#include "Trajectory.hh"
#include "G4TrajectoryContainer.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "DetectorHit.hh"


EventAction::EventAction(HistoManager* histo )
     :G4UserEventAction(), 
     fScinCollID(-1)
{

    fHisto = histo;
}

EventAction::~EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event*)
{

    G4SDManager * SDman = G4SDManager::GetSDMpointer();
    if(fScinCollID<0)
    {
        G4String colNam;
        fScinCollID = SDman->GetCollectionID(colNam="detectorCollection");
    }

     fHisto->Clear();
}


void EventAction::EndOfEventAction(const G4Event* anEvent)
{


     if(anEvent->GetNumberOfPrimaryVertex()==0) return;

     G4int id =  anEvent->GetEventID();
     fHisto->SetEventNumber(id);

     if ( id % 1000 == 0)
     {
         printf ("Processed %i events \n", id);
     }
     

     G4TrajectoryContainer* trajectoryContainer = anEvent->GetTrajectoryContainer();
     G4int trackNum = 0; 
     if (trajectoryContainer) trackNum = trajectoryContainer->entries();
     //G4cout << G4endl;
     //G4cout << "Trajectories in tracker "<< 
     //   "-------------------------------------------------" << G4endl;
     //G4cout << trackNum <<  G4endl;

     for (G4int i=0; i<trackNum; i++)
     {
         Trajectory* vec = (Trajectory*)((*( anEvent->GetTrajectoryContainer()))[i]); 
//         fHisto->GetDecayTree()->Fill(id,vec);
//         fHisto->FillTrk();
     }

    G4HCofThisEvent * HCE = anEvent->GetHCofThisEvent();
    DetectorHitsCollection* DHC = 0;
    if(HCE)
    {
        DHC = (DetectorHitsCollection*)(HCE->GetHC(fScinCollID));
        int n_hit = DHC->entries();
        //G4cout << "Detector hits "<< n_hit <<    
        //"-------------------------------------------------" << G4endl;
        for (int i=0; i<n_hit; i++)
        {
          fHisto->AddNewHit((DetectorHit*)DHC->GetHit(i) );
        }

    }


     if(anEvent->GetNumberOfPrimaryVertex()==1)
     {

        VtxInformation* info =  (VtxInformation*) anEvent->GetPrimaryVertex(0)->GetUserInformation();    
        if( info != 0 )
        {
            fHisto->AddGenInfo(info);
        }
     } else {
         printf(" number of vtx %i \n", anEvent->GetNumberOfPrimaryVertex());
         G4Exception("Event Action","EA01",JustWarning,
                 "Generated more than single vertex - genInfo has no meaning");
     }

    // save full information about event in final ntuples
    fHisto->SaveEvtPack();


}
