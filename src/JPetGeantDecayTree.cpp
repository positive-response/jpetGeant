#include "JPetGeantDecayTree.h"

ClassImp(JPetGeantDecayTree)


JPetGeantDecayTree::JPetGeantDecayTree()
{
}

JPetGeantDecayTree::~JPetGeantDecayTree()
{}

void JPetGeantDecayTree::Clean()
{
      fEvtID        = -1;
      fTrackID        =   -1;                           
//      fParticleName   = "none";                    
      fPDGEncoding    = 0;                      
      fTime           = 0; 

//     G4ThreeVector  fMomentum   ;                          
//     G4ThreeVector  fVertexPosition;                    

}

//void JPetGeantDecayTree::Fill(int id, Trajectory* traj)
//{
//      fEvtID = id;
////    G4cout << "trackid" <<  traj->GetTrackID()  <<  G4endl;
//      fTrackID        = traj->GetTrackID() ;                           
////    G4cout << "parentid" <<  traj->GetParentID()  <<  G4endl;
////      fParentID       = traj->GetParentID()  ;                     
////////      fParticleName   = traj->GetParticleName();                    
////    G4cout << "charge:" << traj->GetCharge()  <<  G4endl;
//      fPDGCharge      = traj->GetCharge();                        
////
////    G4cout << "fPDGCharge:"  <<  G4endl;
//      fPDGEncoding    = traj->GetPDGEncoding();                      
////    G4cout << "time:"  <<  G4endl;
//      fTime           = traj->GetTime(); 
////    G4cout << "material:" << traj->GetCharge()  <<  G4endl;
//      G4ThreeVector  mom = traj->GetInitialMomentum();
//      fVtxMomentum.SetXYZ(mom.x(),mom.y(),mom.z()); 
//
//      G4ThreeVector vtx = traj->GetVertexPosition(); 
//      fVtxPosition.SetXYZ(vtx.x(),vtx.y(),vtx.z());                 
////
//      fMaterial       = 0;
//}
