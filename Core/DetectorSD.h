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
 *  @file DetectorSD.h
 */

#ifndef DETECTORSD_H
#define DETECTORSD_H 1

#include "../Objects/Geant4/DetectorHit.h"
#include <G4VSensitiveDetector.hh>

class DetectorSD : public G4VSensitiveDetector
{
public:
  DetectorSD(G4String name, G4int scinSum, G4double timeMergeValue);
  virtual ~DetectorSD();
  virtual void Initialize(G4HCofThisEvent* HCE);

private:
  struct HitParameters {
    public:
      G4int fID = -1;
      G4double fTime = 0.0;
  };

  G4double timeIntervals;
  G4int totScinNum;
  std::vector<HitParameters> previousHits;
  DetectorHitsCollection* fDetectorCollection;

protected:
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
};

#endif
