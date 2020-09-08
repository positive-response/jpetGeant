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
 *  @file DetectorConstruction.h
 */

#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H 1

#include "MaterialExtension.h"
#include "DetectorSD.h"

#include <G4VUserDetectorConstruction.hh>
#include <G4GeometryManager.hh>
#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4MaterialTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4VisAttributes.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4Material.hh>
#include <G4Element.hh>
#include <G4Colour.hh>
#include <CADMesh.hh>
#include <G4Cache.hh>
#include <globals.hh>
#include <G4Box.hh>
#include <vector>

class DetectorConstructionMessenger;
struct Layer;
struct Scin;
struct Slot;

//! Flag for debugging purposes
const G4bool checkOverlaps = false;

/**
 * @class DetectorConstruction
 * @brief creating detector; can read the CAD geometry
 */
class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  enum GeometryKind {
    Unknown,
    Geo24ModulesLayer,
    Geo24ModulesLayerDistributed
  };

  //! only single instance can exist
  static DetectorConstruction* GetInstance();
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  void LoadGeometryForRun(G4int nr);
  G4int ReturnNumberOfScintillators();
  void UpdateGeometry();
  void ReloadMaterials(const G4String& material);

  //! Basic geometry with 3 layers of scintillators
  void ConstructBasicGeometry(G4bool tf) { fLoadScintillators = tf; };
  void LoadFrame(G4bool tf) { fLoadCADFrame = tf; };

  //! Modular layer (known as 4th layer); 24 modules filled with scintillators
  void ConstructModularLayer(const G4String& module_name) {
    fLoadModularLayer = true;
    if (module_name == "Single") {
      fGeoKind = GeometryKind::Geo24ModulesLayer;
    } else if (module_name == "Double") {
      fGeoKind = GeometryKind::Geo24ModulesLayerDistributed;
    } else {
      fLoadModularLayer = false;
      fGeoKind = GeometryKind::Unknown;
    }
  }
  void CreateGeometryFileFlag(G4bool tf) { fCreateGeometryFile = tf; };
  void SetOldStyleOfGeometryFile(G4bool tf) { fCreateOldGeometryFileStyle = tf; };
  void CreateGeometryFile();
  
  G4int GetRunNumber() const { return fRunNumber; };

private:
  static G4ThreadLocal G4bool fConstructedSDandField;
  static DetectorConstruction* fInstance;

  DetectorConstruction();
  virtual ~DetectorConstruction();
  DetectorConstructionMessenger* fMessenger = nullptr;

  //! Load materials from NIST database
  void InitializeMaterials();
  //! Load detector elements from CAD files
  void ConstructFrameCAD();
  //! Create scintillators only; dimensions are right now fixed in code
  void ConstructScintillators();
  //! Construct modular layer inserted into detector (refered as 4th layer)
  void ConstructScintillatorsModularLayer();
  //! Create target used in run3 - big chamber no XAD material inside
  void ConstructTargetRun3();
  //! Create target used in run5 (and run4) - small chamber + XAD material inside
  void ConstructTargetRun5();
  //! Create target used in run6 - big chamber + XAD material inside tube
  void ConstructTargetRun6();
  //! Create target used in run7
  void ConstructTargetRun7();

  void ConstructLayers(
    std::vector<G4double>& radius_dynamic, G4int& numberofModules,
    G4double& AngDisp_dynamic, G4int& icopyI);

  //! Corresponds to JPET measurements; run 0 = user setup
  G4int fRunNumber;
  //! Flag for loading standard 3 layers of the scintillators
  G4bool fLoadScintillators;
  //! Flag for loading frame from CAD file
  G4bool fLoadCADFrame;
  //! Flag for loading wrapping tf the scintillators
  G4bool fLoadWrapping;
  //! Flag for loading modular (4th) layer
  G4bool fLoadModularLayer;
  //! Flag for creating file with geometry
  G4bool fCreateGeometryFile;
  //! Flag for choosing old style of geometry file
  G4bool fCreateOldGeometryFileStyle = false;

  G4Box* fWorldSolid = nullptr;
  G4LogicalVolume* fWorldLogical = nullptr;
  G4VPhysicalVolume* fWorldPhysical = nullptr;

  MaterialExtension* fAir = nullptr;
  MaterialExtension* fKapton = nullptr;
  MaterialExtension* fVacuum = nullptr;
  MaterialExtension* fPlexiglass = nullptr;
  MaterialExtension* fXADMaterial = nullptr;
  MaterialExtension* fScinMaterial = nullptr;
  MaterialExtension* fAluminiumMaterial = nullptr;
  MaterialExtension* fSmallChamberMaterial = nullptr;
  MaterialExtension* fSmallChamberRun7Material = nullptr;

  G4LogicalVolume* fScinLog = nullptr;
  G4LogicalVolume* fScinLogInModule = nullptr;
  G4Cache<DetectorSD*> fDetectorSD;
  //! Geometry Kind for the modular layer
  GeometryKind fGeoKind = GeometryKind::Unknown;
  //! Maximum ID of the scintillators
  G4int maxScinID = 512;
  
  std::vector<Layer> fLayerContainer;
  std::vector<Scin> fScinContainer;
  std::vector<Slot> fSlotContainer;
  G4int fLayerNumber = 0;
};

struct Layer {
  int fId;
  std::string fName;
  double fRadius;
  int fSetup_id;
  Layer(int id, const std::string& name, double radius, int setup_id) : fId(id), fName(name),
                                                fRadius(radius), fSetup_id(setup_id) {}
};

struct Scin {
  int fId;
  int fSlot_id;
  float fHeight;
  float fWidth;
  float fLength;
  double fX_center;
  double fY_center;
  double fZ_center;
  Scin(int id, int slot_id, double height, double width, double length, double x_center, double y_center,
        double z_center) : fId(id), fSlot_id(slot_id), fHeight(height), fWidth(width), fLength(length),
                            fX_center(x_center), fY_center(y_center), fZ_center(z_center) {}
};

struct Slot {
  int fId;
  int fLayer_id;
  double fTheta;
  std::string fType;
  Slot(int id, int layer_id, double theta, const std::string& type) : fId(id), fLayer_id(layer_id),
                                                fTheta(theta), fType(type) {}
};

void replace(std::string& json, const std::string& placeholder);
#endif /* !DETECTORCONSTRUCTION_H */
