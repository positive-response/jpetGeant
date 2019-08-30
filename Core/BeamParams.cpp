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
 *  @file BeamParams.cpp
 */

#include "BeamParams.h"

BeamParams::BeamParams()
{
  energy = 511 * keV;
  this->SetVtxPosition(0, 0, 0);
  this->SetPolarization(0, 0, 0);
  this->SetMomentum(1, 0, 0);
}

void BeamParams::SetVtxPosition(G4double x, G4double y, G4double z)
{
  vtxPosition.setX(x);
  vtxPosition.setY(y);
  vtxPosition.setZ(z);
}

void BeamParams::SetPolarization(G4double x, G4double y, G4double z)
{
  polarization.setX(x);
  polarization.setY(y);
  polarization.setZ(z);
  polarization = polarization.unit();
}

void BeamParams::SetMomentum(G4double x, G4double y, G4double z)
{
  momentum.setX(x);
  momentum.setY(y);
  momentum.setZ(z);
  momentum = momentum.unit();
}

void BeamParams::SetVtxPosition(G4ThreeVector v)
{
  vtxPosition = v;
}

void BeamParams::SetPolarization(G4ThreeVector v)
{
  polarization = v;
  polarization = polarization.unit();
}

void BeamParams::SetMomentum(G4ThreeVector v)
{
  momentum = v;
  momentum = momentum.unit();
}
