////////////////////////////////////////////////////////////////////////
//                                                                    //
// File:        RMCRunActionMessenger.hh                              //
// Description: run action messenger for RMC detector                 //
//                                                                    //
// Author: Adapated from Ricochet by Adam Anderson (MIT)              //
//         Adapted from existing RMC mini code by D.H. Wright (SLAC) //
// Date: 29 February 2012                                             //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#ifndef RMCRunActionMessenger_h
#define RMCRunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RMCEventSelector;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class RMCRunAction;


class RMCRunActionMessenger: public G4UImessenger {
public:
  RMCRunActionMessenger(RMCRunAction* action);
  ~RMCRunActionMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);

protected:
  RMCEventSelector* MakeEventSelector(const G4String& name) const;

private:
  RMCRunAction* runAction;
  G4UIdirectory* RMCRunDir;
  
  G4UIcmdWithAString* setRunFileName;
  G4UIcmdWithAnInteger* RunNumberCmd;
  G4UIcmdWithABool* OutputTreesCmd;
  G4UIcmdWithABool* setAutoSeedCmd;
  G4UIcmdWithAString* EventFilterCmd;
  G4UIcmdWithAnInteger* setVerboseCmd;
};

#endif	/* RMCRunActionMessenger_h */
