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
 *  @file ActionInitialization.cpp
 */

#include "PrimaryGeneratorAction.h"
#include "ActionInitialization.h"
#include "../Core/HistoManager.h"
#include "TrackingAction.h"
#include "SteppingAction.h"
#include "EventAction.h"
#include "RunAction.h"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {}

void ActionInitialization::Build() const
{
  HistoManager* histo = new HistoManager();
  SetUserAction(new EventAction(histo));
  SetUserAction(new RunAction(histo));
  SetUserAction(new PrimaryGeneratorAction(histo));
  SetUserAction(new TrackingAction);
  SetUserAction(new SteppingAction);
}
