#ifndef DetectorConstruction_h 
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "DetectorSD.hh"

#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4NistManager.hh"
#include "G4MaterialTable.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4SDManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "G4GeometryManager.hh"

#include "CADMesh.hh"
#include <string>



const G4double world_hx = 0.7*m;
const G4double world_hy = 0.7*m;
const G4double world_hz = 0.7*m;
const G4int layers = 3;
const G4double scinDim_x = 1.9*cm;
const G4double scinDim_y = 0.7*cm;
const G4double scinDim_z = 50.0*cm;
const G4double radius[layers] = {42.5*cm,46.75*cm,57.5*cm};
const int nSegments[layers] = {48,48,96};
const  G4bool checkOverlaps = true; 


class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField(); 

    private:
    void InitializeMaterials();
    void ConstructFrame();
    void ConstructScintillators();
    void LoadCAD( const char* fileName);


    G4Box* worldSolid;
    G4LogicalVolume* worldLogical;
    G4VPhysicalVolume* worldPhysical;

    G4Material* air; 
    G4Material* scinMaterial;    
    G4Material* detectorMaterial;

    G4LogicalVolume * scinLog;
};


#endif
