#include "Trajectory.hh"
#include "TrackInformation.hh"

//for details examples/runandaction re01
Trajectory::Trajectory(const G4Track* aTrack) 
:G4VTrajectory()
{
    fParticleDefinition = aTrack->GetDefinition();
    fParticleName = fParticleDefinition->GetParticleName();
    fPDGCharge = fParticleDefinition->GetPDGCharge();
    fPDGEncoding = fParticleDefinition->GetPDGEncoding();

    fTrackID = aTrack->GetTrackID(); 
    fMomentum = aTrack->GetMomentum();
    fVertexPosition = aTrack->GetPosition(); 
    fGlobalTime = aTrack->GetGlobalTime();

    TrackInformation* trackInfo  = (TrackInformation*)(aTrack->GetUserInformation());
    fTrackStatus = trackInfo->GetTrackingStatus();
    if(fTrackStatus == 1) 
    { fParentID = aTrack->GetParentID(); }  
    else if(fTrackStatus == 2) 
    { fParentID = trackInfo->GetSourceTrackID(); }
    else
    { fParentID = -1; } 
//    fPositionRecord = new TrajectoryPointContainer();
//    fPositionRecord->push_back(new G4TrajectoryPoint(aTrack->GetPosition())); 


}

Trajectory::~Trajectory() 
{
}


void Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)  
{ 
  if(!secondTrajectory) return; 
  
  Trajectory* seco = (Trajectory*)secondTrajectory;            
  G4int ent = seco->GetPointEntries();                                 
  //
  // initial point of the second trajectory should not be merged       
  for(int i=1;i<ent;i++) 
  {                                                                    
    fPositionRecord->push_back((*(seco->fPositionRecord))[i]);         
  }
  delete (*seco->fPositionRecord)[0];
  seco->fPositionRecord->clear();
  
} 
