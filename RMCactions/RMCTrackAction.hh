#ifndef RMCTrackAction_h
#define RMCTrackAction_h

#include "globals.hh"
#include "G4UserTrackingAction.hh"
#include "G4TrackStatus.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"

class RMCTrackAction : public G4UserTrackingAction {

  public:
    RMCTrackAction();
    virtual ~RMCTrackAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:
  G4bool StoreTrajectoriesOfSecondariesFlag;

  public:
  inline void SetStoreTrajectoriesOfSecondariesFlag(G4bool val) { StoreTrajectoriesOfSecondariesFlag = val; };

};

#endif

