#ifndef DetectorConstructionMessenger_h
#define DetectorConstructionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "DetectorConstruction.h"
#include "G4UIdirectory.hh" 
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"


class DetectorConstructionMessenger: public G4UImessenger 
{
    public:
        DetectorConstructionMessenger(DetectorConstruction* detector);
        ~DetectorConstructionMessenger();
        void SetNewValue(G4UIcommand*, G4String); 
        const int kDefaultRunNumber = 5;

    private:
        DetectorConstruction* fDetector= nullptr;

        G4UIdirectory* fDirectory = nullptr;
        G4UIcmdWithAnInteger* fLoadGeomForRun= nullptr;
        G4UIcmdWithAnInteger* fLoadIdealGeometry= nullptr; 


        G4UIcmdWithoutParameter* fLoadJPetBasicGeometry= nullptr;
        G4UIcmdWithoutParameter* fLoadJPetExtendedGeometry= nullptr;
        G4UIcmdWithoutParameter* fLoadOnlyScintillators= nullptr;
        G4UIcmdWithABool* fLoadModularLayer= nullptr; 

        G4UIcmdWithADoubleAndUnit* fScinHitMergingTime= nullptr;

};

#endif
