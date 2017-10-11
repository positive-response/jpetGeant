#ifndef SavedEvent_h
#define SavedEvent_h

#include "TObject.h"
#include "ScinBlock.hh"
#include "TrkBlock.hh"


class SavedEvent : public TObject
{
    private:
        TrkBlock*   fTrk;
        ScinBlock*  fScin;


    public:
        SavedEvent();
        ~SavedEvent();
        void Clean();

        TrkBlock* GetTrkBlock() const {return fTrk;}
        ScinBlock* GetScinBlock() const {return fScin;}

        ClassDef(SavedEvent,1)
};



#endif
