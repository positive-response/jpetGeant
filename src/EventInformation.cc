#include "EventInformation.hh"

EventInformation::EventInformation()
    :fVtxPosition(0), fTwoGammaGen(false), fThreeGammaGen(false), fPromptGammaGen(false), fnRun(0), fLifetime(0)
{}


EventInformation::~EventInformation()
{}


void EventInformation::SetVtxPosition(G4double x, G4double y, G4double z)
{ 
    fVtxPosition.setX(x);
    fVtxPosition.setY(y);
    fVtxPosition.setZ(z);
};


void EventInformation::Clear()
{
    fTwoGammaGen = false;
    fThreeGammaGen = false;
    fPromptGammaGen = false;
    fnRun = 0;
    fLifetime =0.;
    fVtxPosition.setX(0);
    fVtxPosition.setY(0);
    fVtxPosition.setZ(0);

}

void EventInformation::Print() const
{
    
}

