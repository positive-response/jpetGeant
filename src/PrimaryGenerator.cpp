#include "PrimaryGenerator.h"
#include "G4SystemOfUnits.hh"
#include "G4HadPhaseSpaceGenbod.hh"
#include "globals.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh" 
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"
#include "G4RandomDirection.hh"
#include "VtxInformation.h"
#include "PrimaryParticleInformation.h"
#include "DetectorConstruction.h"
#include "DetectorConstants.h"
#include "MaterialParameters.h"



PrimaryGenerator::PrimaryGenerator()
    : G4VPrimaryGenerator()
{}

PrimaryGenerator::~PrimaryGenerator()
{}

G4PrimaryVertex* PrimaryGenerator::GenerateThreeGammaVertex(const G4ThreeVector vtxPosition, const G4double T0, const G4double lifetime3g )
{
  // set vtx parameters
    G4PrimaryVertex* vertex = new G4PrimaryVertex() ;
    VtxInformation* info = new VtxInformation();

    G4double lifetime = G4RandExponential::shoot(lifetime3g);
    info->SetThreeGammaGen(true);
    info->SetLifetime((T0+lifetime));
    info->SetVtxPosition(vtxPosition.x(),vtxPosition.y(),vtxPosition.z());
    vertex->SetUserInformation(info);
    vertex->SetT0(T0+lifetime);
    vertex->SetPosition(vtxPosition.x(),vtxPosition.y(),vtxPosition.z());


  // momenta generation
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");
    Double_t mass_secondaries[3] = {0., 0., 0.};

    TGenPhaseSpace event;
    TLorentzVector vec_pozytonium(0.0,0.0,0.0,1022*keV);
    Bool_t test =  event.SetDecay(vec_pozytonium, 3, mass_secondaries);
    if( !test){
        std::cout   << "error: generate_gamma : createThreeEvts:" << test << std::endl;  
    }

    Double_t weight;
    Double_t weight_max= event.GetWtMax()*pow(10,5);
    Double_t rwt;  
    Double_t M_max = 7.65928*pow(10,-6);    
    do{
        weight = event.Generate();        
        weight = weight*calculate_mQED(511,event.GetDecay(0)->E(),event.GetDecay(1)->E(),event.GetDecay(2)->E());
        rwt = M_max*weight_max*(G4UniformRand());
    }while( rwt > weight );    

    event.Generate();   
    G4PrimaryParticle* particle[3];
    for (int i=0; i<3; i++)
    {
        TLorentzVector * out = event.GetDecay(i);

        particle[i] = new G4PrimaryParticle(particleDefinition,
                out->Px(),out->Py(),out->Pz(),out->E());

        PrimaryParticleInformation* infoParticle = new PrimaryParticleInformation();
        infoParticle->SetGammaMultiplicity(3);
        infoParticle->SetIndex(i+1);
        particle[i]->SetUserInformation(infoParticle);
        vertex->SetPrimary(particle[i]);
    }

  return vertex;
}


G4PrimaryVertex* PrimaryGenerator::GenerateTwoGammaVertex(const G4ThreeVector vtxPosition, const G4double T0,  const G4double lifetime2g)
{
  // set vtx parameters
    G4PrimaryVertex* vertex = new G4PrimaryVertex() ;
    VtxInformation* info = new VtxInformation();

    G4double lifetime = G4RandExponential::shoot(lifetime2g);
    info->SetTwoGammaGen(true);
    info->SetLifetime((T0+lifetime));
    info->SetVtxPosition(vtxPosition.x(),vtxPosition.y(),vtxPosition.z());
    vertex->SetUserInformation(info);
    vertex->SetT0(T0+lifetime);
    vertex->SetPosition(vtxPosition.x(),vtxPosition.y(),vtxPosition.z());


  // momenta generation
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");


    Double_t mass_secondaries[2] = {0., 0.};

    TGenPhaseSpace event;
    TLorentzVector vec_pozytonium(0.0,0.0,0.0,1022*keV);
    Bool_t test =  event.SetDecay(vec_pozytonium, 2, mass_secondaries);
    if( !test){
        std::cout   << "error: generate_gamma : createTwoEvts:" << test << std::endl;  
    }


    event.Generate();   
    G4PrimaryParticle* particle[2];

    for (int i=0; i<2; i++)
    {
        TLorentzVector * out = event.GetDecay(i);

        particle[i] = new G4PrimaryParticle(particleDefinition,
                out->Px(),out->Py(),out->Pz(),out->E());


        PrimaryParticleInformation* infoParticle = new PrimaryParticleInformation();
        infoParticle->SetGammaMultiplicity(2);
        infoParticle->SetIndex(i+1);
        particle[i]->SetUserInformation(infoParticle);
        vertex->SetPrimary(particle[i]);
    }

    // add polarization
      G4ThreeVector gammaMom = particle[0]->GetMomentum();
      G4ThreeVector polarization1 = gammaMom.orthogonal().unit();
      polarization1 = polarization1.rotate( twopi * G4UniformRand() , gammaMom);
      particle[1]->SetPolarization( polarization1 );

      G4ThreeVector polarization2 = polarization1;

      polarization2 = polarization2.rotate( pi/2.0, gammaMom);
      particle[0]->SetPolarization( polarization2 );

      return vertex;
}


G4PrimaryVertex* PrimaryGenerator::GeneratePromptGammaVertex(const G4ThreeVector vtxPosition, const G4double T0,  const G4double lifetimePrompt, const G4double energy)
{
  // set vtx parameters
    G4PrimaryVertex* vertex = new G4PrimaryVertex() ;
    VtxInformation* info = new VtxInformation();

    G4double lifetime = G4RandExponential::shoot(lifetimePrompt);
    info->SetPromptGammaGen(true);
    info->SetLifetime((T0+lifetime));
    info->SetVtxPosition(vtxPosition.x(),vtxPosition.y(),vtxPosition.z());
    vertex->SetUserInformation(info);
    vertex->SetT0(T0+lifetime);
    vertex->SetPosition(vtxPosition.x(),vtxPosition.y(),vtxPosition.z());


  // momenta generation
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");

    G4ThreeVector momentum = GetRandomPointOnSphere(energy);

    G4PrimaryParticle* particle1 = new G4PrimaryParticle(particleDefinition,
          momentum.x(), momentum.y(), momentum.z(), energy);
    PrimaryParticleInformation* infoParticle = new PrimaryParticleInformation();
    infoParticle->SetGammaMultiplicity(1);
    infoParticle->SetIndex(1);
    particle1->SetUserInformation(infoParticle);

    vertex->SetPrimary(particle1);

    return vertex;
}

void PrimaryGenerator::GenerateEvtSmallChamber(G4Event* event, const G4double effectivePositronRadius )
{
    G4ThreeVector chamberCenter=DetectorConstants::GetChamberCenter();

    G4ThreeVector vtxPosition; // 2g/3g
    MaterialExtension* material;
    std::tie(vtxPosition, material) = GetVerticesDistributionInFilledSphere(chamberCenter,effectivePositronRadius);

    std::vector<G4double> evtFractions = material->GetEventsFraction();
    G4double random = G4UniformRand();
    G4double lifetime3g = material->GetoPsLifetime(); 

    // for sodium: emitted positrons have up to 100~keV velocity
    // therefore their speed v=sqrt(2*e/m) = 0.6c
    G4double T0 = (vtxPosition-chamberCenter).mag()/(0.6*c_light);

    if( evtFractions[0] > random) {
      // 2g
      event->AddPrimaryVertex(GenerateTwoGammaVertex(vtxPosition,T0,MaterialParameters::tauBulk));
    } else if ( (evtFractions[0] + evtFractions[1]) > random ) {
      // 2g pick off
      event->AddPrimaryVertex(GenerateTwoGammaVertex(vtxPosition,T0,lifetime3g));
    } else if ( (evtFractions[0] + evtFractions[1] + evtFractions[2]) > random ) {
      // 3g direct
      event->AddPrimaryVertex(GenerateThreeGammaVertex(vtxPosition,T0,MaterialParameters::tauBulk));
    } else {
      // 3g oPs
      event->AddPrimaryVertex(GenerateThreeGammaVertex(vtxPosition,T0,lifetime3g));
    }


    // Add prompt gamma from sodium 
    G4ThreeVector promptVtxPosition =  VertexUniformInCylinder(0.2*cm,0.2*cm)+chamberCenter;

    event->AddPrimaryVertex(
    GeneratePromptGammaVertex(promptVtxPosition,0.0f,MaterialParameters::sodiumGammaTau,MaterialParameters::sodiumGammaEnergy)
    ); 

}

std::tuple<G4ThreeVector,MaterialExtension*> PrimaryGenerator::GetVerticesDistributionInFilledSphere(const G4ThreeVector center, G4double radius)
{

    G4bool lookForVtx = false;
    G4ThreeVector myPoint(0*cm,0*cm,0*cm);
    MaterialExtension* mat;

    // annihilation will occure only in materials where it was allowed @see MaterialExtension
    // annihilation rate 2g/3g also depends on the material type
    while (!lookForVtx)
    {
      myPoint=GetRandomPointInFilledSphere(radius)+center;
      theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
      mat = (MaterialExtension*)theNavigator->LocateGlobalPointAndSetup(myPoint)->GetLogicalVolume()->GetMaterial()  ; 
      lookForVtx = mat->IsTarget();

    };

    return std::make_tuple(myPoint,mat);
}

std::tuple<G4ThreeVector,MaterialExtension*> PrimaryGenerator::GetVerticesDistributionAlongStepVector(const G4ThreeVector center, const G4ThreeVector direction)
{
    G4bool lookForVtx = false;
    G4ThreeVector myPoint;
    G4ThreeVector myNextPoint = center;
    MaterialExtension* mat;

    while (!lookForVtx)
    {
      myPoint = myNextPoint;
      theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
      mat = (MaterialExtension*)theNavigator->LocateGlobalPointAndSetup(myPoint)->GetLogicalVolume()->GetMaterial()  ; 
      lookForVtx = mat->IsTarget();

      myNextPoint=myPoint+direction;
    };

 
    return std::make_tuple(myPoint,mat);
}
void PrimaryGenerator::GenerateEvtLargeChamber(G4Event* event)
{

    G4ThreeVector chamberCenter=DetectorConstants::GetChamberCenter();

    G4ThreeVector vtxPosition; // 2g/3g
    MaterialExtension* material;
    std::tie(vtxPosition, material) = GetVerticesDistributionAlongStepVector(chamberCenter,GetRandomPointInFilledSphere(1.0f*mm));


    std::vector<G4double> evtFractions = material->GetEventsFraction();
    G4double random = G4UniformRand();
    G4double lifetime3g = material->GetoPsLifetime(); 

    // for sodium: emitted positrons have up to 100~keV velocity
    // therefore their speed v=sqrt(2*e/m) = 0.6c
    G4double T0 = (vtxPosition-chamberCenter).mag()/(0.6*c_light);

    if( evtFractions[0] > random) {
      // 2g
      event->AddPrimaryVertex(GenerateTwoGammaVertex(vtxPosition,T0,MaterialParameters::tauBulk));
    } else if ( (evtFractions[0] + evtFractions[1]) > random ) {
      // 2g pick off
      event->AddPrimaryVertex(GenerateTwoGammaVertex(vtxPosition,T0,lifetime3g));
    } else if ( (evtFractions[0] + evtFractions[1] + evtFractions[2]) > random ) {
      // 3g direct
      event->AddPrimaryVertex(GenerateThreeGammaVertex(vtxPosition,T0,MaterialParameters::tauBulk));
    } else {
      // 3g oPs
      event->AddPrimaryVertex(GenerateThreeGammaVertex(vtxPosition,T0,lifetime3g));
    }


    // Add prompt gamma from sodium 
    G4ThreeVector promptVtxPosition =  VertexUniformInCylinder(0.2*cm,0.2*cm)+chamberCenter;

    event->AddPrimaryVertex(
    GeneratePromptGammaVertex(promptVtxPosition,0.0f,MaterialParameters::sodiumGammaTau,MaterialParameters::sodiumGammaEnergy)
    ); 

}


void PrimaryGenerator::GenerateBeam(BeamParams* beamParams, G4Event* event)
{

    G4ThreeVector vtxCoor = beamParams->GetVtx();
    G4PrimaryVertex* vertex = new G4PrimaryVertex(vtxCoor,0);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");

    const G4double ene = beamParams->GetEne();
    G4ThreeVector momentum = beamParams->GetMomentum();


    G4double px = ene * momentum.x();
    G4double py = ene * momentum.y();
    G4double pz = ene * momentum.z();

    G4PrimaryParticle* particle1 = new G4PrimaryParticle(particleDefinition,
            px, py, pz,ene);

    PrimaryParticleInformation* infoParticle = new PrimaryParticleInformation();
    infoParticle->SetGammaMultiplicity(1);
    infoParticle->SetIndex(1);
    particle1->SetUserInformation(infoParticle);

    vertex->SetPrimary(particle1);
    VtxInformation* infoPrompt = new VtxInformation();
    vertex->SetUserInformation(infoPrompt);
    infoPrompt->SetPromptGammaGen(true);
    infoPrompt->SetVtxPosition(vtxCoor.x(),vtxCoor.y(),vtxCoor.z());


    event->AddPrimaryVertex(vertex);
}

void PrimaryGenerator::GenerateIsotope(SourceParams* sourceParams, G4Event* event)
{

    G4ThreeVector vtxPosition; 

    if (sourceParams->GetShape() == "cylinder")
    {
        vtxPosition =  VertexUniformInCylinder(sourceParams->GetShapeDim(0),sourceParams->GetShapeDim(1))+sourceParams->GetShapeCenterPosition();
    }


    if ( sourceParams->GetGammasNumber() == 1 )
    { 
      event->AddPrimaryVertex(
        GeneratePromptGammaVertex(vtxPosition,0.0f,MaterialParameters::sodiumGammaTau,MaterialParameters::sodiumGammaEnergy)
      ); 

    } else if ( sourceParams->GetGammasNumber() == 2 )   {
        //generate 2g
        event->AddPrimaryVertex(GenerateTwoGammaVertex(vtxPosition,0.0f,MaterialParameters::tauBulk));
    } else if ( sourceParams->GetGammasNumber() == 3 )   {
        //generate 3g
        event->AddPrimaryVertex(GenerateThreeGammaVertex(vtxPosition,0.0f,MaterialParameters::oPsTauVaccum));
    } else {
        G4Exception("PrimaryGenerator","PG01",FatalException,
                " program does not know how many gamma quanta need to be simulated ");
    }


}

void PrimaryGenerator::GenerateNema(G4int nemaPoint, G4Event* event)
{

    /** NEMA positions - coordinate system centered in the middle of the detector
     * for( z={0,-3/4L}){
     *  y = 1, 10, 20 cm
     *  x =0 
     *  }
     *
     *  20      3       6
     *  10      2       5
     *  1       1       4
     *  z ------0------3/4L ------
     */

    G4double x_creation = 0.0*cm; 
    G4double y_creation = 0.0*cm; 
    G4double z_creation = 0.0*cm;

        if(nemaPoint>3){
            z_creation = z_creation - DetectorConstants::scinDim[2]*3/8;
        }
    
        if(nemaPoint==1 || nemaPoint == 4){
            y_creation = y_creation + 1.0*cm;
        }
    
        if(nemaPoint==2 || nemaPoint == 5){
            y_creation = y_creation + 10.0*cm;
        }
        if(nemaPoint==3 || nemaPoint == 6){
            y_creation = y_creation + 20.0*cm;
        }



    G4ThreeVector vtxPosition
      =  VertexUniformInCylinder(0.1*mm,0.1*mm)+G4ThreeVector(x_creation,y_creation,z_creation);


    event->AddPrimaryVertex(GenerateTwoGammaVertex(vtxPosition,0.0f,MaterialParameters::tauBulk));
    event->AddPrimaryVertex(
        GeneratePromptGammaVertex(vtxPosition,0.0f,MaterialParameters::sodiumGammaTau,MaterialParameters::sodiumGammaEnergy)
      ); 

}


G4ThreeVector PrimaryGenerator::VertexUniformInCylinder(G4double rIn, G4double zmax)
{

    G4double r = std::sqrt(pow(rIn,2)*G4UniformRand());

    G4double alpha = twopi*G4UniformRand();     //alpha uniform in (0, 2*pi)
    G4double ux = std::cos(alpha);
    G4double uy = std::sin(alpha);
    G4double z = zmax*(2*G4UniformRand() - 1);  //z uniform in (-zmax, +zmax)
    G4ThreeVector positionA(r*ux,r*uy,z);

    return positionA;
}



std::tuple<G4ThreeVector,G4double,G4double> PrimaryGenerator::GetVerticesDistribution(G4double maxXhalf, G4double maxYhalf, G4double maxZhalf)
{

    G4bool lookForVtx = false;
    G4ThreeVector myPoint(0*cm,0*cm,0*cm);
    MaterialExtension* mat;

    // annihilation will occure only in materials where it was allowed @see MaterialExtension
    // annihilation rate 2g/3g also depends on the material type
    // now assumed equal distribution in the target - this may be modified in the future
    while (!lookForVtx)
    {
        G4double x_tmp = maxXhalf*(2*G4UniformRand() - 1)*cm;
        G4double y_tmp = maxYhalf*(2*G4UniformRand() - 1)*cm;
        G4double z_tmp = maxZhalf*(2*G4UniformRand() - 1)*cm;

        myPoint.setX(x_tmp);
        myPoint.setY(y_tmp);
        myPoint.setZ(z_tmp);

        theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
        mat = (MaterialExtension*)theNavigator->LocateGlobalPointAndSetup(myPoint)->GetLogicalVolume()->GetMaterial()  ; 
        lookForVtx = mat->IsTarget();

    };

    G4double ratio3g = mat->Get3gFraction();
    G4double  lifetime3g = mat->GetoPsLifetime();


    return std::make_tuple(myPoint,ratio3g,lifetime3g);
}

const G4ThreeVector PrimaryGenerator::GetRandomPointInFilledSphere(G4double radius)
{
    G4double theta = 2 * M_PI * G4UniformRand();
    G4double phi = acos(1 - 2 * G4UniformRand());
    G4double r = radius * cbrt(G4UniformRand());
    G4double x = r * sin(phi) * cos(theta);
    G4double y = r * sin(phi) * sin(theta);
    G4double z = r * cos(phi);

    return G4ThreeVector(x,y,z);

}

const G4ThreeVector PrimaryGenerator::GetRandomPointOnSphere(G4double radius)
{
    G4double theta = 2 * M_PI * G4UniformRand();
    G4double phi = acos(1 - 2 * G4UniformRand());
    G4double x = radius * sin(phi) * cos(theta);
    G4double y = radius * sin(phi) * sin(theta);
    G4double z = radius * cos(phi);

    return G4ThreeVector(x,y,z);
}

G4double PrimaryGenerator::calculate_mQED(Double_t mass_e, Double_t w1, Double_t w2, Double_t w3){ 
    return pow((mass_e-w1)/(w2*w3),2) + pow((mass_e-w2)/(w1*w3),2) + pow((mass_e-w3)/(w1*w2),2); 
}
