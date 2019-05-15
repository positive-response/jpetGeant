#ifndef DetectorSD_h
#define DetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "DetectorHit.h"


class DetectorSD : public G4VSensitiveDetector
{
    public:
        DetectorSD(G4String name, G4int scinSum, G4double timeMergeValue );
        virtual ~DetectorSD();
        virtual void Initialize(G4HCofThisEvent* HCE);

    private:

      struct HitParameters
      {
        public:
        G4int fID = -1;
        G4double fTime = 0.0;
      };

        G4double timeIntervals; 
        G4int totScinNum; 
        std::vector<HitParameters> previousHits;
        //std::vector<G4int> previousHitID;
        //std::vector<G4double> previousHitTime;
        //std::vector<G4double> previousHitsEnergySum;

        DetectorHitsCollection *fDetectorCollection;


    protected:
         virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);


};


#endif
