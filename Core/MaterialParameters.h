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
 *  @file MaterialParameters.h
 */

#ifndef MATERIALPARAMETERS_H
#define MATERIALPARAMETERS_H 1

#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>
#include <globals.hh>

class MaterialParameters
{
public:
  //! Mean lifetime denoted as Tau (greek letter)
  static const G4double oPsTauVaccum;
  static const G4double oPsTauAl;
  static const G4double oPsTauXAD;
  static const G4double oPsProbabilityAl;
  //! 4/3 I; taken only most intense component
  static const G4double oPsProbabilityXAD;
  static const G4double fractionDirect3g;
  static const G4double tauKapton;
  static const G4double tauBulk;
  //! Sodium parameters
  static const G4double sodiumGammaEnergy;
  static const G4double sodiumGammaTau;
  static G4double GetoPsFractionPALS(G4double, G4double);
  static void SetEventsTypesToSimulate(bool b_2g, bool b_3g, bool b_pickOff);
  static void SetXADoPsLifetime(G4double);
  static void SetXADoPsFraction(G4double);
  static void SetXADPickOffFraction(G4double);
  static G4double GetXADoPsTau();
  static G4double GetXADoPsFraction();
  static G4double GetXADPickOffFraction();
  static void SetXADoPsOnly();
  static void SetXAD2gOnly();
  static void SetXADPickOffOnly();

private:
  static G4double foPsTauXAD;
  static G4double foPsFractionXAD;
  static G4double fpickOffFractionXAD;

};

#endif
