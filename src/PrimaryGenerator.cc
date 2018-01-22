#include "PrimaryGenerator.hh"
#include "G4SystemOfUnits.hh"

#include "G4HadPhaseSpaceGenbod.hh"
#include "globals.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh" 


#include "G4TransportationManager.hh" 
#include "G4Navigator.hh"
#include "Randomize.hh"

#include "G4PhysicalConstants.hh"
#include "G4Circle.hh"



PrimaryGenerator::PrimaryGenerator()
: G4VPrimaryGenerator()
{}

PrimaryGenerator::~PrimaryGenerator()
{}

void PrimaryGenerator::GeneratePrimaryVertex(G4Event* event)
{
    // create vertex of 2g/ 3g and if needed add de-excitation gamma quanta to this vertex
    G4double time = 0*s;


    //G4ThreeVector* boost = new G4ThreeVector(0,0,0);
    //HepBoost* boost = new HepBost(Hep3Vector direction(1,0,0),0.1);

    //G4ThreeVector vtxPosition;
    //int a;
    //std::tie(vtxPosition, a) = GetVerticesDistribution();

    G4PrimaryVertex* vertex = new G4PrimaryVertex(VertexUniformInCylinder(25*cm,20*cm), time);
    //G4PrimaryVertex* vertex = new G4PrimaryVertex(VertexUniformInCylinder(1*mm,1*mm), time);
    const G4ThreeVector bo(0.1,0,0);

//    GeneratePromptGammaSodium(vertex);
//    GenerateTwoGammaVertex(vertex);
//    GeneratePromptGammaSodium(vertex);

    GenerateThreeGammaVertex(vertex);

    event->AddPrimaryVertex(vertex);
}

G4ThreeVector PrimaryGenerator::VertexUniformInCylinder(G4double rSquare, G4double zmax)
{
  //vertex A uniform on a cylinder
  //  
  //const G4double rSquare = 144*cm;
  //const G4double zmax = 34*cm;
  //
  G4double r = std::sqrt(rSquare*G4UniformRand());

  G4double alpha = twopi*G4UniformRand();     //alpha uniform in (0, 2*pi)
  G4double ux = std::cos(alpha);
  G4double uy = std::sin(alpha);
  G4double z = zmax*(2*G4UniformRand() - 1);  //z uniform in (-zmax, +zmax)
  G4ThreeVector positionA(r*ux,r*uy,z);
  
  return positionA;
}



std::tuple<G4ThreeVector,int> PrimaryGenerator::GetVerticesDistribution()
{

    G4bool lookForVtx = false;
    G4ThreeVector myPoint(0,0,0);

    // annihilation will occure only in materials where it was allowed @see MaterialExtension
    // annihilation rate 2g/3g also depends on the material type
    // now assumed equal distribution in the target - this may be modified in the future
    while (!lookForVtx)
    {
        G4double x_tmp = 10.*(2*G4UniformRand() - 1)*cm;
        G4double y_tmp = 10.*(2*G4UniformRand() - 1)*cm;
        G4double z_tmp = 30.*(2*G4UniformRand() - 1)*cm;

        myPoint.setX(x_tmp);
        myPoint.setY(y_tmp);
        myPoint.setZ(z_tmp);

        G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
        MaterialExtension* mat = (MaterialExtension*)theNavigator->LocateGlobalPointAndSetup(myPoint)->GetLogicalVolume()->GetMaterial()  ; 
        lookForVtx = mat->IsTarget();

    };
 
    return std::make_tuple(myPoint,4);
}



void PrimaryGenerator::GenerateTwoGammaVertex(G4PrimaryVertex* vertex )
{
     G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
     G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");

    
     G4double mass = 1022*keV;
     std::vector<G4double> mass_secondaries = {0., 0.};
        
     std::vector<G4LorentzVector> out;
    
     G4HadPhaseSpaceGenbod* phS = new G4HadPhaseSpaceGenbod();
     phS->Generate(mass,mass_secondaries,out);

     // boost gamma quanta
     for (int i=0; i<2; i++)
     {
       // out[i].boost(BoostAxis,0.1);

        G4PrimaryParticle* particle1 = new G4PrimaryParticle(particleDefinition,
                out[i].px(),out[i].py(),out[i].pz(),out[i].e());

        vertex->SetPrimary(particle1);
     }

}

void PrimaryGenerator::GenerateThreeGammaVertex(G4PrimaryVertex* vertex )
{
     G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
     G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");

    
     Double_t mass_secondaries[3] = {0., 0., 0.};
    
     TGenPhaseSpace event;
     TLorentzVector vec_pozytonium(0.0,0.0,0.0,1022);
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
     for (int i=0; i<3; i++)
     {
        TLorentzVector * out = event.GetDecay(i);

        G4PrimaryParticle* particle1 = new G4PrimaryParticle(particleDefinition,
                out->Px()*keV,out->Py()*keV,out->Pz()*keV,out->E()*keV);
      // output in MeV

        vertex->SetPrimary(particle1);
     }


}


void PrimaryGenerator::GeneratePromptGammaSodium(G4PrimaryVertex* vertex ) 
{
     G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
     G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");

     const G4double ene = 1.2770*MeV;

     double theta = 2 * M_PI * G4UniformRand();
     double phi = acos(1 - 2 * G4UniformRand());
     double px = ene * sin(phi) * cos(theta);
     double py = ene * sin(phi) * sin(theta);
     double pz = ene * cos(phi);

     G4PrimaryParticle* particle1 = new G4PrimaryParticle(particleDefinition,
                px, py, pz,ene);

      vertex->SetPrimary(particle1);
      //printf(" %f \n", sqrt(pow(px,2)+pow(py,2)+pow(pz,2)));

}


Double_t PrimaryGenerator::calculate_mQED(Double_t mass_e, Double_t w1, Double_t w2, Double_t w3){ 
   Double_t mQED = pow((mass_e-w1)/(w2*w3),2) + pow((mass_e-w2)/(w1*w3),2) + pow((mass_e-w3)/(w1*w2),2); 
  return mQED;
 }
