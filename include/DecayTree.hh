#ifndef DecayTree_h
#define DecayTree_h

#include "globals.hh"
#include "TObject.h"
#include "G4ThreeVector.hh"
#include <vector>
#include "Trajectory.hh"
#include "TVector3.h" 
#include "G4SystemOfUnits.hh"



class DecayTree : public TObject 
{
 
    public:
        DecayTree();
        ~DecayTree();
        void Clean();
        void Fill(G4int id, Trajectory* traj);


    private:
     G4int                          fEvtID;
     G4int                        fTrackID;                           
     G4int                        fParentID;                     
//     G4String                     fParticleName;                    
     G4double                     fPDGCharge;                        
     G4int                        fPDGEncoding;                      
     TVector3                     fVtxMomentum;                 
     TVector3                     fVtxPosition;        
     G4double                     fTime; 
     G4int                        fMaterial;


      ClassDef(DecayTree,1)

};

#endif
