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
 *  @file PhysicsList.h
 */

#ifndef PHYSICSLIST_H
#define PHYSICSLIST_H 1

#include <G4VModularPhysicsList.hh>

/**
 * @class PhysicsList
 * @brief standard GEANT4 package is used for physics
 * used G4EmLivermorePolarizedPhysics deals properly with
 * polarized particles
 */
class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  virtual ~PhysicsList();
};

#endif
