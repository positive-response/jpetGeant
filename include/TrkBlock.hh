#ifndef TrkBlock_h
#define TrkBlock_h

#include "globals.hh"
#include "TObject.h"
#include "G4ThreeVector.hh"
#include <vector>
#include "Trajectory.hh"

class TrkBlock : public TObject 
{

    public:
        TrkBlock();
        ~TrkBlock();
        void Clean();
        void Fill(Trajectory* traj);


    private:
     G4int                        fTrackID;                           
     G4int                        fParentID;                     
//     G4String                     fParticleName;                    
     G4double                     fPDGCharge;                        
     G4int                        fPDGEncoding;                      
//     G4ThreeVector                fMomentum;                 
//     G4ThreeVector                fVertexPosition;        
     G4double                     fTime; 
     G4int                        fMaterial;


        ClassDef(TrkBlock,1)

};

#endif
