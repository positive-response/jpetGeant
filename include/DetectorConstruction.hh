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



const G4double world_hx = 1.0*m; ///< max world size
const G4double world_hy = 1.0*m; ///< max world size
const G4double world_hz = 1.0*m; ///< max world size
const G4int layers = 3; ///< number of simulated laters in detector
const G4double scinDim_x = 1.9*cm; ///<  X dimension of simulated strip
const G4double scinDim_y = 0.7*cm; ///<  Y dimension of simulated strip
const G4double scinDim_z = 50.0*cm; ///<  Z dimension of simulated strip
const G4double radius[layers] = {42.5*cm,46.75*cm,57.5*cm}; ///< layer radius up to center of the strip
const int nSegments[layers] = {48,48,96}; ///< number of segments in each layer
const  G4bool checkOverlaps = false; ///< debugging purpose 

/**
* \class DetectorConstruction
* \brief creating detector; can read the CAD geometry 
*/
class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
    /// standard constructor
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField(); 

    private:
    /// load from NIST database
    void InitializeMaterials();
    ///  function is loading detector elements from CAD files
    void ConstructFrame();
    //// create only scintillators; dimensions are right now fixed in code
    void ConstructScintillators();
    /// rm me
    void LoadCAD( const char* fileName);


    G4Box* worldSolid;
    G4LogicalVolume* worldLogical;
    G4VPhysicalVolume* worldPhysical;

    G4Material* air; 
    G4Material* scinMaterial;    
    G4Material* detectorMaterial;

    G4LogicalVolume * scinLog;

    G4int CAD_icopy;

};


#endif
