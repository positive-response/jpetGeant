#ifndef UserParameters_h
#define UserParameters_h 1
#include "globals.hh"

/**
 * \class UserParameters
 * \brief allows user to easily manipulate with simulation parameters.
 * \todo read parameters from input file
 */



class UserParameters
{
    public:
      static UserParameters* getInstance(); ///< only single instance can exist
      void SetRun(G4int x, G4int y){fRunNumber=x; fRunNumberMinor=y;};
      void SetNumberOfEvents(G4int x){fNumberOfEvents=x;};

      G4int GetRunNumber(){return fRunNumber;};
      G4int GetRunNumberMinor(){return fRunNumberMinor;};
      G4int GetNumberOfEvents(){return fNumberOfEvents;};

      void LoadFrame(G4bool tf){fLoadFrame=tf;};
      void LoadWrapping(G4bool tf){fLoadWrapping=tf;};

      G4bool IfLoadFrame(){return fLoadFrame;};
      G4bool IfLoadWrapping(){return fLoadWrapping;};

      G4bool CheckConsistency();
    private:
       static UserParameters* fInstance;
       UserParameters();
       G4int fRunNumber; ///< corresponds to JPET measurements; run 0 = user setup 
       G4int fRunNumberMinor; ///< used when in single run a different measurements were preformed 
       G4int fNumberOfEvents;
       G4bool fLoadFrame; ///< turn off for faster simulations
       G4bool fLoadWrapping;


};

#endif
