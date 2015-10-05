#include "RMCTrackAction.hh"

RMCTrackAction::RMCTrackAction()
{
  StoreTrajectoriesOfSecondariesFlag = true; 
}

RMCTrackAction::~RMCTrackAction()
{
}


void RMCTrackAction::PreUserTrackingAction(const G4Track* aTrack)
{

  if ( (StoreTrajectoriesOfSecondariesFlag) || (aTrack->GetParentID()==0) )
    { fpTrackingManager->SetStoreTrajectory(true); }
  else
    { fpTrackingManager->SetStoreTrajectory(false); }
  
}

void RMCTrackAction::PostUserTrackingAction(const G4Track* aTrack)
{

  G4String particleName = aTrack->GetDefinition()->GetParticleName();
  G4TrackStatus status = aTrack->GetTrackStatus();
 
  if (status == fKillTrackAndSecondaries) {
#if 0
    fpTrackingManager->SetStoreTrajectory(false);
#else // 0 or 1
    G4VTrajectory *fpTrajectory = fpTrackingManager->GimmeTrajectory();
    if (fpTrajectory) {
      delete fpTrajectory;
      fpTrackingManager->SetTrajectory( ((G4VTrajectory *)0) );
    }
    fpTrackingManager->SetStoreTrajectory(false);
    fpTrackingManager->EventAborted(); // sets fKillTrackAndSecondaries
#endif // 0 or 1
  }

#if 0
  G4cout << "TrackStatus about to be set to " << status << G4endl;
  fpTrackingManager->SetTrackStatus(status);
#endif // 0 or 1
}

