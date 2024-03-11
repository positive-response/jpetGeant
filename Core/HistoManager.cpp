/**
 *  @copyright Copyright 2021 The J-PET Monte Carlo Authors. All rights reserved.
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
 *  @file HistoManager.cpp
 */

#include "../Info/PrimaryParticleInformation.h"
#include "DetectorConstants.h"
#include "HistoManager.h"

#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>
#include <vector>

HistoManager::HistoManager() : fMakeControlHisto(true)
{
  fTempDecayTree = new JPetGeantDecayTree();
  fEventPack = new JPetGeantEventPack();
  fGeantInfo = fEventPack->GetEventInformation();
  fDecayChannel = DecayChannel::kUnknown;
}

HistoManager::~HistoManager() {}

void HistoManager::createHistogramWithAxes(
  TObject* object, TString xAxisName, TString yAxisName, TString zAxisName
) {
  TClass* cl = object->IsA();
  if (cl->InheritsFrom("TH1D")) {
    TH1D* tempHisto = static_cast<TH1D*>(object);
    tempHisto->GetXaxis()->SetTitle(xAxisName);
    tempHisto->GetYaxis()->SetTitle(yAxisName);
  } else if (cl->InheritsFrom("TH2D")) {
    TH2D* tempHisto = static_cast<TH2D*>(object);
    tempHisto->GetXaxis()->SetTitle(xAxisName);
    tempHisto->GetYaxis()->SetTitle(yAxisName);
  } else if (cl->InheritsFrom("TH3D")) {
    TH3D* tempHisto = static_cast<TH3D*>(object);
    tempHisto->GetXaxis()->SetTitle(xAxisName);
    tempHisto->GetYaxis()->SetTitle(yAxisName);
    tempHisto->GetZaxis()->SetTitle(zAxisName);
  }
  fStats.Add(object);
}

void HistoManager::fillHistogram(
  const char* name, double xValue, doubleCheck yValue, doubleCheck zValue
) {
  TObject* tempObject = getObject<TObject>(name);
  if (!tempObject) {
    writeError(name, " does not exist");
    return;
  }
  TClass* cl = tempObject->IsA();
  if (cl->InheritsFrom("TH1D")) {
    TH1D* tempHisto = static_cast<TH1D*>(tempObject);
    tempHisto->Fill(xValue);
  } else if (cl->InheritsFrom("TH2D")) {
    TH2D* tempHisto = static_cast<TH2D*>(tempObject);
    if (yValue.isChanged) {
      tempHisto->Fill(xValue, yValue.value);
    } else {
      writeError(name, " does not received argument for Y axis");
    }
  } else if (cl->InheritsFrom("TH3D")) {
    TH3D* tempHisto = static_cast<TH3D*>(tempObject);
    if (zValue.isChanged) {
      tempHisto->Fill(xValue, yValue.value, zValue.value);
    } else if (yValue.isChanged) {
      writeError(name, " does not received argument for Y and Z axis");
    } else {
      writeError(name, " does not received argument for Z axis");
    }
  } else {
    writeError(name, " is not of any of used types (TH1D, TH2D, TH3D)");
  }
}

void HistoManager::Book()
{
  if (fBookStatus) return;

  G4String fileName = "mcGeant.root";

  if (fEvtMessenger->AddDatetime()) {
    TDatime* now = new TDatime();
    std::string a_year = std::to_string(now->GetYear());
    std::string a_month = std::to_string(now->GetMonth());
    if (a_month.length() == 1) {
      a_month = std::string("0") + a_month;
    }
    std::string a_day = std::to_string(now->GetDay());
    if (a_day.length() == 1) {
      a_day = std::string("0") + a_day;
    }
    std::string a_hour = std::to_string(now->GetHour());
    if (a_hour.length() == 1) {
      a_hour = std::string("0") + a_hour;
    }
    std::string a_minute = std::to_string(now->GetMinute());
    if (a_minute.length() == 1) {
      a_minute = std::string("0") + a_minute;
    }
    std::string a_second = std::to_string(now->GetSecond());
    if (a_second.length() == 1) {
      a_second = std::string("0") + a_second;
    }
    std::string dateTime =
      a_year + "_" + a_month + "_" + a_day + "-"
      + a_hour + "_" + a_minute + "_" + a_second;
    fileName = dateTime + "." + fileName;
  }

  fRootFile = new TFile(fileName, "RECREATE");
  if (!fRootFile) {
    G4cout << " HistoManager::Book :" << " problem creating the ROOT TFile " << G4endl;
    return;
  }

  Int_t bufsize = 32000;
  Int_t splitlevel = 2;

  fTree = new TTree("T", "Tree keeps output from Geant simulation", splitlevel);
  //! autosave when 1 Gbyte written
  fTree->SetAutoSave(1000000000);
  fBranchEventPack = fTree->Branch("eventPack", &fEventPack, bufsize, splitlevel);

  if (GetMakeControlHisto()) BookHistograms();
  fBookStatus = true;
}

void HistoManager::SaveEvtPack() 
{
  if (!fEmptyEvent) {
    JPetGeantDecayTree* newDecayTree = fEventPack->ConstructNextDecayTree();
    newDecayTree->Clear("C");
    newDecayTree->CopyDecayTree(fTempDecayTree);
  }
  fTree->Fill();
  fTempDecayTree->Clear("C");
  fEmptyEvent = true;
}

void HistoManager::BookHistograms()
{
  createHistogramWithAxes(
    new TH1D("gen_gamma_multiplicity", "Generated gammas multiplicity. Bin size: 1", 10, -0.5, 9.5),
    "Gamma quanta multiplicity: 1=prompt; 2=2g; 3=3g", "Entries"
  );

  createHistogramWithAxes(
    new TH1D("gen_hit_time", "Generated hit time. Bin size: 150 ps", 100, -75.0, 14925.0),
    "Hit-times in scintillators [ps]", "Entries"
  );

  createHistogramWithAxes(
    new TH1D("gen_hit_eneDepos", "Generated hit energy deposition. Bin size: 2 keV", 750, -1.0, 1499.0),
    "Deposited energy in scintillators [keV]", "Entries"
  );

  createHistogramWithAxes(
    new TH1D("gen_hits_z_pos", "Generated hits Z position. Bin size: 1 cm", 120, -59.5, 60.5),
    "Hit-position along Z [cm]", "Entries"
  );

  createHistogramWithAxes(
    new TH2D("gen_hits_xy_pos", "Generated hits XY positions. Bin size: 1 cm x 1 cm", 120, -59.5, 60.5, 120, -59.5, 60.5),
    "Hit-position X [cm]", "Hit-position Y [cm]"
  );

  createHistogramWithAxes(
    new TH1D("gen_lifetime", "Generated lifetime. Bin size: 100 ps", 2000, -50.0, 199950.0),
    "Lifetime (2/3g) [ps]", "Entries"
  );

  createHistogramWithAxes(
    new TH1D("gen_prompt_lifetime", "Gen prompt lifetime. Bin size: 10 ps", 100, -5.0, 995.0),
    "Lifetime prompt gamma [ps]", "Entries"
  );

  createHistogramWithAxes(
    new TH2D("gen_XY", "Generated XY coordinates of annihilation point. Bin size: 0.1 cm x 0.1 cm", 
             500, -24.95, 25.05, 500, -24.95, 25.05), "Annihilation point (2/3g) X [cm]", "Annihilation point (2/3g) Y [cm]"
  );

  createHistogramWithAxes(
    new TH2D("gen_XZ", "Generated XZ coordinates of annihilation point. Bin size: 0.1 cm x 0.1 cm", 
             500, -24.95, 25.05, 1200, -59.95, 60.05), "Annihilation point (2/3g) X [cm]", "Annihilation point (2/3g) Z [cm]"
  );

  createHistogramWithAxes(
    new TH2D("gen_YZ", "Generated YZ coordinates of annihilation point. Bin size: 0.1 cm x 0.1 cm", 
             500, -24.95, 25.05, 1200, -59.95, 60.05), "Annihilation point (2/3g) Y [cm]", "Annihilation point (2/3g) Z [cm]"
  );

  createHistogramWithAxes(
    new TH2D("gen_X_vs_lifetime", "Generated X coordinates of annihilation point vs lifetime. Bin size: 1 cm x 100 ns", 
             50, -24.5, 25.5, 2000, -50.0, 199950.0), "Annihilation point X [cm]", "Generated lifetime [ns]"
  );
  
  createHistogramWithAxes(
    new TH2D("gen_Y_vs_lifetime", "Generated Y coordinates of annihilation point vs lifetime. Bin size: 1 cm x 100 ns", 
             50, -24.5, 25.5, 2000, -50.0, 199950.0), "Annihilation point Y [cm]", "Generated lifetime [ns]"
  );
  
  createHistogramWithAxes(
    new TH2D("gen_Z_vs_lifetime", "Generated Z coordinates of annihilation point vs lifetime. Bin size: 1 cm x 100 ns", 
             120, -59.5, 60.5, 2000, -50.0, 199950.0), "Annihilation point Z [cm]", "Generated lifetime [ns]"
  );
  
  createHistogramWithAxes(
    new TH2D("gen_prompt_XY", "Generated XY coordinates of annihilation point. Bin size: 0.1 cm x 0.1 cm", 
             500, -24.95, 25.05, 500, -24.95, 25.05), "Prompt emission point X [cm]", "Prompt emission point Y [cm]"
  );

  createHistogramWithAxes(
    new TH2D("gen_prompt_XZ", "Generated XZ coordinates of annihilation point. Bin size: 0.1 cm x 0.1 cm", 
             500, -24.95, 25.05, 1200, -59.95, 60.05), "Prompt emission point X [cm]", "Prompt emission point Z [cm]"
  );

  createHistogramWithAxes(
    new TH2D("gen_prompt_YZ", "Generated YZ coordinates of annihilation point. Bin size: 0.1 cm x 0.1 cm", 
             500, -24.95, 25.05, 1200, -59.95, 60.05), "Prompt emission point Y [cm]", "Prompt emission point Z [cm]"
  );

  createHistogramWithAxes(
    new TH2D("gen_3g_angles", "Generated angles of 3g. Bin size: 1 deg x 1 deg", 190, -0.5, 189.5, 190, -0.5, 189.5),
    "#Theta_{12} [degree]", "#Theta_{23} [degree]"
  );

  createHistogramWithAxes(
    new TH2D("gen_energy", "Generated energy of 3g. Bin size: 5 keV x 5 keV", 120, -2.5, 597.5, 120, -2.5, 597.5),
    "E_1 [keV]", "E_2 [keV]"
  );

  createHistogramWithAxes(
    new TH1D("gen_g_ene", "Generated energy. Bin size: 5 keV", 300, -2.5, 1497.5),
    "E_1 generated [keV]", "Entries"
  );


  createHistogramWithAxes(
    new TH2D(
      "gen_gamma_multiplicity_vs_lifetime",
      "Generated gammas multiplicity vs generated lifetime. Bin size: 1 x 100 ps",
      10, -0.5, 9.5, 1000, -50.0, 99950.0
    ),
    "Gamma quanta multiplicity: 2=2g; 3=3g", "Lifetime (2/3g) [ps]"
  );
  
  createHistogramWithAxes(
    new TH1D("gen_hits_multiplicity", "Multiplicity of the hit. Bin size: 1", 3000, -0.5, 2999.5),
    "Multiplicity of the hit", "Entries"
  );
  
  createHistogramWithAxes(
    new TH1D("gen_multiplicity_vs_theta", "Multiplicity of the scintillators theta angle", 364, -M_PI - 1.5, M_PI + 2.5),
    "Multiplicity", "Theta"
  );
  
  createHistogramWithAxes(
    new TH1D("cosm_theta", "Cosmics: theta angle", 184, -M_PI/2 - 2.5, M_PI/2 + 1.5),
    "theta [rad]", "number of entries"
  );
  createHistogramWithAxes(
    new TH2D("cosm_vtx_xy", "Cosmics: generated vertex point XY",
    204*(DetectorConstants::world_size[1]/m), -1.015*DetectorConstants::world_size[1], 1.025*DetectorConstants::world_size[1],
    204*(DetectorConstants::world_size[0]/m), -1.015*DetectorConstants::world_size[0], 1.025*DetectorConstants::world_size[0]),
    "Y position [cm]", "X position [cm]"
  );

  createHistogramWithAxes(
    new TH2D("cosm_vtx_xz", "Cosmics: generated vertex point XZ",
    204*(DetectorConstants::world_size[2]/m), -1.015*DetectorConstants::world_size[2], 1.025*DetectorConstants::world_size[2],
    204*(DetectorConstants::world_size[0]/m), -1.015*DetectorConstants::world_size[0], 1.025*DetectorConstants::world_size[0]),
    "Z position [cm]", "X position [cm]"
  );

  createHistogramWithAxes(
    new TH2D("cosm_vtx_yz", "Cosmics: generated vertex point YZ",
    204*(DetectorConstants::world_size[1]/m), -1.015*DetectorConstants::world_size[1], 1.025*DetectorConstants::world_size[1],
    204*(DetectorConstants::world_size[2]/m), -1.015*DetectorConstants::world_size[2], 1.025*DetectorConstants::world_size[2]),
    "Y position [cm]", "Z position [cm]"
  );
}

void HistoManager::FillHistoGenInfo(const G4Event* anEvent)
{
  for (int i = 0; i < anEvent->GetNumberOfPrimaryVertex(); i++) {
    VtxInformation* info = dynamic_cast<VtxInformation*>(
      anEvent->GetPrimaryVertex(i)->GetUserInformation()
    );
    if (info != 0) AddGenInfo(info);

    for (int j = 0; j < anEvent->GetPrimaryVertex(i)->GetNumberOfParticle(); j++) {
      G4PrimaryParticle* particle = anEvent->GetPrimaryVertex(i)->GetPrimary(j);
      if (particle != nullptr) {
        AddGenInfoParticles(particle);
      }
    }
  }

  double theta_12 = (180. / TMath::Pi()) * (fGeantInfo->GetMomentumGamma(1)).Angle(fGeantInfo->GetMomentumGamma(2));
  double theta_23 = (180. / TMath::Pi()) * (fGeantInfo->GetMomentumGamma(2)).Angle(fGeantInfo->GetMomentumGamma(3));

  fillHistogram("gen_3g_angles", theta_12, doubleCheck(theta_23));
  fillHistogram("gen_energy", fGeantInfo->GetMomentumGamma(1).Mag(), doubleCheck(fGeantInfo->GetMomentumGamma(2).Mag()));
}

void HistoManager::FillCosmicInfo(G4double theta, G4ThreeVector init, G4ThreeVector orig)
{
  fillHistogram("cosm_theta", theta);
  fillHistogram("cosm_vtx_xy", init.y(), doubleCheck(init.x()));
  fillHistogram("cosm_vtx_xz", init.z(), doubleCheck(init.x()));
  fillHistogram("cosm_vtx_yz", init.z(), doubleCheck(init.y()));
}

void HistoManager::AddGenInfoParticles(G4PrimaryParticle* particle)
{
  PrimaryParticleInformation* infoParticle = static_cast<PrimaryParticleInformation*>(
    particle->GetUserInformation()
  );
  if (infoParticle == nullptr) { return; }
  G4int index = infoParticle->GetIndex();
  G4ThreeVector genMom = infoParticle->GenGenMomentum();
  fGeantInfo->SetMomentumGamma(index, genMom.x() / keV, genMom.y() / keV, genMom.z() / keV);
}

/**
 * Note about units systems:
 *         time | distance |  energy
 * Geant:   ns  |  mm      |   MeV
 * J-PET:   ps  |  cm      |   keV
 */
void HistoManager::AddGenInfo(VtxInformation* info)
{
  bool is3g = info->GetThreeGammaGen();
  bool is2g = info->GetTwoGammaGen();
  bool isPrompt = info->GetPromptGammaGen();
  bool isCosmic = info->GetCosmicGammaGen();
  bool is5g = info->GetFiveGammaGen();
  
  if (is2g || is3g) {
	  fGeantInfo->SetThreeGammaGen(is5g);
    fGeantInfo->SetThreeGammaGen(is3g);
    fGeantInfo->SetTwoGammaGen(is2g);
    fGeantInfo->SetVtxPosition(info->GetVtxPositionX() / cm, info->GetVtxPositionY() / cm, info->GetVtxPositionZ() / cm);
    fGeantInfo->SetLifetime(info->GetLifetime() / ps);
    fGeantInfo->SetRunNr(info->GetRunNr());

    if (GetMakeControlHisto()) {
      if (is2g) {
        fillHistogram("gen_gamma_multiplicity", 2);
        fillHistogram("gen_gamma_multiplicity_vs_lifetime", 2, doubleCheck(info->GetLifetime() / ps));
      }
      if (is3g) {
        fillHistogram("gen_gamma_multiplicity", 3);
        fillHistogram("gen_gamma_multiplicity_vs_lifetime", 3, doubleCheck(info->GetLifetime() / ps));
      }
      fillHistogram("gen_lifetime", info->GetLifetime() / ps);
      fillHistogram("gen_XY", info->GetVtxPositionX() / cm, doubleCheck(info->GetVtxPositionY() / cm));
      fillHistogram("gen_XZ", info->GetVtxPositionX() / cm, doubleCheck(info->GetVtxPositionZ() / cm));
      fillHistogram("gen_YZ", info->GetVtxPositionY() / cm, doubleCheck(info->GetVtxPositionZ() / cm));
      fillHistogram("gen_X_vs_lifetime", info->GetVtxPositionX() / cm, doubleCheck(info->GetLifetime() / ps));
      fillHistogram("gen_Y_vs_lifetime", info->GetVtxPositionY() / cm, doubleCheck(info->GetLifetime() / ps));
      fillHistogram("gen_Z_vs_lifetime", info->GetVtxPositionZ() / cm, doubleCheck(info->GetLifetime() / ps));
    }
  }

  if (isPrompt) {
    fGeantInfo->SetPromptGammaGen(isPrompt);
    fGeantInfo->SetPromptLifetime(info->GetLifetime() / ps);
    fGeantInfo->SetVtxPromptPosition(
      info->GetVtxPositionX() / cm, info->GetVtxPositionY() / cm, info->GetVtxPositionZ() / cm
    );
    fGeantInfo->SetRunNr(info->GetRunNr());

    if (GetMakeControlHisto()) {
      fillHistogram("gen_gamma_multiplicity", 1);
      fillHistogram("gen_prompt_lifetime", info->GetLifetime() / ps);
      fillHistogram("gen_prompt_XY", info->GetVtxPositionX() / cm, doubleCheck(info->GetVtxPositionY() / cm));
      fillHistogram("gen_prompt_XZ", info->GetVtxPositionX() / cm, doubleCheck(info->GetVtxPositionZ() / cm));
      fillHistogram("gen_prompt_YZ", info->GetVtxPositionY() / cm, doubleCheck(info->GetVtxPositionZ() / cm));
    }
  }
  SetParentIDofPhoton(0);
  fEndOfEvent = true;
  
  if (isCosmic){
    fGeantInfo->setCosmicEventTag(true);
  }
}

void HistoManager::AddNewHit(DetectorHit* hit)
{
  JPetGeantScinHits* geantHit = fEventPack->ConstructNextHit();
  geantHit->Fill(
    fEventPack->GetEventNumber(), hit->GetScinID(), hit->GetTrackID(),
    hit->GetTrackPDG(), hit->GetNumInteractions(), hit->GetEdep()/keV,
    hit->GetTime()/ps
  );

  //! Converting G4Vector into root TVector3
  geantHit->SetHitPosition(
    hit->GetPosition().getX()/cm, hit->GetPosition().getY()/cm, hit->GetPosition().getZ()/cm
  );
  geantHit->SetPolarizationIn(
    hit->GetPolarizationIn().getX(), hit->GetPolarizationIn().getY(),
    hit->GetPolarizationIn().getZ()
  );
  geantHit->SetPolarizationOut(
    hit->GetPolarizationOut().getX(), hit->GetPolarizationOut().getY(),
    hit->GetPolarizationOut().getZ()
  );
  geantHit->SetMomentumIn(
    hit->GetMomentumIn().getX()/keV, hit->GetMomentumIn().getY()/keV,
    hit->GetMomentumIn().getZ()/keV
  );
  geantHit->SetMomentumOut(
    hit->GetMomentumOut().getX()/keV, hit->GetMomentumOut().getY()/keV,
    hit->GetMomentumOut().getZ()/keV
  );
  geantHit->SetGenGammaMultiplicity(hit->GetGenGammaMultiplicity());
  geantHit->SetGenGammaIndex(hit->GetGenGammaIndex());
  
  if (GetMakeControlHisto()) {
    fillHistogram("gen_hit_time", hit->GetTime()/ps);
    fillHistogram("gen_hit_eneDepos", hit->GetEdep()/keV);
    fillHistogram("gen_hits_z_pos", hit->GetPosition().getZ()/cm);
    fillHistogram("gen_hits_xy_pos", hit->GetPosition().getX()/cm, doubleCheck(hit->GetPosition().getY()/cm));
    fillHistogram("gen_hits_multiplicity", hit->GetGenGammaMultiplicity());
    if (hit->GetPosition().getX() > 0)
      fillHistogram("gen_multiplicity_vs_theta", atan(hit->GetPosition().getY() / hit->GetPosition().getX()));
    else if (hit->GetPosition().getX() < 0 && hit->GetPosition().getY() >= 0)
      fillHistogram("gen_multiplicity_vs_theta", M_PI + atan(hit->GetPosition().getY() / hit->GetPosition().getX()));
    else if (hit->GetPosition().getX() < 0 && hit->GetPosition().getY() <= 0)
      fillHistogram("gen_multiplicity_vs_theta", -M_PI + atan(hit->GetPosition().getY() / hit->GetPosition().getX()));
    else {
      double signCheck = (hit->GetPosition().getY() >= 0 ? 1 : -1);
      fillHistogram("gen_multiplicity_vs_theta", signCheck*M_PI/2);
    }
  }
}

void HistoManager::AddNodeToDecayTree(int nodeID, int trackID)
{
  if (!fEvtMessenger->GetCreateDecayTreeFlag())
    return;
  
  InteractionType interactionType = InteractionType::kSecondaryPart;
  
  if (nodeID - fParentIDofPhoton == 10)
    interactionType = InteractionType::kScattNonActivePart;
  else if (nodeID - fParentIDofPhoton == 100)
    interactionType = InteractionType::kScattActivePart;
  
  if (interactionType == InteractionType::kScattActivePart)
    fEmptyEvent = false;
  bool firstInteraction = (fParentIDofPhoton < 10 ? true : false);
  if (fEndOfEvent) {
    fEndOfEvent = false;
    fTempDecayTree->Clear("C");
    fTempDecayTree->SetEventNumber(GetEventNumber());
    fTempDecayTree->SetDecayChannel(fDecayChannel);
    if (firstInteraction) {
      fTempDecayTree->AddNodeToBranch(fParentIDofPhoton, trackID, InteractionType::kPrimaryGamma);
    }
    fTempDecayTree->AddNodeToBranch(nodeID, trackID, interactionType);
  } else {
    if (firstInteraction) {
      fTempDecayTree->AddNodeToBranch(fParentIDofPhoton, trackID, InteractionType::kPrimaryGamma);
    }
    fTempDecayTree->AddNodeToBranch(nodeID, trackID, interactionType);
  }
}

void HistoManager::Save()
{
  if (!fRootFile) return;
  fTree->Write();
  if (GetMakeControlHisto()) {
    TIterator* it = fStats.MakeIterator();
    TObject* obj;
    while ((obj = it->Next())) obj->Write();
  }
  fRootFile->Close();
  G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
}

void HistoManager::writeError(const char* nameOfHistogram, const char* messageEnd)
{
  std::string histName(nameOfHistogram);
  bool histExists = (fErrorCounts.find(histName) != fErrorCounts.end());
  if (!histExists) {
    fErrorCounts.insert(histName);
    G4cout << "!!![Error]!!!  -  Histogram with name "
    << histName << " " << messageEnd << G4endl;
  }
}
