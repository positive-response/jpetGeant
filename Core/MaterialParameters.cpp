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
 *  @file MaterialParameters.cpp
 */

#include "MaterialParameters.h"

const G4double MaterialParameters::oPsTauVaccum = 142 * ns;
//! data for Al https://doi.org/10.1007/s00339-015-9214-0
const G4double MaterialParameters::oPsTauAl = 0.32 * ns;
const G4double MaterialParameters::oPsTauXAD = 2.45 * ns;
const G4double MaterialParameters::oPsProbabilityAl = 0.12;
//! 4/3 I; taken only most intense component
const G4double MaterialParameters::oPsProbabilityXAD = 89.6;
const G4double MaterialParameters::fractionDirect3g = 0.002688;
//! doi 10.2478/umcschem-2013-0012
const G4double MaterialParameters::tauKapton = 0.380 * ns;
const G4double MaterialParameters::tauBulk = 0.2 * ns;
const G4double MaterialParameters::sodiumGammaEnergy = 1.2770 * MeV;
const G4double MaterialParameters::sodiumGammaTau = 3.7 * ps;

G4double MaterialParameters::foPsTauXAD = oPsTauXAD;
G4double MaterialParameters::foPsFractionXAD = GetoPsFractionPALS(oPsProbabilityXAD, oPsTauXAD);
G4double MaterialParameters::fpickOffFractionXAD = 10 * foPsFractionXAD;

void MaterialParameters::SetXADoPsLifetime(G4double x)
{
  foPsTauXAD = x;
}

void MaterialParameters::SetXADoPsFraction(G4double x)
{
  foPsFractionXAD = x;
}

void MaterialParameters::SetXADPickOffFraction(G4double x)
{
  fpickOffFractionXAD = x;
}

G4double MaterialParameters::GetXADoPsTau()
{
  return foPsTauXAD;
}
G4double MaterialParameters::GetXADoPsFraction()
{
  return foPsFractionXAD;
}
G4double MaterialParameters::GetXADPickOffFraction()
{
  return fpickOffFractionXAD;
}

void MaterialParameters::SetXADoPsOnly()
{
  foPsFractionXAD = 1.0f;
  fpickOffFractionXAD = 0.0f;
}

void MaterialParameters::SetXAD2gOnly()
{
  foPsFractionXAD = 0.0f;
  fpickOffFractionXAD = 0.0f;
}

void MaterialParameters::SetXADPickOffOnly()
{
  foPsFractionXAD = 0.0f;
  fpickOffFractionXAD = 1.0f;
}

G4double MaterialParameters::GetoPsFractionPALS(G4double oPsPobability, G4double oPslifetime)
{
  return (oPslifetime / oPsTauVaccum) * oPsPobability / 100.;
}
