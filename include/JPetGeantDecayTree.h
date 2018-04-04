#ifndef JPetGeantDecayTree_h
#define JPetGeantDecayTree_h

#include "globals.hh"
#include "TObject.h"
#include <vector>
#include "TVector3.h" 
#include "TBits.h"



class JPetGeantDecayTree : public TObject 
{
 
    public:
        JPetGeantDecayTree();
        ~JPetGeantDecayTree();
        void Clean();

        //setters
        void SetEvtID(int x){fEvtID=x;};
        void SetTrackID(int x){fTrackID=x;};
        void SetPDG(int x){fPDGEncoding=x;};
        void SetMomentumAtVtx(TVector3 x){fVtxMomentum=x;};
        void SetMomentumAtVtx(float x, float y, float z){fVtxMomentum.SetXYZ(x,y,z);};
        void SetPolarizationAtVtx(TVector3 x){fVtxPolarization=x;};
        void SetPolarizationAtVtx(float x, float y, float z){fVtxPolarization.SetXYZ(x,y,z);};
        void SetParentTrackID(int x){fParentTrackID=x;};
        void SetTimeAtVtx(float x){fTime=x;};
        void SetVtxPosition(TVector3 x){fVtxPosition=x;};
        void SetVtxPosition(float x, float y, float z){fVtxPosition.SetXYZ(x,y,z);};

        //getters
        int GetEvtID(){return fEvtID;};
        int GetTrackID(){return fTrackID;};
        int GetPDG(){return fPDGEncoding;};
        int  GetParentTrackID(){return fParentTrackID;};
        float GetTimeAtVtx(){return fTime;};
        TVector3 GetMomentumAtVtx(){return fVtxMomentum;};
        TVector3 GetPolarizationAtVtx(){return fVtxPolarization;};
        TVector3 GetVtxPosition(){return fVtxPosition;};

    private:
     //general
     int                        fEvtID;

     // track description 
     int                        fTrackID;                           
     int                        fPDGEncoding;                      
     TVector3                   fVtxMomentum;                 
     TVector3                   fVtxPolarization;                 

     // vertex description
     int                        fParentTrackID;                     
     float                      fTime; 
     TVector3                   fVtxPosition;        


     ClassDef(JPetGeantDecayTree,1)

};

#endif
