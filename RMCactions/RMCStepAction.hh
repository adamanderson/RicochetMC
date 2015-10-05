#ifndef RMCStepAction_h
#define RMCStepAction_h

#include "globals.hh"
#include "G4UserSteppingAction.hh"
#include "G4StepStatus.hh"
#include "G4Step.hh"

class RMCStepAction : public G4UserSteppingAction {

  public:
    RMCStepAction();
    virtual ~RMCStepAction();
    virtual void UserSteppingAction(const G4Step*);

};



#endif

