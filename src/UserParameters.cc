#include "UserParameters.hh"



UserParameters * UserParameters::fInstance = 0;

UserParameters* UserParameters::getInstance()
{
    if(fInstance == 0)     
    {
        fInstance = new UserParameters();
    }

    return fInstance;
}

UserParameters::UserParameters()
{
    fRunNumber = 0;
    fLoadWrapping = true;
    fLoadFrame = false;
    fNumberOfEvents=10;
}

G4bool UserParameters::CheckConsistency()
{
    // now only run small annihilation chamber from Run3 is implemented
    if( fRunNumber ==0 )
    {
        return true;
    } else if(fRunNumber == 3) 
    {
        return true;
    } else {

        return false;
    }

}
