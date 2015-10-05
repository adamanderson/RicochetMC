#include "RMCStepAction.hh"
#include "G4RunManager.hh"


RMCStepAction::RMCStepAction()
{

}

RMCStepAction::~RMCStepAction()
{
}


void RMCStepAction::UserSteppingAction(const G4Step* aStep)
{

 // Suspend an event if anything enters the veto region

  // check if it is alive
//   G4Track * theTrack = aStep->GetTrack();
//   if(theTrack->GetTrackStatus()!=fAlive) { return; }
// 
//   G4StepPoint * thePostPoint = aStep->GetPostStepPoint();
//   G4LogicalVolume * thePostLV = thePostPoint->GetPhysicalVolume()->GetLogicalVolume();
// 
//   if(thePostLV->GetName().find("Scint")!=std::string::npos)
//   {
//   std::cout<<"ABORT!!!"<<std::endl;
//   G4RunManager::GetRunManager()->AbortEvent();
//   }


}




