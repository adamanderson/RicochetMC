#ifndef RMCEventSelector_hh
#define RMCEventSelector_hh 1
////////////////////////////////////////////////////////////////////////
//                                                                    //
//  File:        RMCEventSelector.hh                                  //
//  Description: Accept/reject interface for use in RMCRunAction      //
//                                                                    //
//  Author:      Adam Anderson (MIT)                                  //
//               Adapted from: Michael Kelsey (SLAC)                  //
//  Date:        28 June 2011                                         //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include "globals.hh"

class G4Event;


class RMCEventSelector {
public:
  RMCEventSelector(const G4String& name, G4int verbose=0)
    : selectorName(name), verboseLevel(verbose) {}

  virtual ~RMCEventSelector() {}

  // Subclasses must implement selection function
  virtual G4bool accept(const G4Event* evt) const = 0;
  virtual G4bool reject(const G4Event* evt) const { return !accept(evt); }

  virtual void SetVerboseLevel(G4int verbose) { verboseLevel = verbose; }

  virtual const G4String& GetName() const { return selectorName; }

protected:
  G4String selectorName;
  G4int verboseLevel;
};

#endif	/* RMCEventSelector_hh */
