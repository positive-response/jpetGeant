/**
 *  @copyright Copyright 2020 The J-PET Monte Carlo Authors. All rights reserved.
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

#include "../Core/RunManager.h"
#include "EventMessenger.h"

EventMessenger* EventMessenger::fInstance = nullptr;

EventMessenger* EventMessenger::GetEventMessenger()
{
  if (fInstance == nullptr) {
    fInstance = new EventMessenger();
  }
  return fInstance;
}

EventMessenger::EventMessenger()
{
  fDirectory = new G4UIdirectory("/jpetmc/event/");
  fDirectory->SetGuidance("Define events to save");

  fCMDKillEventsEscapingWorld = new G4UIcmdWithABool("/jpetmc/event/saveEvtsDetAcc", this);
  fCMDKillEventsEscapingWorld->SetGuidance("Killing events when generated particle escapes detector");

  fPrintStat = new G4UIcmdWithABool("/jpetmc/event/printEvtStat", this);
  fPrintStat->SetGuidance("Print how many events was generated");

  fPrintStatPower = new G4UIcmdWithAnInteger("/jpetmc/event/printEvtFactor", this);
  fPrintStatPower->SetGuidance("Define X in divisor (10^X) for number of printed event ");

  fCMDMinRegMulti = new G4UIcmdWithAnInteger("/jpetmc/event/minRegMulti", this);
  fCMDMinRegMulti->SetGuidance("Set the lower value of registered multiplicity (works only with saveEvtsDetAcc); def: 0");

  fCMDMaxRegMulti = new G4UIcmdWithAnInteger("/jpetmc/event/maxRegMulti", this);
  fCMDMaxRegMulti->SetGuidance("Set the upper value of registered multiplicity (works only with saveEvtsDetAcc); def: 10");

  fCMDExcludedMulti = new G4UIcmdWithAnInteger("/jpetmc/event/excludedMulti", this);
  fCMDExcludedMulti->SetGuidance("Set excluded  multiplicity (works only with saveEvtsDetAcc); def: 1");

  fPrintStatBar = new G4UIcmdWithABool("/jpetmc/event/ShowProgress", this);
  fPrintStatBar->SetGuidance("Print how many events was generated (in %)");

  fAddDatetime = new G4UIcmdWithABool("/jpetmc/output/AddDatetime", this);
  fAddDatetime->SetGuidance("Adds to the output file name date and time of simulation start.");

  fSetSeed = new G4UIcmdWithAnInteger("/jpetmc/SetSeed", this);
  fSetSeed->SetGuidance("Use specific seed. If 0 provided seed will be random.");
  fSetSeed->SetDefaultValue(0);

  fSaveSeed = new G4UIcmdWithABool("/jpetmc/SaveSeed", this);
  fSaveSeed->SetGuidance("Save random seed (default false).");

  fCMDAllowedMomentumTransfer = new G4UIcmdWithADoubleAndUnit("/jpetmc/setAllowedMomentumTransfer", this);
  fCMDAllowedMomentumTransfer->SetGuidance("Limit on momentum transfer that will classify interaction as background (10keV)");
  fCMDAllowedMomentumTransfer->SetDefaultValue(1 * keV);
  fCMDAllowedMomentumTransfer->SetUnitCandidates("keV");
}

EventMessenger::~EventMessenger()
{
  delete fPrintStat;
  delete fPrintStatPower;
  delete fPrintStatBar;
  delete fAddDatetime;
  delete fSetSeed;
  delete fSaveSeed;
  delete fCMDKillEventsEscapingWorld;
  delete fCMDMinRegMulti;
  delete fCMDMaxRegMulti;
  delete fCMDExcludedMulti;
}

void EventMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fPrintStat){
    fPrintStatistics = fPrintStat->GetNewBoolValue(newValue);
  } else if (command == fPrintStatPower) {
    fPrintPower = fPrintStatPower->GetNewIntValue(newValue);
  } else if (command == fCMDMinRegMulti) {
    fMinRegisteredMultiplicity = fCMDMinRegMulti->GetNewIntValue(newValue);
  } else if (command == fCMDMaxRegMulti) {
    fMaxRegisteredMultiplicity = fCMDMaxRegMulti->GetNewIntValue(newValue);
  } else if (command == fPrintStatBar) {
    fShowProgress = fPrintStatBar->GetNewBoolValue(newValue);
  } else if (command == fAddDatetime){
    fOutputWithDatetime = fAddDatetime->GetNewBoolValue(newValue);
  } else if (command == fCMDKillEventsEscapingWorld){
    fKillEventsEscapingWorld = fCMDKillEventsEscapingWorld->GetNewBoolValue(newValue);
  } else if (command == fCMDExcludedMulti) {
    fExcludedMultiplicity = fCMDExcludedMulti->GetNewIntValue(newValue);
  } else if (command == fSetSeed) {
    fSeed = fSetSeed->GetNewIntValue(newValue);
  } else if (command == fSaveSeed) {
    fSaveRandomSeed = fSaveSeed->GetNewBoolValue(newValue);
  } else if (command == fAddDatetime) {
    fOutputWithDatetime = fAddDatetime->GetNewBoolValue(newValue);
  } else if (command == fCMDAllowedMomentumTransfer) {
    fAllowedMomentumTransfer = fCMDAllowedMomentumTransfer->GetNewDoubleValue(newValue);
  }
}

void EventMessenger::ClearHitOrigin()
{
  if(hitOrigin.size() > 0) {
    hitOrigin.clear();
  }
}

void EventMessenger::FillHitOrigin(int originIndex, InteractionType type, int originMultiplicity)
{
  bool isItFirstInteraction = true;
  for (unsigned i = 0; i < hitOrigin.size(); i++) {
    if (hitOrigin[i][0] == originIndex) {
      isItFirstInteraction = false;
      switch(type) {
        case scattActivePart:
          hitOrigin[i][2]++;
          break;
        case scattNonActivePart:
          hitOrigin[i][3]++;
          break;
        case secondaryPart:
          hitOrigin[i][4]++;
          break;
      }
    }
  }
  if (isItFirstInteraction) {
    std::vector<int> tempHitOrigin = {originIndex, originMultiplicity, 0, 0, 0};
    switch(type) {
      case scattActivePart:
        tempHitOrigin[2]++;
        break;
      case scattNonActivePart:
        tempHitOrigin[3]++;
        break;
      case secondaryPart:
        tempHitOrigin[4]++;
        break;
    }
    hitOrigin.push_back(tempHitOrigin);
  }
}

void EventMessenger::PassToEventMap(int originIndex, int originMultiplicity)
{
  bool isItFirstInteraction = true;
  for (unsigned i = 0; i < hitOrigin.size(); i++) {
    if (hitOrigin[i][0] == originIndex) {
      eventMap.push_back(hitOrigin[i]);
      isItFirstInteraction = false;
    }
  }
  if (isItFirstInteraction) {
    std::vector<int> tempHitOrigin = {originIndex, originMultiplicity, 0, 0, 0};
    eventMap.push_back(tempHitOrigin);
  }
}

void EventMessenger::PrintInfo()
{
  for (unsigned i = 0; i < hitOrigin.size(); i++) {
    std::cout << hitOrigin[i][0] << " " << hitOrigin[i][1] << " " << hitOrigin[i][2] << " " << hitOrigin[i][3] << " " << hitOrigin[i][4] << std::endl;
  }
}
