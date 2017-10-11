#include "DetectorConstruction.hh"
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


DetectorConstruction::DetectorConstruction()
:  G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{

}


G4VPhysicalVolume* DetectorConstruction::Construct() 
{
    // define material
    G4NistManager* nistManager = G4NistManager::Instance();
    nistManager->FindOrBuildMaterial("G4_AIR");   
    nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4Material* air = G4Material::GetMaterial("G4_AIR"); 
    G4Material* scinMaterial  =G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4double world_hx = 1.0*m;
    G4double world_hy = 1.0*m;
    G4double world_hz = 1.0*m;

    G4bool checkOverlaps = true; 
    // world 
    G4Box* worldSolid
        = new G4Box("world", world_hx, world_hy, world_hz);
    G4LogicalVolume* worldLogical 
        = new G4LogicalVolume(worldSolid,air,"worldLogical");  
    G4VPhysicalVolume* worldPhysical
        = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,false,0,checkOverlaps);                
    const G4int layers = 3;
    const G4double scinDim_x = 1.9*cm;
    const G4double scinDim_y = 0.7*cm;
    const G4double scinDim_z = 50.0*cm;
    const G4double radius[layers] = {42.5*cm,46.75*cm,57.5*cm};
    const int nSegments[layers] = {48,48,96};

    // scintillator
    G4Box* scinBox = new G4Box("scinBox", scinDim_x/2.0 ,scinDim_y/2.0 , scinDim_z/2.0 );
    scinLog = new G4LogicalVolume(scinBox, scinMaterial , "scinLogical");


    G4int icopy = 0;

    for(int j=0;j<layers;j++){
        for(int i=0;i<nSegments[j];i++){
            G4double phi = i*2*M_PI/nSegments[j];
            G4double fi = M_PI/nSegments[j];


            if( j == 0 ){
                fi =0.;
            }

            G4RotationMatrix rot = G4RotationMatrix();
            rot.rotateZ(phi);

            G4ThreeVector loc = G4ThreeVector(radius[j]*(cos(phi+fi)),radius[j]*(sin(phi+fi)),0.0);
            G4Transform3D transform(rot,loc);

            new G4PVPlacement(transform,             //rotation,position
                              scinLog,            //its logical volume
                              "scin",             //its name
                               worldLogical,      //its mother (logical) volume
                              false,                 //no boolean operation
                              icopy,                 //copy number
                              checkOverlaps);       // checking overlaps 


            icopy++;

        }
    }


    return worldPhysical;
}


void DetectorConstruction::ConstructSDandField()
{
    G4String detectorName = "/mydet/detector";
    DetectorSD * detectorSD = new DetectorSD(detectorName);
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    SetSensitiveDetector(scinLog,detectorSD);
}
