#include "TrkBlock.hh"


TrkBlock::TrkBlock()
{

}

TrkBlock::~TrkBlock()
{}

void TrkBlock::Clean()
{
      fTrackID        =   0;                           
      fParentID       =   0;                     
//      fParticleName   = "none";                    
      fPDGCharge      = 0.0;                        
      fPDGEncoding    = 0;                      
      fTime           = 0; 
      fMaterial       = 0;

//     G4ThreeVector  fMomentum   ;                          
//     G4ThreeVector  fVertexPosition;                    

}

void TrkBlock::Fill(Trajectory* traj)
{
//    G4cout << "trackid" <<  traj->GetTrackID()  <<  G4endl;
      fTrackID        = traj->GetTrackID() ;                           
//    G4cout << "parentid" <<  traj->GetParentID()  <<  G4endl;
      fParentID       = traj->GetParentID()  ;                     
//////      fParticleName   = traj->GetParticleName();                    
//    G4cout << "charge:" << traj->GetCharge()  <<  G4endl;
      fPDGCharge      = traj->GetCharge();                        
//
//    G4cout << "fPDGCharge:"  <<  G4endl;
      fPDGEncoding    = traj->GetPDGEncoding();                      
//    G4cout << "time:"  <<  G4endl;
      fTime           = traj->GetTime(); 
//    G4cout << "material:" << traj->GetCharge()  <<  G4endl;
//////      G4ThreeVector  fMomentum = traj->GetInitialMomentum()  ;                          
//////      G4ThreeVector  fVertexPosition = traj->GetVertexPosition();                    
//
      fMaterial       = 0;
}
