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
 *  @file EvtInfo.h
 */

#ifndef EVTINFO_H
#define EVTINFO_H 1

#include <globals.hh>
#include <TVector3.h>
#include <TObject.h>

class EvtInfo : public TObject
{
public:
  EvtInfo();
  ~EvtInfo();
  void FillEvtInfo(G4double id, G4double x, G4double y, G4double z, G4double t);

private:
  G4int fEvtId;
  G4double fX;
  G4double fY;
  G4double fZ;
  G4double fTime;

  ClassDef(EvtInfo, 1)
};

#endif
