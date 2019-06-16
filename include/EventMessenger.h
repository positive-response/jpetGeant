#ifndef EventMessenger_h
#define EventMessenger_h 1

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


class EventMessenger: public G4UImessenger 
{
    public:
        static EventMessenger* GetEventMessenger();
        void SetNewValue(G4UIcommand*, G4String); 
        bool SaveOnlySelectedEvents(){return fSave2gEvts||fSave3gEvts;}
        bool Save2gEvents(){return fSave2gEvts;}
        bool Save3gEvents(){return fSave3gEvts;}
        bool PrintStatistics(){return fPrintStatistics;}
        bool ShowProgress(){return fShowProgress;}
        G4int GetPowerPrintStat(){return fPrintPower;}

    private:
        static EventMessenger* fInstance;
        EventMessenger();
        ~EventMessenger();

        G4UIdirectory* fDirectory = nullptr;
        bool fSave2gEvts = false;
        bool fSave3gEvts = false;
        G4UIcmdWithoutParameter* fSave2g;
        G4UIcmdWithoutParameter* fSave3g;
        G4UIcmdWithABool* fPrintStat;
        G4UIcmdWithABool* fPrintStatBar;
        G4UIcmdWithAnInteger* fPrintStatPower;
        bool fPrintStatistics = false;
        G4int fPrintPower = 10;
        bool fShowProgress = true;

};

#endif
