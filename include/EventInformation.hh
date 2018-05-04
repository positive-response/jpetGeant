#ifndef EVENT_INFORMATION_H
#define EVENT_INFORMATION_H

#include "G4VUserEventInformation.hh" 
#include "globals.hh"

class EventInformation : public G4VUserEventInformation {
    public:
        EventInformation();
        ~EventInformation(){};
        void Clear();

        void SetThreeGammaGen(G4bool tf){fThreeGammaGen=tf;};
        void SetTwoGammaGen(G4bool tf){fTwoGammaGen=tf;};
        void SetPromptGammaGen(G4bool tf){fPromptGammaGen=tf;};
        void SetRunNr(G4int x){fnRun =x;};

        G4bool SetThreeGammaGen(){return fThreeGammaGen;};
        G4bool SetTwoGammaGen(){return fTwoGammaGen;};
        G4bool SetPromptGammaGen(){return fPromptGammaGen;};
        G4int SetRunNr(){return fnRun;};


    private:
        G4bool fTwoGammaGen;
        G4bool fThreeGammaGen;
        G4bool fPromptGammaGen;
        G4int fnRun;

};



#endif
