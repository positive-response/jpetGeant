#include "JPetGeantEventInformation.h"

ClassImp(JPetGeantEventInformation)


JPetGeantEventInformation::JPetGeantEventInformation()
    :fVtxPosition(0,0,0), fVtxPromptPosition(0,0,0), fGenGammaNum(fMaxGammaNumberIndex), fnRun(0), fLifetime(0), fPromptLifetime(0),
    fMomentumGamma0(0,0,0), fMomentumGamma1(0,0,0), fMomentumGamma2(0,0,0),fMomentumGamma3(0,0,0)
{}


JPetGeantEventInformation::~JPetGeantEventInformation()
{}

void JPetGeantEventInformation::Clear()
{
    fGenGammaNum.ResetAllBits();  
    fnRun = 0;
    fLifetime = 0.;
    fPromptLifetime = 0.;
    fVtxPosition.SetXYZ(0,0,0);
    fVtxPromptPosition.SetXYZ(0,0,0);
    fMomentumGamma0.SetXYZ(0,0,0);
    fMomentumGamma1.SetXYZ(0,0,0);
    fMomentumGamma2.SetXYZ(0,0,0);
    fMomentumGamma3.SetXYZ(0,0,0);

}

