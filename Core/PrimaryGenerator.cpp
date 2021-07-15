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
 *  @file PrimaryGenerator.cpp
 */

#include "../Info/PrimaryParticleInformation.h"
#include "../Info/VtxInformation.h"
#include "DetectorConstruction.h"
#include "MaterialParameters.h"
#include "DetectorConstants.h"
#include "MaterialExtension.h"
#include "PrimaryGenerator.h"

#include <G4HadPhaseSpaceGenbod.hh>
#include <G4ParticleDefinition.hh>
#include <G4PhysicalConstants.hh>
#include <G4RandomDirection.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleTable.hh>
#include <Randomize.hh>
#include <globals.hh>

NemaPoint::NemaPoint()
{
  G4ThreeVector temp(0,0,0);
  position = temp;
  lifetime = MaterialParameters::fTauBulk;
  is3GAllowed = false;
  G4ThreeVector temp2(0.1,0.1,0.1);
  sizeOfPoint = temp2;
  orientationOfPoint = temp;
  shapeOfPointInX = temp;
  shapeOfPointInY = temp;
}

void NemaGenerator::AddPoint(int pointID)
{
  auto search = fIDPointsConnection.find(pointID);
  if (search == fIDPointsConnection.end()) {
    int genPointSize = fGeneratedPoints.size();
    NemaPoint newPoint;
    fGeneratedPoints.push_back(newPoint);
    fIDPointsConnection.insert(std::make_pair(pointID, genPointSize));
  }
}

void NemaGenerator::AddPointWeight(int pointID, int weight)
{
  unsigned removedElements = 0;
  if (weight == 0) {
    for (unsigned i=0; i<fWeightPositions.size(); i++) {
      if (fWeightPositions.at(i-removedElements) == pointID) {
        fWeightPositions.erase(fWeightPositions.begin()+i-removedElements);
        removedElements++;
      }
    }
  } else {
    int count = 0;
    for (unsigned i=0; i<fWeightPositions.size(); i++) {
      if (fWeightPositions.at(i) == pointID)
        count++;
    }
    if (weight >= count) {
      for (int i=0; i<weight-count; i++)
        fWeightPositions.push_back(pointID);
    } else {
      int nmbOfPointToErase = count - weight;
      for (unsigned i=0; i<fWeightPositions.size(); i++) {
        if (fWeightPositions.at(i-removedElements) == pointID) {
          if ((int)removedElements < nmbOfPointToErase) {
            fWeightPositions.erase(fWeightPositions.begin()+i-removedElements);
            removedElements++;
          }
        }
      }
    }
  }
}

void NemaGenerator::SetOnePointOnly(int pointID)
{
  NemaPoint tempPoint = GetPoint(pointID);
  Clear();
  fGeneratedPoints.push_back(tempPoint);
  fWeightPositions.push_back(pointID);
  fIDPointsConnection.insert(std::make_pair(pointID, 0));
}

bool NemaGenerator::DoesPointExistAlready(int pointID) 
{
  auto search = fIDPointsConnection.find(pointID);
  if (search == fIDPointsConnection.end()) {
    return false;
  } else {
    return true;
  }
}

NemaPoint NemaGenerator::GetPoint(int pointID) const
{
  auto search = fIDPointsConnection.find(pointID);
  if (search != fIDPointsConnection.end()) {
    return fGeneratedPoints.at(fIDPointsConnection.at(pointID));
  } else {
    NemaPoint newPoint;
    return newPoint;
  }
}

NemaPoint NemaGenerator::GetRandomPoint() const
{
  if (fGeneratedPoints.size() == 1)
    return fGeneratedPoints.at(0);
  
  unsigned indexOfPoint = rand() % (int)fWeightPositions.size();
  int pointID = fWeightPositions.at(indexOfPoint);
  return GetPoint(pointID);
}

PrimaryGenerator::PrimaryGenerator() : G4VPrimaryGenerator() {}

PrimaryGenerator::~PrimaryGenerator() {}

G4PrimaryVertex* PrimaryGenerator::GenerateThreeGammaVertex( 
  const DecayChannel channel, const G4ThreeVector vtxPosition, 
  const G4double T0, const G4double lifetime3g
) {

  G4PrimaryVertex* vertex = new G4PrimaryVertex();
  VtxInformation* info = new VtxInformation();

  G4double lifetime = G4RandExponential::shoot(lifetime3g);
  info->SetThreeGammaGen(true);
  info->SetLifetime((T0 + lifetime));
  info->SetVtxPosition(vtxPosition.x(), vtxPosition.y(), vtxPosition.z());
  vertex->SetUserInformation(info);
  vertex->SetT0(T0 + lifetime);
  vertex->SetPosition(vtxPosition.x(), vtxPosition.y(), vtxPosition.z());

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");
  Double_t mass_secondaries[3] = {0., 0., 0.};

  TGenPhaseSpace event;
  TLorentzVector positonium(0.0, 0.0, 0.0, 1022 * keV);
  Bool_t test = event.SetDecay(positonium, 3, mass_secondaries);
  if (!test) {
    G4cout << "error: generate_gamma : createThreeEvts:" << test << G4endl;
  }

  Double_t weight = 1;
  Double_t weight_max = event.GetWtMax() * pow(10, -1);
  Double_t rwt = 1;
  Double_t M_max = 1;
  if (channel == DecayChannel::Para3G) {
    M_max = 2.00967 * pow(10, 25);
  } else {
    M_max = 7.65928 * pow(10, -6);
  }
  do {
    weight = event.Generate();
    weight = weight * calculate_mQED( channel, 511., event.GetDecay(0)->E() / keV, event.GetDecay(1)->E() / keV, event.GetDecay(2)->E() / keV );
    rwt = M_max * weight_max * (G4UniformRand());
  } while (rwt > weight);

  G4PrimaryParticle* particle[3];
  for (int i = 0; i < 3; i++) {
    TLorentzVector* out = event.GetDecay(i);
    particle[i] = new G4PrimaryParticle(
      particleDefinition, out->Px(), out->Py(), out->Pz(), out->E()
    );
    PrimaryParticleInformation* infoParticle = new PrimaryParticleInformation();
    infoParticle->SetGammaMultiplicity(PrimaryParticleInformation::koPsGamma);
    infoParticle->SetGeneratedGammaMultiplicity(PrimaryParticleInformation::koPsGamma);
    infoParticle->SetIndex(i + 1);
    infoParticle->SetGenMomentum(out->Px(), out->Py(), out->Pz());
    particle[i]->SetUserInformation(infoParticle);
    vertex->SetPrimary(particle[i]);
  }
  return vertex;
}

G4PrimaryVertex* PrimaryGenerator::GenerateTwoGammaVertex(
  const G4ThreeVector vtxPosition, const G4double T0, const G4double lifetime2g
) {
  G4PrimaryVertex* vertex = new G4PrimaryVertex();
  VtxInformation* info = new VtxInformation();

  G4double lifetime = G4RandExponential::shoot(lifetime2g);
  info->SetTwoGammaGen(true);
  info->SetLifetime((T0 + lifetime));
  info->SetVtxPosition(vtxPosition.x(), vtxPosition.y(), vtxPosition.z());
  vertex->SetUserInformation(info);
  vertex->SetT0(T0 + lifetime);
  vertex->SetPosition(vtxPosition.x(), vtxPosition.y(), vtxPosition.z());

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");

  Double_t mass_secondaries[2] = {0., 0.};

  TGenPhaseSpace event;
  TLorentzVector positonium(0.0, 0.0, 0.0, 1022 * keV);
  Bool_t test = event.SetDecay(positonium, 2, mass_secondaries);
  if (!test) {
    G4cout << "Error: generate_gamma : createTwoEvts" << test << G4endl;
  }

  event.Generate();
  G4PrimaryParticle* particle[2];

  for (int i = 0; i < 2; i++) {
    TLorentzVector* out = event.GetDecay(i);
    particle[i] = new G4PrimaryParticle(
      particleDefinition, out->Px(), out->Py(), out->Pz(), out->E()
    );

    PrimaryParticleInformation* infoParticle = new PrimaryParticleInformation();
    infoParticle->SetGammaMultiplicity(PrimaryParticleInformation::kBackToBackGamma);
    infoParticle->SetGeneratedGammaMultiplicity(PrimaryParticleInformation::kBackToBackGamma);
    infoParticle->SetIndex(i + 1);
    infoParticle->SetGenMomentum(out->Px(), out->Py(), out->Pz());
    particle[i]->SetUserInformation(infoParticle);
    vertex->SetPrimary(particle[i]);
  }

  G4ThreeVector gammaMom = particle[0]->GetMomentum();
  G4ThreeVector polarization1 = gammaMom.orthogonal().unit();
  polarization1 = polarization1.rotate(twopi * G4UniformRand(), gammaMom);
  particle[1]->SetPolarization(polarization1);

  G4ThreeVector polarization2 = polarization1;
  polarization2 = polarization2.rotate(pi / 2.0, gammaMom);
  particle[0]->SetPolarization(polarization2);
  return vertex;
}

G4PrimaryVertex* PrimaryGenerator::GeneratePromptGammaVertex(
  const G4ThreeVector vtxPosition, const G4double T0,
  const G4double lifetimePrompt, const G4double energy
) {
  G4PrimaryVertex* vertex = new G4PrimaryVertex();
  VtxInformation* info = new VtxInformation();

  G4double lifetime = G4RandExponential::shoot(lifetimePrompt);
  info->SetPromptGammaGen(true);
  info->SetLifetime((T0 + lifetime));
  info->SetVtxPosition(vtxPosition.x(), vtxPosition.y(), vtxPosition.z());
  vertex->SetUserInformation(info);
  vertex->SetT0(T0 + lifetime);
  vertex->SetPosition(vtxPosition.x(), vtxPosition.y(), vtxPosition.z());

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");
  G4ThreeVector momentum = GetRandomPointOnSphere(energy);
  G4PrimaryParticle* particle1 = new G4PrimaryParticle(
    particleDefinition, momentum.x(), momentum.y(), momentum.z(), energy
  );
  PrimaryParticleInformation* infoParticle = new PrimaryParticleInformation();
  infoParticle->SetGammaMultiplicity(PrimaryParticleInformation::kPromptGamma);
  infoParticle->SetGeneratedGammaMultiplicity(PrimaryParticleInformation::kPromptGamma);
  infoParticle->SetIndex(0);
  infoParticle->SetGenMomentum(momentum.x(), momentum.y(), momentum.z());
  particle1->SetUserInformation(infoParticle);
  vertex->SetPrimary(particle1);
  return vertex;
}

void PrimaryGenerator::GenerateEvtSmallChamber(
  G4Event* event, const G4double effectivePositronRadius
) {
  G4ThreeVector chamberCenter = DetectorConstants::GetChamberCenter();
  G4ThreeVector vtxPosition;
  MaterialExtension* material;
  std::tie(vtxPosition, material) = GetVerticesDistributionInFilledSphere(
    chamberCenter, effectivePositronRadius
  );
  std::vector<G4double> evtFractions = material->GetEventsFraction();
  G4double random = G4UniformRand();

  //! for sodium: emitted positrons have up to 100~keV velocity
  //! therefore their speed v=sqrt(2*e/m) = 0.6c
  G4double T0 = (vtxPosition - chamberCenter).mag() / (0.6 * c_light);

  if (evtFractions[0] > random) {
  // pPs 2G
    event->AddPrimaryVertex(GenerateTwoGammaVertex(
      vtxPosition, T0,
      material->GetLifetime(random, DecayChannel::Para2G)
    ));
    fDecayChannel = DecayChannel::Para2G;
  } else if (evtFractions[0] + evtFractions[1] > random) {
  // Direct 2G
    event->AddPrimaryVertex(GenerateTwoGammaVertex(
      vtxPosition, T0,
      material->GetLifetime(random - evtFractions[0], DecayChannel::Direct2G)
    ));
    fDecayChannel = DecayChannel::Direct2G;
  } else if (evtFractions[0] + evtFractions[1] + evtFractions[2] > random) {
  // oPs 2G
    event->AddPrimaryVertex(GenerateTwoGammaVertex(
      vtxPosition, T0,
      material->GetLifetime(random - evtFractions[0] - evtFractions[1], DecayChannel::Ortho2G)
    ));
    fDecayChannel = DecayChannel::Ortho2G;
  } else if (evtFractions[0] + evtFractions[1] + evtFractions[2] + evtFractions[3] > random) {
  // pPs 3G
    event->AddPrimaryVertex(GenerateThreeGammaVertex(
      DecayChannel::Para3G, vtxPosition, T0,
      material->GetLifetime(random - evtFractions[0] - evtFractions[1] - evtFractions[2], DecayChannel::Para3G)
    ));
    fDecayChannel = DecayChannel::Para3G;
  } else if (evtFractions[0] + evtFractions[1] + evtFractions[2] + evtFractions[3] + evtFractions[4] > random) {
  // Direct 3G
    event->AddPrimaryVertex(GenerateThreeGammaVertex(
      DecayChannel::Direct3G, vtxPosition, T0,
      material->GetLifetime(random - evtFractions[0] - evtFractions[1] - evtFractions[2] - evtFractions[3], DecayChannel::Direct3G)
    ));
    fDecayChannel = DecayChannel::Direct3G;
  } else {
  // oPs 3G
    event->AddPrimaryVertex(GenerateThreeGammaVertex(
      DecayChannel::Ortho3G, vtxPosition, T0,
      material->GetLifetime(random - evtFractions[0] - evtFractions[1] - evtFractions[2] - evtFractions[3] - evtFractions[4], DecayChannel::Ortho3G)
    ));
    fDecayChannel = DecayChannel::Ortho3G;
  } 

  //! Add prompt gamma from sodium
  G4ThreeVector promptVtxPosition = VertexUniformInCylinder(0.2 * cm, 0.2 * cm) + chamberCenter;
  event->AddPrimaryVertex(GeneratePromptGammaVertex(
    promptVtxPosition, 0.0f, MaterialParameters::fSodiumGammaTau,
    MaterialParameters::fSodiumGammaEnergy
  ));
}

/**
 * @param maximal dimension(/2) of annihilation chamber are taken (to speed up simulatons)
 * @return: vtx position, 2/3g ratio, meanlifetime;
 */
std::tuple<G4ThreeVector, MaterialExtension*>
PrimaryGenerator::GetVerticesDistributionInFilledSphere(
  const G4ThreeVector center, G4double radius
) {
  G4bool vertexFound = false;
  G4ThreeVector vertex(0 * cm, 0 * cm, 0 * cm);
  MaterialExtension* material;

  //! annihilation will occure only in materials where it was allowed
  //! @see MaterialExtension
  //! annihilation rate 2g/3g also depends on the material type
  while (!vertexFound) {
    vertex = GetRandomPointInFilledSphere(radius) + center;
    theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    material = dynamic_cast<MaterialExtension*>(
      theNavigator->LocateGlobalPointAndSetup(vertex)->GetLogicalVolume()->GetMaterial()
    );
    vertexFound = material->IsTarget();
  };
  return std::make_tuple(vertex, material);
}

std::tuple<G4ThreeVector, MaterialExtension*>
PrimaryGenerator::GetVerticesDistributionAlongStepVector(
  const G4ThreeVector center, const G4ThreeVector direction
) {
  G4bool lookForVtx = false;
  G4ThreeVector myPoint;
  G4ThreeVector myNextPoint = center;
  MaterialExtension* mat;
  while (!lookForVtx) {
    myPoint = myNextPoint;
    theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
    mat = dynamic_cast<MaterialExtension*>(
      theNavigator->LocateGlobalPointAndSetup(myPoint)->GetLogicalVolume()->GetMaterial()
    );
    lookForVtx = mat->IsTarget();
    myNextPoint = myPoint + direction;
  };
  return std::make_tuple(myPoint, mat);
}
void PrimaryGenerator::GenerateEvtLargeChamber(G4Event* event)
{
  G4ThreeVector chamberCenter = DetectorConstants::GetChamberCenter();
  G4ThreeVector vtxPosition;
  MaterialExtension* material;
  std::tie(vtxPosition, material) = GetVerticesDistributionAlongStepVector(
    chamberCenter, GetRandomPointInFilledSphere(1.0f * mm)
  );
  std::vector<G4double> evtFractions = material->GetEventsFraction();
  G4double random = G4UniformRand();

  //! for sodium: emitted positrons have up to 100~keV velocity
  //! therefore their speed v=sqrt(2*e/m) = 0.6c
  G4double T0 = (vtxPosition - chamberCenter).mag() / (0.6 * c_light);

  if (evtFractions[0] > random) {
  // pPs 2G
    event->AddPrimaryVertex(GenerateTwoGammaVertex(
      vtxPosition, T0,
      material->GetLifetime(random, DecayChannel::Para2G)
    ));
    fDecayChannel = DecayChannel::Para2G;
  } else if (evtFractions[0] + evtFractions[1] > random) {
  // Direct 2G
    event->AddPrimaryVertex(GenerateTwoGammaVertex(
      vtxPosition, T0,
      material->GetLifetime(random - evtFractions[0], DecayChannel::Direct2G)
    ));
    fDecayChannel = DecayChannel::Direct2G;
  } else if (evtFractions[0] + evtFractions[1] + evtFractions[2] > random) {
  // oPs 2G
    event->AddPrimaryVertex(GenerateTwoGammaVertex(
      vtxPosition, T0,
      material->GetLifetime(random - evtFractions[0] - evtFractions[1], DecayChannel::Ortho2G)
    ));
    fDecayChannel = DecayChannel::Ortho2G;
  } else if (evtFractions[0] + evtFractions[1] + evtFractions[2] + evtFractions[3] > random) {
  // pPs 3G
    event->AddPrimaryVertex(GenerateThreeGammaVertex(
      DecayChannel::Para3G, vtxPosition, T0,
      material->GetLifetime(random - evtFractions[0] - evtFractions[1] - evtFractions[2], DecayChannel::Para3G)
    ));
    fDecayChannel = DecayChannel::Para3G;
  } else if (evtFractions[0] + evtFractions[1] + evtFractions[2] + evtFractions[3] + evtFractions[4] > random) {
  // Direct 3G
    event->AddPrimaryVertex(GenerateThreeGammaVertex(
      DecayChannel::Direct3G, vtxPosition, T0,
      material->GetLifetime(random - evtFractions[0] - evtFractions[1] - evtFractions[2] - evtFractions[3], DecayChannel::Direct3G)
    ));
    fDecayChannel = DecayChannel::Direct3G;
  } else {
  // oPs 3G
    event->AddPrimaryVertex(GenerateThreeGammaVertex(
      DecayChannel::Ortho3G, vtxPosition, T0,
      material->GetLifetime(random - evtFractions[0] - evtFractions[1] - evtFractions[2] - evtFractions[3] - evtFractions[4], DecayChannel::Ortho3G)
    ));
    fDecayChannel = DecayChannel::Ortho3G;
  } 

  //! Add prompt gamma from sodium
  G4ThreeVector promptVtxPosition = VertexUniformInCylinder(0.2 * cm, 0.2 * cm) + chamberCenter;
  event->AddPrimaryVertex(GeneratePromptGammaVertex(
    promptVtxPosition, 0.0f, MaterialParameters::fSodiumGammaTau,
    MaterialParameters::fSodiumGammaEnergy
  ));
}

void PrimaryGenerator::GenerateBeam(BeamParams* beamParams, G4Event* event)
{
  G4ThreeVector vtxCoor = beamParams->GetVtx();
  G4PrimaryVertex* vertex = new G4PrimaryVertex(vtxCoor, 0);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");
  const G4double ene = beamParams->GetEnergy();
  G4ThreeVector momentum = beamParams->GetMomentum();
  G4double px = ene * momentum.x();
  G4double py = ene * momentum.y();
  G4double pz = ene * momentum.z();
  G4PrimaryParticle* particle1 = new G4PrimaryParticle(particleDefinition, px, py, pz, ene);
  PrimaryParticleInformation* infoParticle = new PrimaryParticleInformation();
  infoParticle->SetGammaMultiplicity(PrimaryParticleInformation::kPromptGamma);
  infoParticle->SetGeneratedGammaMultiplicity(PrimaryParticleInformation::kPromptGamma);
  infoParticle->SetIndex(0);
  infoParticle->SetGenMomentum(px, py, pz);
  particle1->SetUserInformation(infoParticle);
  vertex->SetPrimary(particle1);
  VtxInformation* infoPrompt = new VtxInformation();
  vertex->SetUserInformation(infoPrompt);
  infoPrompt->SetPromptGammaGen(true);
  infoPrompt->SetVtxPosition(vtxCoor.x(), vtxCoor.y(), vtxCoor.z());
  event->AddPrimaryVertex(vertex);
}

void PrimaryGenerator::GenerateIsotope(SourceParams* sourceParams, G4Event* event)
{
  G4ThreeVector vtxPosition;
  if (sourceParams->GetShape() == "cylinder") {
    vtxPosition =
      VertexUniformInCylinder(sourceParams->GetShapeDim(0), sourceParams->GetShapeDim(1))
      + sourceParams->GetShapeCenterPosition();
  }
  if (sourceParams->GetGammasNumber() == 1) {
    event->AddPrimaryVertex(GeneratePromptGammaVertex(
      vtxPosition, 0.0f, MaterialParameters::fSodiumGammaTau,
      MaterialParameters::fSodiumGammaEnergy
    ));
  } else if (sourceParams->GetGammasNumber() == 2) {
    //! generate 2g
    event->AddPrimaryVertex(GenerateTwoGammaVertex(
      vtxPosition, 0.0f, MaterialParameters::fTauBulk
    ));
  } else if (sourceParams->GetGammasNumber() == 3) {
    //! generate 3g
    event->AddPrimaryVertex(GenerateThreeGammaVertex(
      DecayChannel::Ortho3G ,vtxPosition, 0.0f, MaterialParameters::foPsTauVaccum
    ));
  } else {
    G4Exception(
      "PrimaryGenerator", "PG01", FatalException,
      "program does not know how many gamma quanta is needed to be simulated"
    );
  }
}

/**
 * Generation for NEMA studies
 * Positions - coordinate system centered in the middle of the detector
 *  for(z={0,-3/4L}){
 *    y = 1, 10, 20 cm
 *    x =0
 *  }
 *  20      3       6
 *  10      2       5
 *  1       1       4
 *  z ------0------3/4L ------
 */
void PrimaryGenerator::GenerateNema(G4Event* event, NemaGenerator* nemaGen)
{
  NemaPoint nemaPoint = nemaGen->GetRandomPoint();
  
  G4ThreeVector nemaPosition = nemaPoint.position;
  G4ThreeVector vtxPosition = VertexUniformInCylinder(nemaPoint.sizeOfPoint.getX(), nemaPoint.sizeOfPoint.getZ());
  if (nemaPoint.shapeOfPointInX.getX() != 0) {
    double directionX = nemaPoint.shapeOfPointInX.getX();
    double lengthX = nemaPoint.shapeOfPointInX.getZ()*nemaPoint.sizeOfPoint.getZ();
    double normLengthX = 1 + fabs(lengthX/(2*nemaPoint.sizeOfPoint.getZ()-fabs(lengthX)));
    normLengthX = normLengthX*nemaPoint.sizeOfPoint.getZ();
    double xAdd = pow(1 - fabs(vtxPosition.getZ() - nemaPoint.sizeOfPoint.getZ())/normLengthX, nemaPoint.shapeOfPointInX.getY());
    vtxPosition = vtxPosition - G4ThreeVector(directionX*xAdd, 0, 0);
  }
  if (nemaPoint.shapeOfPointInY.getX() != 0) {
    double directionY = nemaPoint.shapeOfPointInY.getX();
    double lengthY = nemaPoint.shapeOfPointInY.getZ()*nemaPoint.sizeOfPoint.getZ();
    double normLengthY = 1 + fabs(lengthY/(2*nemaPoint.sizeOfPoint.getZ()-fabs(lengthY)));
    normLengthY = normLengthY*nemaPoint.sizeOfPoint.getZ();
    double yAdd = pow(1 - fabs(vtxPosition.getZ() - nemaPoint.sizeOfPoint.getZ())/normLengthY, nemaPoint.shapeOfPointInY.getY());
    vtxPosition = vtxPosition - G4ThreeVector(0, directionY*yAdd, 0);
  }
  if (nemaPoint.orientationOfPoint.mag() > 0) {
    double theta = nemaPoint.orientationOfPoint.getX() * M_PI/180.;
    double phi = nemaPoint.orientationOfPoint.getY() * M_PI/180.;
    if (theta != 0. || phi != 0.) {
      TVector3 normal(cos(theta)*sin(phi), sin(theta)*sin(phi), cos(phi));
      TVector3 z(0.,0.,1.);
      TVector3 rotAxis = z.Cross(normal);
      double angle = normal.Angle(z); // radians
      TRotation rot;
      rot.Rotate(angle, rotAxis);
      TVector3 tempVector(vtxPosition.getX(), vtxPosition.getY(), vtxPosition.getZ());
      tempVector = rot*tempVector;
      vtxPosition.setX(tempVector.X());
      vtxPosition.setY(tempVector.Y());
      vtxPosition.setY(tempVector.Z());
    }
  }
  vtxPosition = vtxPosition + nemaPosition;

  double lifetime = nemaPoint.lifetime;
  bool is3GAllowed = nemaPoint.is3GAllowed;
  if (is3GAllowed) {
    double randLF = G4UniformRand();
    double lfTest = MaterialParameters::GetoPsIntensity3G(lifetime * ns, 100);
    if (randLF > lfTest) {
      event->AddPrimaryVertex(GenerateTwoGammaVertex(
        vtxPosition, 0.0f, lifetime
      ));
    } else {
      event->AddPrimaryVertex(GenerateThreeGammaVertex(
        DecayChannel::Ortho3G, vtxPosition, 0.0f, lifetime
      ));
    }
  } else {
    event->AddPrimaryVertex(GenerateTwoGammaVertex(
      vtxPosition, 0.0f, lifetime
    ));
  }
  
  event->AddPrimaryVertex(GeneratePromptGammaVertex(
    vtxPosition, 0.0f, MaterialParameters::fSodiumGammaTau,
    MaterialParameters::fSodiumGammaEnergy
  ));
}
  
G4ThreeVector PrimaryGenerator::VertexUniformInCylinder(G4double rIn, G4double zmax)
{
  G4double r = std::sqrt(pow(rIn, 2) * G4UniformRand());
  //! alpha uniform in (0, 2*pi)
  G4double alpha = twopi * G4UniformRand();
  G4double ux = std::cos(alpha);
  G4double uy = std::sin(alpha);
  //! z uniform in (-zmax, +zmax)
  G4double z = zmax * (2 * G4UniformRand() - 1);
  G4ThreeVector positionA(r * ux, r * uy, z);
  return positionA;
}

const G4ThreeVector PrimaryGenerator::GetRandomPointInFilledSphere(G4double radius)
{
  G4double theta = 2 * M_PI * G4UniformRand();
  G4double phi = acos(1 - 2 * G4UniformRand());
  G4double r = radius * cbrt(G4UniformRand());
  G4double x = r * sin(phi) * cos(theta);
  G4double y = r * sin(phi) * sin(theta);
  G4double z = r * cos(phi);
  return G4ThreeVector(x, y, z);
}

const G4ThreeVector PrimaryGenerator::GetRandomPointOnSphere(G4double radius)
{
  G4double theta = 2 * M_PI * G4UniformRand();
  G4double phi = acos(1 - 2 * G4UniformRand());
  G4double x = radius * sin(phi) * cos(theta);
  G4double y = radius * sin(phi) * sin(theta);
  G4double z = radius * cos(phi);
  return G4ThreeVector(x, y, z);
}

G4double PrimaryGenerator::calculate_mQED(const DecayChannel channel, Double_t mass_e, Double_t w1, Double_t w2, Double_t w3)
{
  if (channel == DecayChannel::Para3G) {
    return pow(w1*w2*w3,2)* pow(sin(acos((-pow(w1,2) - pow(w2,2) + pow(w3,2))/(2*w1*w2))) +
				       sin(acos(( pow(w1,2) - pow(w2,2) - pow(w3,2))/(2*w3*w2))) +
				       sin(acos((-pow(w1,2) + pow(w2,2) - pow(w3,2))/(2*w1*w3))), 2)* 
				       (pow(mass_e-w3,2)*pow(w1-w2,2) + pow(mass_e-w1,2)*pow(w2-w3,2) + pow(mass_e-w2,2)*pow(w3-w1,2));
  } else {
    return pow((mass_e - w1) / (w2 * w3), 2) + pow((mass_e - w2) / (w1 * w3), 2) + pow((mass_e - w3) / (w1 * w2), 2);
  }
  return 0;
}
