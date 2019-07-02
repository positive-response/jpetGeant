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
 *  @file MaterialExtension.h
 */

#ifndef MATERIALEXTENSION_H
#define MATERIALEXTENSION_H 1

#include "../Info/MaterialExtensionMessenger.h"
#include <G4VMaterialExtension.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <G4Material.hh>
#include <globals.hh>

/**
 * @class MaterialExtension
 * @brief extended material keeps information about 2g/3g creation fraction
 */

class MaterialExtension : public G4Material
{
public:
  MaterialExtension(const G4String& name, const G4Material* baseMaterial);
  ~MaterialExtension();

  G4Material* GetMaterial()
  {
    return fMaterial;
  };

  void SetoPsLifetime(G4double);

  void SetoPsFraction(G4double);

  void SetPickOffFraction(G4double sf)
  {
    fPickOffFraction = sf;
  }

  const G4double Get3gFraction();

  const G4double GetoPsLifetime()
  {
    return foPslifetime;
  };

  //! 2g direct // 2g pickoff (lifetime 3g) // 3g direct // 3g oPs (lifetime 3g)
  const std::vector<G4double> GetEventsFraction();

  G4bool IsTarget()
  {
    return fTarget;
  };

  void AllowsAnnihilations(G4bool tf)
  {
    fTarget = tf;
  };

  void Print() const {;};

  G4bool IsExtended() const
  {
    return true;
  }

private:
  G4Material* fMaterial;
  MaterialExtensionMessenger* fMaterialExtensionMessenger
    = MaterialExtensionMessenger::GetMaterialExtensionMessenger();
  G4bool fTarget;
  //! 3g/2g events fraction
  G4double foPsFraction;
  G4double foPslifetime;
  G4double fPickOffFraction;

};

#endif
