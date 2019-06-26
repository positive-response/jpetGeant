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
 *  @file EventMessenger.cpp
 */

#include "EventMessenger.h"
#include "RunManager.h"

EventMessenger*  EventMessenger::fInstance = nullptr;

EventMessenger* EventMessenger::GetEventMessenger()
{
  if ( fInstance == nullptr ) {
    fInstance = new EventMessenger();
  }
  return fInstance;
}


EventMessenger::EventMessenger()
{
  fDirectory = new G4UIdirectory("/jpetmc/event/");
  fDirectory->SetGuidance("Define events to save");

  fSave2g = new G4UIcmdWithoutParameter("/jpetmc/event/save2g", this);
  fSave2g->SetGuidance("Events with registered 2g will be saved");
  fSave3g = new G4UIcmdWithoutParameter("/jpetmc/event/save3g", this);
  fSave3g->SetGuidance("Events with registered 3g will be saved");
  fPrintStat = new G4UIcmdWithABool("/jpetmc/event/printEvtStat", this);
  fPrintStat->SetGuidance("Print how many events was generated");
  fPrintStatPower = new G4UIcmdWithAnInteger("/jpetmc/event/printEvtFactor", this);
  fPrintStatPower->SetGuidance("Define X in divisor (10^X) for number of printed event ");
  fPrintStatBar = new G4UIcmdWithABool("/jpetmc/event/ShowProgress", this);
  fPrintStatBar->SetGuidance("Print how many events was generated (in %)");
}

EventMessenger::~EventMessenger()
{
  delete fSave2g;
  delete fSave3g;
  delete fPrintStat;
  delete fPrintStatPower;
  delete fPrintStatBar;
}


void EventMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fSave2g) {
    fSave2gEvts = true;
  }

  if (command == fSave3g) {
    fSave3gEvts = true;
  }

  if (command == fPrintStat) {
    fPrintStatistics = fPrintStat->GetNewBoolValue(newValue);
  }

  if (command == fPrintStatPower) {
    fPrintPower = fPrintStatPower->GetNewIntValue(newValue);
  }

  if (command == fPrintStatBar) {
    fShowProgress = fPrintStatBar->GetNewBoolValue(newValue);
  }
}
