#include "EventAction.h"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"

#include "Trajectory.h"
#include "G4TrajectoryContainer.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "DetectorHit.h"
#include "PrimaryParticleInformation.h"

bool EventAction::is2gRec=false;
bool EventAction::is3gRec=false;


EventAction::EventAction(HistoManager* histo )
     :G4UserEventAction(), 
     fScinCollID(-1)
{

    fHisto = histo;
}

EventAction::~EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event* anEvent)
{

    G4SDManager * SDman = G4SDManager::GetSDMpointer();
    if(fScinCollID<0)
    {
        G4String colNam;
        fScinCollID = SDman->GetCollectionID(colNam="detectorCollection");
    }

     fHisto->Clear();

     G4int id =  anEvent->GetEventID();
     fHisto->SetEventNumber(id);
     fHisto->FillHistoGenInfo(anEvent);


}


void EventAction::EndOfEventAction(const G4Event* anEvent)
{
  
  if(anEvent->GetNumberOfPrimaryVertex()==0) return;

  if(fEvtMessenger->SaveOnlySelectedEvents())
  {
    // check evts
    if(fEvtMessenger->Save2gEvents()){ CheckIf2gIsRegistered(anEvent);}
    if(fEvtMessenger->Save3gEvents()){ CheckIf3gIsRegistered(anEvent);}
    if ( !(Is2gRegistered()||Is3gRegistered())) {return;}
  }
  WriteToFile(anEvent);

}



void EventAction::WriteToFile(const G4Event* anEvent)
{


    G4HCofThisEvent * HCE = anEvent->GetHCofThisEvent();
    DetectorHitsCollection* DHC = 0;
    if(HCE)
    {
        DHC = dynamic_cast<DetectorHitsCollection*>(HCE->GetHC(fScinCollID));
        int n_hit = DHC->entries();
        for (int i=0; i<n_hit; i++)
        {
           DetectorHit* dh =  dynamic_cast<DetectorHit*>(DHC->GetHit(i));
           fHisto->AddNewHit(dh);
        }
    }


    // save full information about event in final ntuples
    fHisto->SaveEvtPack();
}


bool EventAction::Is2gRegistered()
{
  return is2gRec;
}

bool EventAction::Is3gRegistered()
{
  return is3gRec;
}

void EventAction::CheckIf3gIsRegistered(const G4Event* anEvent)
{
  bool isGenerated = false;
  bool isReconstructed = true;
  is3gRec = false;
  std::vector<bool> isGammaRec{false,false,false,false};

  for( int i=0; i<anEvent->GetNumberOfPrimaryVertex(); i++)
  {
    VtxInformation* info =  dynamic_cast<VtxInformation*>( anEvent->GetPrimaryVertex(i)->GetUserInformation());    
    if( info != nullptr )
    {
      isGenerated = isGenerated || info->GetThreeGammaGen();
    }
  }

    if (!isGenerated){ return;}

    G4HCofThisEvent * HCE = anEvent->GetHCofThisEvent();
    DetectorHitsCollection* DHC = 0;
    if(HCE)
    {
        DHC = dynamic_cast<DetectorHitsCollection*>(HCE->GetHC(fScinCollID));
        int n_hit = DHC->entries();
        if(n_hit<3) return;

        for (int i=0; i<n_hit; i++)
        {
           DetectorHit* dh =  dynamic_cast<DetectorHit*>(DHC->GetHit(i));
           if ( dh->GetGenGammaMultiplicity() == 3 )
           {
             isGammaRec[dh->GetGenGammaIndex()] = true;
           }
        }
    }



  for(int i=1; i<=3; i++)
  {
    isReconstructed = isReconstructed && isGammaRec[i];
  }

  is3gRec = isReconstructed;



}




void EventAction::CheckIf2gIsRegistered(const G4Event* anEvent)
{
  bool isGenerated = false;
  bool isReconstructed = true;
  is2gRec = false;
  std::vector<bool> isGammaRec{false,false,false};

  for( int i=0; i<anEvent->GetNumberOfPrimaryVertex(); i++)
  {
    VtxInformation* info =  dynamic_cast<VtxInformation*>( anEvent->GetPrimaryVertex(i)->GetUserInformation());    
    if( info != nullptr )
    {
      isGenerated = isGenerated || info->GetTwoGammaGen();
    }
  }

    if (!isGenerated){ return;}

    G4HCofThisEvent * HCE = anEvent->GetHCofThisEvent();
    DetectorHitsCollection* DHC = 0;
    if(HCE)
    {
        DHC = dynamic_cast<DetectorHitsCollection*>(HCE->GetHC(fScinCollID));
        int n_hit = DHC->entries();
        if(n_hit<2) return;

        for (int i=0; i<n_hit; i++)
        {
           DetectorHit* dh =  dynamic_cast<DetectorHit*>(DHC->GetHit(i));
           if ( dh->GetGenGammaMultiplicity() == 2 )
           {
             isGammaRec[dh->GetGenGammaIndex()] = true;
           }
        }
    }



  for(int i=1; i<=2; i++)
  {
    isReconstructed = isReconstructed && isGammaRec[i];
  }

  is2gRec = isReconstructed;


}




