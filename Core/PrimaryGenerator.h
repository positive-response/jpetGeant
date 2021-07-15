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
 *  @file PrimaryGenerator.h
 */

#ifndef PRIMARYGENERATOR_H
#define PRIMARYGENERATOR_H 1

#include "../Objects/Framework/JPetGeantDecayTree.h"
#include "MaterialExtension.h"
#include "SourceParams.h"
#include "BeamParams.h"

#include <G4TransportationManager.hh>
#include <G4VPrimaryGenerator.hh>
#include <G4PrimaryVertex.hh>
#include <TGenPhaseSpace.h>
#include <TLorentzVector.h>
#include <G4Navigator.hh>
#include <TRotation.h>
#include <G4Event.hh>
#include <TVector3.h>
#include <vector>
#include <map>

struct NemaPoint
{
  NemaPoint();
  G4ThreeVector position;
  G4double lifetime;
  bool is3GAllowed;
  G4ThreeVector sizeOfPoint;
  G4ThreeVector orientationOfPoint;
  G4ThreeVector shapeOfPointInX;
  G4ThreeVector shapeOfPointInY;
};

class NemaGenerator
{
public:
  NemaGenerator() {
    fSeed = time(NULL);
    srand (fSeed);
  };
  ~NemaGenerator() {
    fGeneratedPoints.clear();
    fWeightPositions.clear();
    fIDPointsConnection.clear();
  };
  void Clear() {
    fGeneratedPoints.clear();
    fWeightPositions.clear();
    fIDPointsConnection.clear();
  };
  void AddPoint(int pointID);
  void AddPointWeight(int pointID, int weight);
  void SetOnePointOnly(int pointID);
  void SetPointPosition(int pointID, G4ThreeVector pos) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).position = pos; };
  void SetPointLifetime(int pointID, G4double lf) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).lifetime = lf; };
  void SetPoint3GOption(int pointID, bool is3G) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).is3GAllowed = is3G; };
  void SetPointSize(int pointID, G4ThreeVector size) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).sizeOfPoint = size; };
  void SetPointOrientation(int pointID, G4ThreeVector orient) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).orientationOfPoint = orient; };
  void SetPointShapeX(int pointID, G4ThreeVector shape) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).shapeOfPointInX = shape; };
  void SetPointShapeY(int pointID, G4ThreeVector shape) { fGeneratedPoints.at(fIDPointsConnection.at(pointID)).shapeOfPointInY = shape; };
  bool DoesPointExistAlready(int pointID);
  NemaPoint GetPoint(int pointID) const;
  NemaPoint GetRandomPoint() const;
private:
  int fSeed;
  std::vector<NemaPoint> fGeneratedPoints;
  std::vector<int> fWeightPositions;
  std::map<int, int> fIDPointsConnection;
};

class PrimaryGenerator : public G4VPrimaryGenerator
{
public:
  PrimaryGenerator();
  ~PrimaryGenerator();
  void GenerateBeam(BeamParams*, G4Event*);
  void GenerateIsotope(SourceParams*, G4Event*);
  void GenerateNema(G4Event*, NemaGenerator*);
  void GenerateEvtSmallChamber(G4Event* event, const G4double);
  void GenerateEvtLargeChamber(G4Event* event);
  virtual void GeneratePrimaryVertex(G4Event*){};
  DecayChannel GetDecayChannel() { return fDecayChannel; };

private:
  DecayChannel fDecayChannel = DecayChannel::Unknown;
  //! return: vtx position, 2/3g ratio, meanlifetime;
  //! as input the maximal dimension(/2) of annihilation chamber are taken (to speed up simulatons)
  std::tuple<G4ThreeVector, MaterialExtension*> GetVerticesDistributionInFilledSphere(
    const G4ThreeVector center, G4double radius
  );
  std::tuple<G4ThreeVector, MaterialExtension*> GetVerticesDistributionAlongStepVector(
    const G4ThreeVector center, const G4ThreeVector direction
  );
  G4PrimaryVertex* GenerateTwoGammaVertex(
    const G4ThreeVector vtxPosition, const G4double T0, const G4double lifetime2g
  );
  G4PrimaryVertex* GenerateThreeGammaVertex(
    const DecayChannel channel, const G4ThreeVector vtxPosition, const G4double T0, const G4double lifetime3g
  );
  G4PrimaryVertex* GeneratePromptGammaVertex(
    const G4ThreeVector vtxPosition, const G4double T0, const G4double lifetimePrompt, const G4double energy
  );
  G4ThreeVector VertexUniformInCylinder(G4double, G4double);
  G4double calculate_mQED(
    const DecayChannel channel, Double_t mass_e, Double_t w1, Double_t w2, Double_t w3
  );
  const G4ThreeVector GetRandomPointInFilledSphere(G4double radius);
  const G4ThreeVector GetRandomPointOnSphere(G4double radius);
  
  G4Navigator* theNavigator =  G4TransportationManager::GetTransportationManager()
  ->GetNavigatorForTracking();
};

#endif /* !PRIMARYGENERATOR_H */
