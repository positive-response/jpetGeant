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
 *  @file Trajectory.cpp
 */

#include <G4DynamicParticle.hh>
#include <G4PrimaryParticle.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleTypes.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4AttDefStore.hh>
#include <G4VVisManager.hh>
#include <G4UnitsTable.hh>
#include <G4UIcommand.hh>
#include <G4Polyline.hh>
#include <G4AttValue.hh>
#include "Trajectory.h"
#include <G4Colour.hh>
#include <G4AttDef.hh>
#include <G4Circle.hh>

G4ThreadLocal G4Allocator<Trajectory>* myTrajectoryAllocator = 0;

Trajectory::Trajectory() : G4VTrajectory(), fPositionRecord(0) {}

Trajectory::Trajectory(const G4Track* aTrack) : G4VTrajectory(), fPositionRecord(0)
{
  fParticleDefinition = aTrack->GetDefinition();
  fParticleName = fParticleDefinition->GetParticleName();
  fPDGCharge = fParticleDefinition->GetPDGCharge();
  fPDGEncoding = fParticleDefinition->GetPDGEncoding();
  fTrackID = aTrack->GetTrackID();
  fPositionRecord = new TrajectoryPointContainer();
  fPositionRecord->push_back(new G4TrajectoryPoint(aTrack->GetPosition()));
  fMomentum = aTrack->GetMomentumDirection();
  fVertexPosition = aTrack->GetPosition();
  fGlobalTime = aTrack->GetGlobalTime();
  fParentID = aTrack->GetParentID();
}

Trajectory::~Trajectory()
{
  size_t i;
  for (i = 0; i < fPositionRecord->size(); i++) {
    delete (*fPositionRecord)[i];
  }
  fPositionRecord->clear();
  delete fPositionRecord;
}

void Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if (!secondTrajectory) return;
  Trajectory* seco = (Trajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  //! initial point of the second trajectory should not be merged
  for (int i = 1; i < ent; i++) {
    fPositionRecord->push_back((*(seco->fPositionRecord))[i]);
  }
  delete (*seco->fPositionRecord)[0];
  seco->fPositionRecord->clear();
}

void Trajectory::AppendStep(const G4Step* aStep)
{
  fPositionRecord->push_back(
    new G4TrajectoryPoint(aStep->GetPostStepPoint()->GetPosition())
  );
}
