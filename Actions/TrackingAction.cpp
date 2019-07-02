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
 *  @file TrackingAction.cpp
 */

#include "../Objects/Geant4/Trajectory.h"
#include <G4TrackingManager.hh>
#include "TrackingAction.h"
#include <G4Track.hh>

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //! Create trajectory only for primaries
  fpTrackingManager->SetStoreTrajectory(true);
  fpTrackingManager->SetTrajectory(new Trajectory(aTrack));
}

void TrackingAction::PostUserTrackingAction(const G4Track*) {}
