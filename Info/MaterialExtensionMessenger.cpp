/**
 *  @copyright Copyright 2019 The J-PET Monte Carlo Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file MaterialExtensionMessenger.cpp
 */

#include "../Core/DetectorConstruction.h"
#include "../Core/MaterialParameters.h"
#include "MaterialExtensionMessenger.h"

MaterialExtensionMessenger*  MaterialExtensionMessenger::fInstance = nullptr;

MaterialExtensionMessenger* MaterialExtensionMessenger::GetMaterialExtensionMessenger()
{
  if ( fInstance == nullptr ) 
  {
    fInstance = new MaterialExtensionMessenger();
  }
  return fInstance;
}

MaterialExtensionMessenger::MaterialExtensionMessenger()
{
  fDirectory = new G4UIdirectory("/jpetmc/material/");
  fDirectory->SetGuidance("Commands for controling the geometry materials");

  fDirectory = new G4UIdirectory("/jpetmc/material/xad4/");
  fDirectory->SetGuidance("Commands for controling the XAD4 material properties");

  f3GammaOnly = new G4UIcmdWithoutParameter("/jpetmc/material/threeGammaOnly", this);
  f3GammaOnly->SetGuidance("Only 3 gamma events will be generated (lifetime as for oPs)");

  f2GammaOnly = new G4UIcmdWithoutParameter("/jpetmc/material/twoGammaOnly", this);
  f2GammaOnly->SetGuidance("Only 2 gamma events will be generated (lifetime as for pPs)");

  fPickOffOnly = new G4UIcmdWithoutParameter("/jpetmc/material/pickOffOnly", this);
  fPickOffOnly->SetGuidance("Only 2 gamma events from pick-off/conversion will be generated (lifetime as for 3g)");

  fXADAdd_oPsComponent = new G4UIcmdWithAString("/jpetmc/material/xad4/oPsComponent", this);
  fXADAdd_oPsComponent->SetGuidance("Adding oPs annihilation component with mean lifetime and intensity");
  
  fXADSet_pPsComponent = new G4UIcmdWithAString("/jpetmc/material/xad4/pPsComponent", this);
  fXADSet_pPsComponent->SetGuidance("Setting pPs annihilation component to mean lifetime and intensity");
  
  fXADAdd_directComponent = new G4UIcmdWithAString("/jpetmc/material/xad4/directComponent", this);
  fXADAdd_directComponent->SetGuidance("Adding direct positron annihilation component with mean lifetime and intensity");
  
  fReloadMaterials = new G4UIcmdWithAString("/jpetmc/material/reloadMaterials", this);
  fReloadMaterials->SetGuidance("Reloading material for parameters given by user");
}

MaterialExtensionMessenger::~MaterialExtensionMessenger()
{
  delete f3GammaOnly;
  delete f2GammaOnly;
  delete fPickOffOnly;
  delete fXADAdd_oPsComponent;
  delete fXADSet_pPsComponent;
  delete fXADAdd_directComponent;
}

void MaterialExtensionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == f3GammaOnly)
    MaterialParameters::SetAnnihilationMode( "oPs3G" );
  else if (command == f2GammaOnly)
    MaterialParameters::SetAnnihilationMode( "pPs2G" );
  else if (command == fPickOffOnly)
    MaterialParameters::SetAnnihilationMode( "oPs2G" );
  else if (command == fXADAdd_oPsComponent)
  {
    G4String paramString = newValue;
    std::istringstream is(paramString);
    G4double meanLifetime;
    G4double probability;
    is >> meanLifetime >> probability;
    MaterialParameters::Temp.oPsLifetimes.push_back(meanLifetime);
    MaterialParameters::Temp.oPsProbabilities.push_back(probability);
  }
  else if (command == fXADSet_pPsComponent) 
  {
    G4String paramString = newValue;
    std::istringstream is(paramString);
    G4double meanLifetime;
    G4double fraction;
    is >> meanLifetime >> fraction;
    MaterialParameters::Temp.pPsLifetime = meanLifetime;
    MaterialParameters::Temp.pPsFraction = fraction;
  }
  else if (command == fXADAdd_directComponent) 
  {
    G4String paramString = newValue;
    std::istringstream is(paramString);
    G4double meanLifetime;
    G4double probability;
    is >> meanLifetime >> probability;
    MaterialParameters::Temp.directLifetimes.push_back(meanLifetime);
    MaterialParameters::Temp.directProbabilities.push_back(probability);
  }
  else if (command == fReloadMaterials)
  {
    G4String paramString = newValue;
    DetectorConstruction::GetInstance()->ReloadMaterials(paramString);   
    MaterialParameters::ClearTemp();
  }
}
