#include "DetectorConstruction.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4RunManager.hh"
//#include "G4GeometryTolerance.hh"
//#include "G4GDMLParser.hh"
//
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "DetectorConstructionMessenger.hh"

DetectorConstruction::DetectorConstruction()
:  G4VUserDetectorConstruction(), fRunNumber(3),  fLoadFrame(false), fLoadCADFrame(false), fLoadWrapping(true)
{

    InitializeMaterials();

    fMessenger = new DetectorConstructionMessenger(this);

}

DetectorConstruction::~DetectorConstruction()
{
     delete fMessenger;
}


void DetectorConstruction::UpdateGeometry()
{
      // clean-up previous geometry
      G4SolidStore::GetInstance()->Clean();
      G4LogicalVolumeStore::GetInstance()->Clean();
      G4PhysicalVolumeStore::GetInstance()->Clean();
      //define new one
      G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
      G4RunManager::GetRunManager()->GeometryHasBeenModified();
      // Please note that materials and sensitive detectors cannot be deleted. Thus the user has to set the pointers of already-existing materials / sensitive detectors to the relevant logical volumes. 
}


G4VPhysicalVolume* DetectorConstruction::Construct() 
{

    // world 
     worldSolid   = new G4Box("world", world_hx, world_hy, world_hz);
     worldLogical  = new G4LogicalVolume(worldSolid,air,"worldLogical");  
     worldPhysical = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,false,0,checkOverlaps);                
    // scintillators for standard setup; right now always loaded
     ConstructScintillators();

     if(fLoadFrame)
     { 
       ConstructFrame();
       // \todo show differences between geometry loaded with cad and width simple volumes
        //ConstructFrameCAD();
     }

     if (fRunNumber == 3) {
         ConstructTargetRun3();
     }



    return worldPhysical;
}



void DetectorConstruction::LoadGeometryForRun(G4int nr)
{
    fRunNumber = nr;

     if (fRunNumber == 0) {
        LoadFrame(false);
     } else if (fRunNumber == 3) {
        LoadFrame(true);  
     } else {
         G4Exception ("DetectorConstruction","DC02", FatalException, 
             " This run setup is not implemented ");    
     }

}



void DetectorConstruction::ConstructTargetRun3()
{
   G4RotationMatrix rot = G4RotationMatrix();

   G4double z[] = {-37*cm, -32.61*cm,-32.6*cm, -31.1*cm, -31*cm, 31*cm, 31.1*cm, 32.6*cm, 32.61*cm, 37*cm}; 
   G4double rInner[] = { 0*cm, 0*cm, 0*cm, 0*cm, 9.1*cm, 9.1*cm, 0*cm, 0*cm, 0*cm, 0*cm};
   G4double rOuter[] = { 3*cm, 3*cm, 12*cm, 12*cm, 9.5*cm, 9.5*cm, 12*cm, 12*cm, 3*cm, 3*cm}; 

   G4Polycone* bigChamber = new G4Polycone("bigChamber",0*degree,360*degree, 10 , z, rInner, rOuter);
        

   G4LogicalVolume * bigChamber_logical = new G4LogicalVolume(bigChamber, bigChamberMaterial, "bigChamber_logical");

    G4VisAttributes* DetVisAtt =  new G4VisAttributes(G4Colour(0.9,0.9,.9));
    DetVisAtt->SetForceWireframe(true);
    DetVisAtt->SetForceSolid(true);
    bigChamber_logical->SetVisAttributes(DetVisAtt);


     G4ThreeVector loc = G4ThreeVector(0.0,0.0,0.0);
     G4Transform3D transform(rot,loc);
     new G4PVPlacement(transform,             //rotation,position
                       bigChamber_logical,            //its logical volume
                       "bigChamberGeom",             //its name
                       worldLogical,      //its mother (logical) volume
                       true,                 //no boolean operation
                       0,                 //copy number
                       checkOverlaps);       // checking overlaps 



    G4Tubs* ringInner = new G4Tubs("ringInner",15*mm,20.8*mm,0.8*mm,0*degree,360*degree);


    G4Box* conn = new G4Box("conn",31*mm,7.*mm,0.8*mm);
    G4LogicalVolume* conn_logical = new G4LogicalVolume(conn,bigChamberMaterial,"conn_logical");
    conn_logical->SetVisAttributes(DetVisAtt);


    G4ThreeVector loc2;
    G4Transform3D transform2;

    loc2 = G4ThreeVector(49.8*mm,0.0,0.0);
    transform2 = G4Transform3D(rot,loc2);
    G4UnionSolid*  unionSolid =  new G4UnionSolid("c1", ringInner,conn,transform2);

    loc2 = G4ThreeVector(-49.8*mm,0.0,0.0);
    transform2 = G4Transform3D(rot,loc2);
    unionSolid =  new G4UnionSolid("c2", unionSolid,conn,transform2);

    loc2 = G4ThreeVector(0.0,49.8*mm,0.0);
    transform2 = G4Transform3D(rot.rotateZ(90*degree),loc2);
    unionSolid =  new G4UnionSolid("c3", unionSolid,conn,transform2);

    loc2 = G4ThreeVector(0.0,-49.8*mm,0.0);
    transform2 = G4Transform3D(rot,loc2);
    unionSolid =  new G4UnionSolid("c4", unionSolid,conn,transform2);

    //G4Tubs* ringOuter = new G4Tubs("ringOuter",80*mm,95*mm,0.8*mm,0*degree,360*degree);
    G4Tubs* ringOuter = new G4Tubs("ringOuter",80*mm,90*mm,0.8*mm,0*degree,360*degree);
    unionSolid =  new G4UnionSolid("c5", unionSolid,ringOuter);


    G4LogicalVolume* unionSolid_logical = new G4LogicalVolume(unionSolid,bigChamberMaterial,"union_logical");
    unionSolid_logical->SetVisAttributes(DetVisAtt);

    new G4PVPlacement(transform,             //rotation,position
                       unionSolid_logical,            //its logical volume
                       "bigChamberInnerStructure",             //its name
                       worldLogical,      //its mother (logical) volume
                       true,                 //no boolean operation
                       0,                 //copy number
                       checkOverlaps);       // checking overlaps 


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

    G4Box* scinBoxFree = new G4Box("scinBoxFree", scinDim_x/2.0+wrappingShift ,scinDim_y/2.0+wrappingShift ,
            scinDim_z/2.0 );
    G4Box* wrappingBox = new G4Box("wrappingBox", scinDim_x/2.0+wrappingThickness,
            scinDim_y/2.0+wrappingThickness , scinDim_z/2.0-1*cm );
    G4LogicalVolume* wrappingLog; 

    G4VisAttributes* BoxVisAttWrapping =  new G4VisAttributes(G4Colour(0.4,0.4,.4));
    BoxVisAttWrapping->SetForceWireframe(true);
    BoxVisAttWrapping->SetForceSolid(true);



    G4int icopy = 1;

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

            G4String name = "scin_"+G4UIcommand::ConvertToString(icopy);

            new G4PVPlacement(transform,             //rotation,position
                              scinLog,            //its logical volume
                              name,             //its name
                              worldLogical,      //its mother (logical) volume
                              true,                 //no boolean operation
                              icopy,                 //copy number
                              checkOverlaps);       // checking overlaps 


             if(fLoadWrapping) 
             {
             // wrapping 
             
             G4VSolid* unionSolid =  new G4SubtractionSolid("wrapping", wrappingBox, scinBoxFree);
             wrappingLog = new G4LogicalVolume(unionSolid, kapton , "wrappingLogical");
             wrappingLog->SetVisAttributes(BoxVisAttWrapping);

             G4String nameWrapping = "wrapping_"+G4UIcommand::ConvertToString(icopy);

             new G4PVPlacement(transform,             //rotation,position
                              wrappingLog,            //its logical volume
                              nameWrapping,             //its name
                              worldLogical,      //its mother (logical) volume
                              true,                 //no boolean operation
                              icopy,                 //copy number
                              checkOverlaps);       // checking overlaps 
              }

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
    nistManager->FindOrBuildMaterial("G4_KAPTON");
    nistManager->FindOrBuildMaterial("G4_Galactic");

    //air = G4Material::GetMaterial("G4_AIR"); 
    //scinMaterial      =G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    //detectorMaterial  =G4Material::GetMaterial("G4_Al");
    //kapton  =G4Material::GetMaterial("G4_KAPTON");

    air =  new MaterialExtension("air",G4Material::GetMaterial("G4_AIR")); 
    vacuum =  new MaterialExtension("vacuum", G4Material::GetMaterial("G4_Galactic")); 
    scinMaterial      = new MaterialExtension("scinMaterial", G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE"));
    detectorMaterial  = new MaterialExtension("detectorMaterial",G4Material::GetMaterial("G4_Al"));
    kapton =  new MaterialExtension("kapton", G4Material::GetMaterial("G4_KAPTON")); 


    bigChamberMaterial = new MaterialExtension("bigChamber", G4Material::GetMaterial("G4_Al"));
    bigChamberMaterial->AllowsAnnihilations(true); 
}

void DetectorConstruction::ConstructFrame()
{

    G4VSolid* unionSolid; 
    G4Box* frameBox = new G4Box("frameBox", 600*mm , 600*mm , 7.5*mm );
    G4Tubs* tubsBox = new G4Tubs("tubsBox", 0*mm, 400*mm, 60*mm, 0*degree, 360*degree);

    unionSolid =  new G4SubtractionSolid("frameTub", frameBox, tubsBox );

    // remove scintillators volumes; ugly but working  
    G4int icopy = 1;

    G4Box* scinBox = new G4Box("scinBox", 10.5*mm, 4.5*mm, scinDim_z );

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

            G4String name = "unionScin_"+G4UIcommand::ConvertToString(icopy);
            unionSolid =  new G4SubtractionSolid(name, unionSolid, scinBox, transform);

            icopy++;

        }
    }

    // remove scintillators from two additional layers  
    for(int j=0;j<extraLayers;j++){
        for(int i=0;i<nSegmentsExtraLayers[j];i++){
            G4double phi = i*2*M_PI/nSegmentsExtraLayers[j];
            G4double fi = M_PI/nSegmentsExtraLayers[j];

            if( j == 0 ){
                fi =0.;
            }
            G4RotationMatrix rot = G4RotationMatrix();
            rot.rotateZ(phi+fi);

            G4ThreeVector loc = G4ThreeVector(radiusExtraLayers[j]*(cos(phi+fi)),radiusExtraLayers[j]*(sin(phi+fi)),0.0);
            G4Transform3D transform(rot,loc);

            G4String name = "unionScin_"+G4UIcommand::ConvertToString(icopy);

            unionSolid =  new G4SubtractionSolid(name, unionSolid, scinBox, transform);

            icopy++;

        }
    }


    G4LogicalVolume * cad_logical = new G4LogicalVolume(unionSolid, detectorMaterial, "cad_logical");

    G4VisAttributes* DetVisAtt =  new G4VisAttributes(G4Colour(0.9,0.9,.9));
    DetVisAtt->SetForceWireframe(true);
    DetVisAtt->SetForceSolid(true);
    cad_logical->SetVisAttributes(DetVisAtt);

     G4RotationMatrix rot = G4RotationMatrix();
     //rot.rotateX(0.1*deg);
     G4ThreeVector loc = G4ThreeVector(0.0,0.0,238.5*mm);
     G4Transform3D transform(rot,loc);

     new G4PVPlacement(transform,             //rotation,position
                       cad_logical,            //its logical volume
                       "Frame1",             //its name
                       worldLogical,      //its mother (logical) volume
                       true,                 //no boolean operation
                       0,                 //copy number
                       checkOverlaps);       // checking overlaps 


     G4ThreeVector loc2 = G4ThreeVector(0.0,0.0,-238.5*mm);
     G4Transform3D transform2(rot,loc2);

     new G4PVPlacement(transform2,             //rotation,position
                       cad_logical,            //its logical volume
                       "Frame2",             //its name
                       worldLogical,      //its mother (logical) volume
                       true,                 //no boolean operation
                       0,                 //copy number
                       checkOverlaps);       // checking overlaps 


    G4Box* connectorBox = new G4Box("connectorBox", 30*mm , 30*mm , 231*mm-0.1*mm );
    G4LogicalVolume * connectorBox_logical = new G4LogicalVolume(connectorBox, detectorMaterial, "connectorBox_logical");
    connectorBox_logical->SetVisAttributes(DetVisAtt);

    G4ThreeVector loc3 = G4ThreeVector(570.0*mm,570.0*mm,0.*mm);
    G4Transform3D transform3(rot,loc3);
    new G4PVPlacement(transform3,             //rotation,position
                       connectorBox_logical,            //its logical volume
                       "connector1",             //its name
                       worldLogical,      //its mother (logical) volume
                       true,                 //no boolean operation
                       0,                 //copy number
                       checkOverlaps);       // checking overlaps 

    G4ThreeVector loc4 = G4ThreeVector(-570.0*mm,570.0*mm,0.*mm);
    G4Transform3D transform4(rot,loc4);
    new G4PVPlacement(transform4,             //rotation,position
                       connectorBox_logical,            //its logical volume
                       "connector2",             //its name
                       worldLogical,      //its mother (logical) volume
                       true,                 //no boolean operation
                       0,                 //copy number
                       checkOverlaps);       // checking overlaps 

    G4ThreeVector loc5 = G4ThreeVector(570.0*mm,-570.0*mm,0.*mm);
    G4Transform3D transform5(rot,loc5);
    new G4PVPlacement(transform5,             //rotation,position
                       connectorBox_logical,            //its logical volume
                       "connector3",             //its name
                       worldLogical,      //its mother (logical) volume
                       true,                 //no boolean operation
                       0,                 //copy number
                       checkOverlaps);       // checking overlaps 

    G4ThreeVector loc6 = G4ThreeVector(-570.0*mm,-570.0*mm,0.*mm);
    G4Transform3D transform6(rot,loc6);
    new G4PVPlacement(transform6,             //rotation,position
                       connectorBox_logical,            //its logical volume
                       "connector4",             //its name
                       worldLogical,      //its mother (logical) volume
                       true,                 //no boolean operation
                       0,                 //copy number
                       checkOverlaps);       // checking overlaps 

}


void DetectorConstruction::ConstructFrameCAD()
{

     // in stl file the jscintillaotr slot were made bigger !!!
     //             Length      width
     //    true      2.1 cm      0.9 cm
     //    used in stl       2.6 cm      1.7 cm

     CADMesh * mesh1 = new CADMesh((char*)"../stl_sushil/Frame_JPET.stl" );
     mesh1->SetScale(mm);
     G4VSolid* cad_solid1 = mesh1->TessellatedMesh();

     G4LogicalVolume * cad_logical = new G4LogicalVolume(cad_solid1, detectorMaterial, "cad_logical");
 
     G4VisAttributes* DetVisAtt =  new G4VisAttributes(G4Colour(0.9,0.9,.9));
     DetVisAtt->SetForceWireframe(true);
     DetVisAtt->SetForceSolid(true);
     cad_logical->SetVisAttributes(DetVisAtt);

     G4RotationMatrix rot = G4RotationMatrix();
     rot.rotateY(90*deg);
     G4ThreeVector loc = G4ThreeVector(0*cm, 306.5*cm ,-23*cm);
     G4Transform3D transform(rot,loc);

     new G4PVPlacement(transform,             //rotation,position
                       cad_logical,            //its logical volume
                       "cadGeom",             //its name
                       worldLogical,      //its mother (logical) volume
                       true,                 //no boolean operation
                       0,                 //copy number
                       checkOverlaps);       // checking overlaps 

}

void DetectorConstruction::LoadCAD( const char* fileName)
{
     G4RotationMatrix rot = G4RotationMatrix();
     //G4ThreeVector loc = G4ThreeVector(0.,0.,0.0);
     rot.rotateY(90*deg);
     //G4ThreeVector loc = G4ThreeVector(0.018, 191.4003*cm, -23.05*cm);
     //G4ThreeVector loc = G4ThreeVector(0.0, 191.4*cm, -23.05*cm);
     G4ThreeVector loc = G4ThreeVector(0,0,23*cm);

     G4Transform3D transform(rot,loc);


     //CADMesh * mesh = new CADMesh((char*) "stl_geometry/frame_side_A.stl");
     CADMesh * mesh = new CADMesh((char*) fileName);
     //mesh->SetScale(m);
     mesh->SetScale(mm);
     G4VSolid* cad_solid = mesh->TessellatedMesh();
     G4LogicalVolume * cad_logical = new G4LogicalVolume(cad_solid, detectorMaterial, "cad_logical", 0, 0, 0);
     G4VisAttributes* DetVisAtt =  new G4VisAttributes(G4Colour(0.9,0.9,.9));
     DetVisAtt->SetForceWireframe(true);
     DetVisAtt->SetForceSolid(true);
     cad_logical->SetVisAttributes(DetVisAtt);
     //G4VPhysicalVolume * cad_physical = new G4PVPlacement(0, G4ThreeVector(), cad_logical, "cad_physical", worldLogical, false, checkOverlaps);
     
     G4String name = "cad_physical_"; //+G4UIcommand::ConvertToString(CAD_icopy);
     new G4PVPlacement(transform, cad_logical, name, worldLogical, false, 0, checkOverlaps);

}

void DetectorConstruction::ConstructSDandField()
{
    G4String detectorName = "/mydet/detector";
    DetectorSD * detectorSD = new DetectorSD(detectorName);
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    SetSensitiveDetector(scinLog,detectorSD);
}
