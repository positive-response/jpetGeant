#include "PhysicsList.hh"
#include "G4EmLivermorePolarizedPhysics.hh"


PhysicsList::PhysicsList() 
: G4VModularPhysicsList()
{
     SetVerboseLevel(1);
     RegisterPhysics(new G4EmLivermorePolarizedPhysics());
}

PhysicsList::~PhysicsList()
{
}

