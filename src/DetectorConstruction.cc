#include "DetectorConstruction.hh"
#include "G4UnionSolid.hh"


DetectorConstruction::DetectorConstruction()
:  G4VUserDetectorConstruction()
{
    InitializeMaterials();
    CAD_icopy = 0;
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
            rot.rotateZ(phi+fi);

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
    CAD_icopy++;
     G4RotationMatrix rot = G4RotationMatrix();
     G4ThreeVector loc = G4ThreeVector(0.,0.,0.0);
     G4Transform3D transform(rot,loc);

     G4VSolid* unionSolid; 

      CADMesh * mesh0 = new CADMesh((char*)"stl_geometry/frame_side_A.stl" );
      mesh0->SetScale(m);
      G4VSolid* cad_solid0 = mesh0->TessellatedMesh();

      CADMesh * mesh1 = new CADMesh((char*)"stl_geometry/frame_side_B.stl" );
      mesh1->SetScale(m);
      G4VSolid* cad_solid1 = mesh1->TessellatedMesh();
      unionSolid =  new G4UnionSolid("Union1", cad_solid1, cad_solid0);

      CADMesh * mesh2 = new CADMesh((char*) "stl_geometry/spojnik_1.stl");
      mesh2->SetScale(m);
      G4VSolid* cad_solid2 = mesh2->TessellatedMesh();
      unionSolid =  new G4UnionSolid("Union2", unionSolid, cad_solid2);

      CADMesh * mesh3 = new CADMesh((char*)"stl_geometry/spojnik_2.stl" );
      mesh3->SetScale(m);
      G4VSolid* cad_solid3 = mesh3->TessellatedMesh();
      unionSolid =  new G4UnionSolid("Union3", unionSolid, cad_solid3);

      CADMesh * mesh4 = new CADMesh((char*) "stl_geometry/spojnik_3.stl");
      mesh4->SetScale(m);
      G4VSolid* cad_solid4 = mesh4->TessellatedMesh();
      unionSolid =  new G4UnionSolid("Union4", unionSolid, cad_solid4);

      CADMesh * mesh5 = new CADMesh((char*)"stl_geometry/spojnik_4.stl");
      mesh5->SetScale(m);
      G4VSolid* cad_solid5 = mesh5->TessellatedMesh();
      unionSolid =  new G4UnionSolid("Union5", unionSolid, cad_solid5);

//      CADMesh * mesh6 = new CADMesh((char*) "stl_geometry/spornik_dlugi_1.stl");
//      mesh6->SetScale(m);
//      G4VSolid* cad_solid6 = mesh6->TessellatedMesh();
//      unionSolid =  new G4UnionSolid("Union6", unionSolid, cad_solid6);
//
//      CADMesh * mesh7 = new CADMesh((char*)"stl_geometry/spornik_dlugi_2.stl");
//      mesh7->SetScale(m);
//      G4VSolid* cad_solid7 = mesh7->TessellatedMesh();
//      unionSolid =  new G4UnionSolid("Union7", unionSolid, cad_solid7);
//


     G4LogicalVolume * cad_logical = new G4LogicalVolume(unionSolid, detectorMaterial, "cad_logical", 0, 0, 0);

    G4VisAttributes* DetVisAtt =  new G4VisAttributes(G4Colour(0.9,0.9,.9));
    DetVisAtt->SetForceWireframe(true);
    DetVisAtt->SetForceSolid(true);
    cad_logical->SetVisAttributes(DetVisAtt);

    new G4PVPlacement(transform, cad_logical, "cadGeom", worldLogical, false, 0, checkOverlaps);

}

void DetectorConstruction::LoadCAD( const char* fileName)
{
    CAD_icopy++;
     G4RotationMatrix rot = G4RotationMatrix();
     G4ThreeVector loc = G4ThreeVector(0.,0.,0.0);
     G4Transform3D transform(rot,loc);


     //CADMesh * mesh = new CADMesh((char*) "stl_geometry/frame_side_A.stl");
     CADMesh * mesh = new CADMesh((char*) fileName);
     mesh->SetScale(m);
     G4VSolid* cad_solid = mesh->TessellatedMesh();
     G4LogicalVolume * cad_logical = new G4LogicalVolume(cad_solid, detectorMaterial, "cad_logical", 0, 0, 0);
     G4VisAttributes* DetVisAtt =  new G4VisAttributes(G4Colour(0.9,0.9,.9));
     DetVisAtt->SetForceWireframe(true);
     DetVisAtt->SetForceSolid(true);
     cad_logical->SetVisAttributes(DetVisAtt);
     //G4VPhysicalVolume * cad_physical = new G4PVPlacement(0, G4ThreeVector(), cad_logical, "cad_physical", worldLogical, false, checkOverlaps);
     
     G4String name = "cad_physical_"+G4UIcommand::ConvertToString(CAD_icopy);
     new G4PVPlacement(transform, cad_logical, name, worldLogical, false, 1000, checkOverlaps);

}

void DetectorConstruction::ConstructSDandField()
{
    G4String detectorName = "/mydet/detector";
    DetectorSD * detectorSD = new DetectorSD(detectorName);
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    SetSensitiveDetector(scinLog,detectorSD);
}
