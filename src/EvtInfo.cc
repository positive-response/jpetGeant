#include "EvtInfo.hh"


EvtInfo::EvtInfo()
{
    fEvtId =-1;
    fX=-1;
    fY=-1;
    fZ=-1;
    fTime=-1;
}

void EvtInfo::FillEvtInfo(G4double id, G4double x, G4double y, G4double z, G4double t)
{
    fEvtId = id;
    fX= x;
    fY= y;
    fZ= z;
    fTime= t;

}


EvtInfo::~EvtInfo()
{}
