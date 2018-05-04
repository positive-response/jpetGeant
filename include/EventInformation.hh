#ifndef EVENT_INFORMATION_H
#define EVENT_INFORMATION_H

#include "G4VUserEventInformation.hh" 
#include "globals.hh"
#include "G4ThreeVector.hh"

class EventInformation : public G4VUserEventInformation {
    public:
        EventInformation();
        virtual ~EventInformation();
        void Clear();
        virtual void Print() const;

        void SetThreeGammaGen(G4bool tf){fThreeGammaGen=tf;};
        void SetTwoGammaGen(G4bool tf){fTwoGammaGen=tf;};
        void SetPromptGammaGen(G4bool tf){fPromptGammaGen=tf;};
        void SetRunNr(G4int x){fnRun =x;};
        void SetVtxPosition(G4double x, G4double y, G4double z);
        void SetLifetime(G4double x){printf("lifetime %f\n",x); fLifetime=x;  printf("lifetime FIN %f\n",x); };

        G4bool GetThreeGammaGen(){return fThreeGammaGen;};
        G4bool GetTwoGammaGen(){return fTwoGammaGen;};
        G4bool GetPromptGammaGen(){return fPromptGammaGen;};
        G4int  GetRunNr(){return fnRun;};
        G4double GetVtxPositionX(){return fVtxPosition.x();};
        G4double GetVtxPositionY(){return fVtxPosition.y();};
        G4double GetVtxPositionZ(){return fVtxPosition.z();};
        G4double GetLifetime(){return fLifetime;};




    private:
        G4ThreeVector fVtxPosition;
        G4bool fTwoGammaGen;
        G4bool fThreeGammaGen;
        G4bool fPromptGammaGen;
        G4int fnRun;
        G4double fLifetime;

};



#endif
