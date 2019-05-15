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

        G4UIdirectory* fDirectory = nullptr;
        MaterialExtension* fMaterial = nullptr;
        
        G4UIcmdWithoutParameter* fXAD3GammaOnly = nullptr;
        G4UIcmdWithoutParameter* fXAD2GammaOnly = nullptr;
        G4UIcmdWithoutParameter* fXADPickOffOnly = nullptr;
        G4UIcmdWithADoubleAndUnit* fXADSet3gLifetime = nullptr;
        G4UIcmdWithADouble* fXADSet3gFraction = nullptr;
        G4UIcmdWithADouble* fXADSetPickOffFraction = nullptr;


        bool fIsOnly3g = true;
};

#endif
