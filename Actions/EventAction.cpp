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

bool EventAction::is2gRec = false;
bool EventAction::is3gRec = false;

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
  G4int id =  anEvent->GetEventID();
  fHisto->SetEventNumber(id);
  fHisto->FillHistoGenInfo(anEvent);
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
  if (anEvent->GetNumberOfPrimaryVertex() == 0) return;
  if (fEvtMessenger->SaveOnlySelectedEvents()) {
    if (fEvtMessenger->Save2gEvents()) {
      CheckIf2gIsRegistered(anEvent);
    }
    if (fEvtMessenger->Save3gEvents()) {
      CheckIf3gIsRegistered(anEvent);
    }
    if (!(Is2gRegistered() || Is3gRegistered())) {
      return;
    }
  }
  WriteToFile(anEvent);
}

void EventAction::WriteToFile(const G4Event* anEvent)
{
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
  fHisto->SaveEvtPack();
}

bool EventAction::Is2gRegistered()
{
  return is2gRec;
}

bool EventAction::Is3gRegistered()
{
  return is3gRec;
}

void EventAction::CheckIf3gIsRegistered(const G4Event* anEvent)
{
  bool isGenerated = false;
  bool isReconstructed = true;
  is3gRec = false;
  std::vector<bool> isGammaRec{false, false, false, false};
  for (int i = 0; i < anEvent->GetNumberOfPrimaryVertex(); i++) {
    VtxInformation* info =  dynamic_cast<VtxInformation*>(
      anEvent->GetPrimaryVertex(i)->GetUserInformation()
    );
    if (info != nullptr) {
      isGenerated = isGenerated || info->GetThreeGammaGen();
    }
  }

  if (!isGenerated) {
    return;
  }

  G4HCofThisEvent* HCE = anEvent->GetHCofThisEvent();
  DetectorHitsCollection* DHC = 0;
  if (HCE) {
    DHC = dynamic_cast<DetectorHitsCollection*>(HCE->GetHC(fScinCollID));
    int n_hit = DHC->entries();
    if (n_hit < 3) return;

    for (int i = 0; i < n_hit; i++) {
      DetectorHit* dh =  dynamic_cast<DetectorHit*>(DHC->GetHit(i));
      if (dh->GetGenGammaMultiplicity() == 3) {
        isGammaRec[dh->GetGenGammaIndex()] = true;
      }
    }
  }

  for (int i = 1; i <= 3; i++) {
    isReconstructed = isReconstructed && isGammaRec[i];
  }

  is3gRec = isReconstructed;
}

void EventAction::CheckIf2gIsRegistered(const G4Event* anEvent)
{
  bool isGenerated = false;
  bool isReconstructed = true;
  is2gRec = false;
  std::vector<bool> isGammaRec{false, false, false};

  for (int i = 0; i < anEvent->GetNumberOfPrimaryVertex(); i++) {
    VtxInformation* info =  dynamic_cast<VtxInformation*>(
      anEvent->GetPrimaryVertex(i)->GetUserInformation()
    );
    if (info != nullptr) {
      isGenerated = isGenerated || info->GetTwoGammaGen();
    }
  }

  if (!isGenerated) {
    return;
  }

  G4HCofThisEvent* HCE = anEvent->GetHCofThisEvent();
  DetectorHitsCollection* DHC = 0;
  if (HCE) {
    DHC = dynamic_cast<DetectorHitsCollection*>(HCE->GetHC(fScinCollID));
    int n_hit = DHC->entries();
    if (n_hit < 2) return;

    for (int i = 0; i < n_hit; i++) {
      DetectorHit* dh =  dynamic_cast<DetectorHit*>(DHC->GetHit(i));
      if (dh->GetGenGammaMultiplicity() == 2) {
        isGammaRec[dh->GetGenGammaIndex()] = true;
      }
    }
  }

  for (int i = 1; i <= 2; i++) {
    isReconstructed = isReconstructed && isGammaRec[i];
  }

  is2gRec = isReconstructed;
}
