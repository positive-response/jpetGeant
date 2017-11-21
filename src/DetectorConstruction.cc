#include "DetectorConstruction.hh"


DetectorConstruction::DetectorConstruction()
:  G4VUserDetectorConstruction()
{
    InitializeMaterials();
}

DetectorConstruction::~DetectorConstruction()
{

}


G4VPhysicalVolume* DetectorConstruction::Construct() 
{
//    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(2.*world_hx);

    // world 
     worldSolid  = new G4Box("world", world_hx, world_hy, world_hz);
     worldLogical  = new G4LogicalVolume(worldSolid,air,"worldLogical");  
     worldPhysical = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,false,0,checkOverlaps);                
    // scintillaotrs
     ConstructScintillators();

    // FRAME 
      ConstructFrame();



    return worldPhysical;
}


void DetectorConstruction::ConstructScintillators()
{
    // scintillator
    G4Box* scinBox = new G4Box("scinBox", scinDim_x/2.0 ,scinDim_y/2.0 , scinDim_z/2.0 );
    scinLog = new G4LogicalVolume(scinBox, scinMaterial , "scinLogical");
    G4VisAttributes* BoxVisAtt =  new G4VisAttributes(G4Colour(0.3,0.4,.9));
    BoxVisAtt->SetForceWireframe(true);
    BoxVisAtt->SetForceSolid(true);
    scinLog->SetVisAttributes(BoxVisAtt);

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
}

void DetectorConstruction::InitializeMaterials()
{
    // define material
    G4NistManager* nistManager = G4NistManager::Instance();
    nistManager->FindOrBuildMaterial("G4_AIR");   
    nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    nistManager->FindOrBuildMaterial("G4_Al");

    air = G4Material::GetMaterial("G4_AIR"); 
    scinMaterial      =G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    detectorMaterial  =G4Material::GetMaterial("G4_Al");

}

void DetectorConstruction::ConstructFrame()
{
     LoadCAD( "stl_geometry/frame_side_A.stl" );
     LoadCAD( "stl_geometry/frame_side_B.stl" );
     LoadCAD( "stl_geometry/spojnik_1.stl" );
     LoadCAD( "stl_geometry/spojnik_2.stl" );
     LoadCAD( "stl_geometry/spojnik_3.stl" );
     LoadCAD( "stl_geometry/spojnik_4.stl" );
     LoadCAD( "stl_geometry/spornik_dlugi_1.stl" );
     LoadCAD( "stl_geometry/spornik_dlugi_2.stl" );
}

void DetectorConstruction::LoadCAD( const char* fileName)
{
     G4ThreeVector  offset = G4ThreeVector(0, 0, 0);
     //CADMesh * mesh = new CADMesh((char*) "stl_geometry/frame_side_A.stl");
     CADMesh * mesh = new CADMesh((char*) fileName);
     mesh->SetScale(m);
     G4VSolid* cad_solid = mesh->TessellatedMesh();
     G4LogicalVolume * cad_logical = new G4LogicalVolume(cad_solid, detectorMaterial, "cad_logical", 0, 0, 0);
     G4VisAttributes* DetVisAtt =  new G4VisAttributes(G4Colour(0.9,0.9,.9));
     DetVisAtt->SetForceWireframe(true);
     DetVisAtt->SetForceSolid(true);
     cad_logical->SetVisAttributes(DetVisAtt);
     G4VPhysicalVolume * cad_physical = new G4PVPlacement(0, G4ThreeVector(), cad_logical, "cad_physical", worldLogical, false, checkOverlaps);

}

void DetectorConstruction::ConstructSDandField()
{
    G4String detectorName = "/mydet/detector";
    DetectorSD * detectorSD = new DetectorSD(detectorName);
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    SetSensitiveDetector(scinLog,detectorSD);
}
