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
 *  @file TrackingAction.h
 */

#ifndef TRACKINGACTION_H
#define TRACKINGACTION_H 1

#include <G4UserTrackingAction.hh>

class TrackingAction : public G4UserTrackingAction
{
public:
  TrackingAction() : G4UserTrackingAction() {}
  virtual ~TrackingAction() {}
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);

};

#endif
