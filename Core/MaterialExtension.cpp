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
 *  @file JPetMC.cpp
 */

#include "MaterialParameters.h"
#include "MaterialExtension.h"

MaterialExtension::MaterialExtension(const G4String& name, const G4Material* baseMaterial) :
  G4Material(
    name, baseMaterial->GetDensity(), baseMaterial, baseMaterial->GetState(),
    baseMaterial->GetTemperature(), baseMaterial->GetPressure()
  )
{
  foPslifetime = 0.;
  foPsFraction = 0.;
  fTarget = false ;
  //! depends on the material typ
  fPickOffFraction = 0.;
}


MaterialExtension::~MaterialExtension() {}

const G4double MaterialExtension::Get3gFraction()
{
  return foPsFraction;
}

void MaterialExtension::SetoPsFraction(G4double x)
{
  if (x < 0 || x > 1) {
    G4Exception(
      "MaterialExtension", "ME01", JustWarning, "3gamma fraction can not extend 1; nothing happend"
    );
  } else {
    foPsFraction = x;
  };
}

void MaterialExtension::SetoPsLifetime(G4double x)
{
  if (x < 0 || x > MaterialParameters::oPsTauVaccum) {
    G4Exception(
      "MaterialExtension", "ME02", JustWarning, "oPs lifetime can not excide 142*ns"
    );
  } else {
    foPslifetime = x;
  };
}

const std::vector<G4double> MaterialExtension::GetEventsFraction()
{
  G4double direct3g = MaterialParameters::fractionDirect3g;

  if (foPsFraction == 1) {
    direct3g = 0.0f;
  }

  if ( fPickOffFraction == 1) {
    direct3g = 0.0f;
  }

  //! 2g direct // 2g pickoff // 3g direct // 3g oPs
  std::vector<G4double> frac = {
    1. - fPickOffFraction - direct3g - foPsFraction,
    fPickOffFraction,
    direct3g,
    foPsFraction};
  return frac;
}
