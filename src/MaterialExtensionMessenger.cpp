#include "MaterialExtensionMessenger.h"
#include "MaterialParameters.h"
#include "DetectorConstruction.h"

using namespace MaterialParameters;

MaterialExtensionMessenger*  MaterialExtensionMessenger::fInstance = nullptr;

MaterialExtensionMessenger* MaterialExtensionMessenger::GetMaterialExtensionMessenger()
{
  if ( fInstance == nullptr ){
   fInstance = new MaterialExtensionMessenger();
  }

  return fInstance;
}


MaterialExtensionMessenger::MaterialExtensionMessenger()
{

  fDirectory = new G4UIdirectory("/jpetmc/material/"); 
  fDirectory->SetGuidance("Commands for controling the geometry materials");

  fDirectory = new G4UIdirectory("/jpetmc/material/xad/"); 
  fDirectory->SetGuidance("Commands for controling the XAD material properties");

//  fDirectory = new G4UIdirectory("/jpetmc/material/kapton/"); 
//  fDirectory->SetGuidance("Commands for controling the kapton material properties");
//
//  fDirectory = new G4UIdirectory("/jpetmc/material/chamberRun5/"); 
//  fDirectory->SetGuidance("Commands for controling the kapton material properties");
//

  fXAD3GammaOnly = new G4UIcmdWithoutParameter("/jpetmc/material/xad/threeGammaOnly",this);
  fXAD3GammaOnly->SetGuidance("Only 3 gamma events will be generated"); 

  fXAD2GammaOnly = new G4UIcmdWithoutParameter("/jpetmc/material/xad/twoGammaOnly",this);
  fXAD2GammaOnly->SetGuidance("Only 2 gamma events will be generated"); 

  fXADPickOffOnly = new G4UIcmdWithoutParameter("/jpetmc/material/xad/pickOffOnly",this);
  fXADPickOffOnly->SetGuidance("Only 2 gamma events from pick-off/conversion will be generated (lifetime as for 3g)"); 


  fXADSet3gLifetime = new G4UIcmdWithADoubleAndUnit("/jpetmc/material/xad/oPslifetime",this);
  fXADSet3gLifetime->SetGuidance("Set mean lifetime for 3g events");
  fXADSet3gLifetime->SetDefaultValue(2.45);
  fXADSet3gLifetime->SetDefaultUnit("ns");
  fXADSet3gLifetime->SetUnitCandidates("ns");

  fXADSet3gFraction = new G4UIcmdWithADouble("/jpetmc/material/xad/oPsFraction",this);
  fXADSet3gFraction->SetGuidance("Set fraction of oPs events (0-1)");
  fXADSet3gFraction->SetDefaultValue(0.1);

  fXADSetPickOffFraction = new G4UIcmdWithADouble("/jpetmc/material/xad/pickOffFraction",this);
  fXADSetPickOffFraction->SetGuidance("Set fraction of pickOff events (0-1)");
  fXADSetPickOffFraction->SetDefaultValue(0.1);



}

MaterialExtensionMessenger::~MaterialExtensionMessenger()
{
  delete fXAD3GammaOnly;
  delete fXAD2GammaOnly;
  delete fXADPickOffOnly;
  delete fXADSet3gLifetime;
  delete fXADSet3gFraction;
  delete fXADSetPickOffFraction;
}

void MaterialExtensionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if(command==fXAD3GammaOnly) 
  {
    SetXADoPsOnly();  
    DetectorConstruction::GetInstance()->ReloadMaterials();
  }

  if(command==fXAD2GammaOnly) 
  { 
    SetXAD2gOnly();  
    DetectorConstruction::GetInstance()->ReloadMaterials();
  }

  if(command==fXADPickOffOnly) 
  { 
    SetXADPickOffOnly(); 
    DetectorConstruction::GetInstance()->ReloadMaterials();
  }

  if(command==fXADSet3gLifetime)
  {
    SetXADoPsLifetime(fXADSet3gLifetime->GetNewDoubleValue(newValue));
    DetectorConstruction::GetInstance()->ReloadMaterials();
  }

  if(command==fXADSet3gFraction)
  {
    G4double frac =  fXADSet3gFraction->GetNewDoubleValue(newValue);
    if( (frac > 1.0) || (frac < 0.0) )
    {
      G4Exception ("MaterialMessenger", "MP01",JustWarning,
      "Fraction should be between 0 and 1 ! ");
    } else {
      SetXADoPsFraction(frac);
      DetectorConstruction::GetInstance()->ReloadMaterials();
    }
  }


  if(command==fXADSetPickOffFraction)
  {
    G4double frac =  fXADSetPickOffFraction->GetNewDoubleValue(newValue);
    if( (frac > 1.0) || (frac < 0.0) )
    {
      G4Exception ("MaterialMessenger", "MP01",JustWarning,
      "Fraction should be between 0 and 1 ! ");
    } else {
      SetXADPickOffFraction(frac);
      DetectorConstruction::GetInstance()->ReloadMaterials();
    }
  }
}

