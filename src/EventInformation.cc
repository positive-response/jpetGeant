#include "EventInformation.hh"

EventInformation::EventInformation()
    : fTwoGammaGen(false), fThreeGammaGen(false), fPromptGammaGen(false), fnRun(0)
{}

void EventInformation::Clear()
{
    fTwoGammaGen = false;
    fThreeGammaGen = false;
    fPromptGammaGen = false;
    fnRun = 0;

}

