#ifndef PrimaryGeneratorActionMessenger_h
#define PrimaryGeneratorActionMessenger_h 1

#include "globals.hh" 
#include "G4UImessenger.hh"


#include "G4UIdirectory.hh" 
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh" 
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh" 
//#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"

class PrimaryGeneratorAction;

class PrimaryGeneratorActionMessenger: public G4UImessenger
{
    public:
        PrimaryGeneratorActionMessenger(PrimaryGeneratorAction* primGeneratorAction);
        ~PrimaryGeneratorActionMessenger();
        void SetNewValue(G4UIcommand*, G4String);

    private:
        void ChangeToBeam();
        void ChangeToRun();
        void ChangeToIsotope();

        bool CheckIfRun();
        PrimaryGeneratorAction* fPrimGen = nullptr;
        
        G4UIdirectory* fDirectory = nullptr;
        G4UIdirectory* fDirectoryRun = nullptr;
        G4UIcmdWithAString* fSourceType = nullptr;

        G4UIcmdWithADoubleAndUnit* fGammaBeamSetEnergy = nullptr;
        G4UIcmdWith3VectorAndUnit* fGammaBeamSetPosition = nullptr;
        G4UIcmdWith3VectorAndUnit* fGammaBeamSetMomentum = nullptr;


        G4UIcmdWithAString* fIsotopeSetShape = nullptr;
        G4UIcmdWithAnInteger* fIsotopeSetGenGammas = nullptr;
        G4UIcmdWithADoubleAndUnit* fIsotopeSetShapeDimCylinderRadius = nullptr; 
        G4UIcmdWithADoubleAndUnit* fIsotopeSetShapeDimCylinderZ = nullptr; 
        G4UIcmdWith3VectorAndUnit* fIsotopeSetCenter = nullptr;


        G4UIcmdWithAnInteger* fNemaPosition = nullptr;

        G4UIcmdWith3VectorAndUnit* fSetChamberCenter = nullptr;
        G4UIcmdWithADoubleAndUnit* fSetChamberEffectivePositronRadius = nullptr;

};

#endif
