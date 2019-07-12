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
  if ( fInstance == nullptr ) {
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

  fXAD3GammaOnly = new G4UIcmdWithoutParameter("/jpetmc/material/xad/threeGammaOnly", this);
  fXAD3GammaOnly->SetGuidance("Only 3 gamma events will be generated");

  fXAD2GammaOnly = new G4UIcmdWithoutParameter("/jpetmc/material/xad/twoGammaOnly", this);
  fXAD2GammaOnly->SetGuidance("Only 2 gamma events will be generated");

  fXADPickOffOnly = new G4UIcmdWithoutParameter("/jpetmc/material/xad/pickOffOnly", this);
  fXADPickOffOnly->SetGuidance("Only 2 gamma events from pick-off/conversion will be generated (lifetime as for 3g)");

  fXADSet3gLifetime = new G4UIcmdWithADoubleAndUnit("/jpetmc/material/xad/oPslifetime", this);
  fXADSet3gLifetime->SetGuidance("Set mean lifetime for 3g events");
  fXADSet3gLifetime->SetDefaultValue(2.45);
  fXADSet3gLifetime->SetDefaultUnit("ns");
  fXADSet3gLifetime->SetUnitCandidates("ns");

  fXADSet3gFraction = new G4UIcmdWithADouble("/jpetmc/material/xad/oPsFraction", this);
  fXADSet3gFraction->SetGuidance("Set fraction of oPs events (0-1)");
  fXADSet3gFraction->SetDefaultValue(0.1);

  fXADSetPickOffFraction = new G4UIcmdWithADouble("/jpetmc/material/xad/pickOffFraction", this);
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
  if (command == fXAD3GammaOnly) {
    MaterialParameters::SetXADoPsOnly();
    DetectorConstruction::GetInstance()->ReloadMaterials();
  }

  if (command == fXAD2GammaOnly) {
    MaterialParameters::SetXAD2gOnly();
    DetectorConstruction::GetInstance()->ReloadMaterials();
  }

  if (command == fXADPickOffOnly) {
    MaterialParameters::SetXADPickOffOnly();
    DetectorConstruction::GetInstance()->ReloadMaterials();
  }

  if (command == fXADSet3gLifetime) {
    MaterialParameters::SetXADoPsLifetime(fXADSet3gLifetime->GetNewDoubleValue(newValue));
    DetectorConstruction::GetInstance()->ReloadMaterials();
  }

  if (command == fXADSet3gFraction) {
    G4double frac =  fXADSet3gFraction->GetNewDoubleValue(newValue);
    if ((frac > 1.0) || (frac < 0.0)) {
      G4Exception(
        "MaterialMessenger", "MP01", JustWarning, "Fraction should be between 0 and 1!"
      );
    } else {
      MaterialParameters::SetXADoPsFraction(frac);
      DetectorConstruction::GetInstance()->ReloadMaterials();
    }
  }

  if (command == fXADSetPickOffFraction) {
    G4double frac =  fXADSetPickOffFraction->GetNewDoubleValue(newValue);
    if ((frac > 1.0) || (frac < 0.0)) {
      G4Exception(
        "MaterialMessenger", "MP01", JustWarning, "Fraction should be between 0 and 1!"
      );
    } else {
      MaterialParameters::SetXADPickOffFraction(frac);
      DetectorConstruction::GetInstance()->ReloadMaterials();
    }
  }
}
