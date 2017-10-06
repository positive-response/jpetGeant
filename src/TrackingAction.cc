#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "TrackingAction.hh"

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
// Create trajectory only for primaries 
//    fpTrackingManager->SetStoreTrajectory(true); 
    if(aTrack->GetParentID()==0)
    { fpTrackingManager->SetStoreTrajectory(true); }
    else
    { fpTrackingManager->SetStoreTrajectory(false); }
 }

