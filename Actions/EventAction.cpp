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
 *  @file EventAction.cpp
 */

#include "../Info/PrimaryParticleInformation.h"
#include "../Objects/Geant4/DetectorHit.h"
#include "../Objects/Geant4/Trajectory.h"
#include <G4TrajectoryContainer.hh>
#include <G4ParticleDefinition.hh>
#include <G4PrimaryParticle.hh>
#include <G4EventManager.hh>
#include <G4SDManager.hh>
#include "EventAction.h"
#include <G4Event.hh>

EventAction::EventAction(HistoManager* histo): G4UserEventAction(), fScinCollID(-1)
{
  fHisto = histo;
}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* anEvent)
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if (fScinCollID < 0) {
    G4String colNam;
    fScinCollID = SDman->GetCollectionID(colNam = "detectorCollection");
  }
  fHisto->Clear();
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{

  if (anEvent->GetNumberOfPrimaryVertex() == 0) return;
  if(fEvtMessenger->KillEventsEscapingWorld()) {
    if(G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->IsAborted()){
      return;
    }
  }

  WriteToFile(anEvent);
}

void EventAction::WriteToFile(const G4Event* anEvent)
{
  // save information about generated events
  G4int id =  anEvent->GetEventID();
  fHisto->SetEventNumber(id);
  fHisto->FillHistoGenInfo(anEvent);

  //save information about registered events
  G4HCofThisEvent* HCE = anEvent->GetHCofThisEvent();
  DetectorHitsCollection* DHC = 0;
  if (HCE) {
    DHC = dynamic_cast<DetectorHitsCollection*>(HCE->GetHC(fScinCollID));
    int n_hit = DHC->entries();
    for (int i = 0; i < n_hit; i++) {
      DetectorHit* dh =  dynamic_cast<DetectorHit*>(DHC->GetHit(i));
      fHisto->AddNewHit(dh);
    }
  }

  //save to disk
  fHisto->SaveEvtPack();
}

