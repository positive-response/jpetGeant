#ifndef MaterialExtensionMessenger_h
#define MaterialExtensionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include "G4UIdirectory.hh" 
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh" 
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"


class MaterialExtension;

class MaterialExtensionMessenger: public G4UImessenger 
{
    public:
        static MaterialExtensionMessenger* GetMaterialExtensionMessenger();
        void SetNewValue(G4UIcommand*, G4String); 
        bool GenerateOnly3g(){return fIsOnly3g;}


    private:
        static MaterialExtensionMessenger* fInstance;
        MaterialExtensionMessenger();
        ~MaterialExtensionMessenger();

        G4UIdirectory* fDirectory;
        MaterialExtension* fMaterial;
        
        G4UIcmdWithoutParameter* fXAD3GammaOnly;
        G4UIcmdWithoutParameter* fXAD2GammaOnly;
        G4UIcmdWithoutParameter* fXADPickOffOnly;
        G4UIcmdWithADoubleAndUnit* fXADSet3gLifetime;
        G4UIcmdWithADouble* fXADSet3gFraction;
        G4UIcmdWithADouble* fXADSetPickOffFraction;

        //G4UIcmdWithABool* fKaptonNoAnnihilations;
        //G4UIcmdWithABool* fKapton3GammaOnly;
        //G4UIcmdWithABool* fKapton2GammaOnly;
        //G4UIcmdWithADoubleAndUnit* fKaptonSet2gLifetime;
        //G4UIcmdWithADoubleAndUnit* fKaptonSet3gLifetime;

        //G4UIcmdWithABool* fChamberRun5NoAnnihilations;
        //G4UIcmdWithABool* fChamberRun53GammaOnly;
        //G4UIcmdWithABool* fChamberRun52GammaOnly;
        //G4UIcmdWithADoubleAndUnit* fChamberRun5Set2gLifetime;
        //G4UIcmdWithADoubleAndUnit* fChamberRun5Set3gLifetime;

        //G4UIcmdWithABool* fChamberRun6NoAnnihilations;
        //G4UIcmdWithABool* fChamberRun63GammaOnly;
        //G4UIcmdWithABool* fChamberRun62GammaOnly;
        //G4UIcmdWithADoubleAndUnit* fChamberRun6Set2gLifetime;
        //G4UIcmdWithADoubleAndUnit* fChamberRun6Set3gLifetime;

        //G4UIcmdWithABool* fAlNoAnnihilations;
        //G4UIcmdWithABool* fAl3GammaOnly;
        //G4UIcmdWithABool* fAl2GammaOnly;
        //G4UIcmdWithADoubleAndUnit* fAlSet2gLifetime;
        //G4UIcmdWithADoubleAndUnit* fAlSet3gLifetime;




        bool fIsOnly3g = true;
};

#endif
