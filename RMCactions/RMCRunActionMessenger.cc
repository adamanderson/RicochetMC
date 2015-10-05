////////////////////////////////////////////////////////////////////////
//                                                                    //
// File:        RMCRunActionMessenger.hh                              //
// Description: run action messenger for RMC detector                 //
//                                                                    //
// Author: Adapated from Ricochet by Adam Anderson (MIT)              //
//         Adapted from existing CDMS mini code by D.H. Wright (SLAC) //
// Date: 29 February 2012                                             //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include "RMCactions/RMCRunActionMessenger.hh"
#include "RMCactions/RMCRunAction.hh"
#include "RMCactions/RMCEventSelector.hh"

#include "G4RunManager.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"


RMCRunActionMessenger::RMCRunActionMessenger(RMCRunAction* action)
 : runAction(action) { 
  // Create the run directory
  RMCRunDir = new G4UIdirectory("/RMC/");
  RMCRunDir->SetGuidance("RMC run action controls.");

  // run directory already exists
  // Set autoSeed command
  setAutoSeedCmd = new G4UIcmdWithABool("/run/autoSeed",this);
  setAutoSeedCmd->SetGuidance("Switch on/off time-based random seeds");
  setAutoSeedCmd->SetGuidance(" true: run seeds determined by system time");
  setAutoSeedCmd->SetGuidance("false: use command 'random/resetEngineFrom'");
  setAutoSeedCmd->SetGuidance("Default = false");
  setAutoSeedCmd->SetParameterName("autoSeed", false);
  setAutoSeedCmd->AvailableForStates(G4State_Idle);

  RunNumberCmd = new G4UIcmdWithAnInteger("/run/setRunID",this);
  RunNumberCmd->SetGuidance("Initialize run ID counter before beamOn");
  RunNumberCmd->SetParameterName("runID", false);
  RunNumberCmd->AvailableForStates(G4State_Idle);


  // Set OutputTreesCmd
  OutputTreesCmd = new G4UIcmdWithABool("/RMC/writeTrees",this);
  OutputTreesCmd->SetGuidance("Output results to root trees");
  OutputTreesCmd->SetParameterName("treesout",true,true);
  OutputTreesCmd->SetDefaultValue(true);
  OutputTreesCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Set file name
  setRunFileName = new G4UIcmdWithAString("/RMC/writeFilePrefix",this);
  setRunFileName->SetGuidance("Set the prefix-name of the output files.");
  setRunFileName->SetParameterName("prefix",true);
  setRunFileName->SetDefaultValue("G4MC");
  setRunFileName->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Assign event filter to restrict output
  EventFilterCmd = new G4UIcmdWithAString("/RMC/eventFilter",this);
  EventFilterCmd->SetGuidance("Use specified selector to filter event output.");
  EventFilterCmd->SetParameterName("name",false);
  EventFilterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}


RMCRunActionMessenger::~RMCRunActionMessenger() {
  delete setAutoSeedCmd;
  delete RunNumberCmd;
  delete OutputTreesCmd;
  delete setRunFileName;
  delete EventFilterCmd;
}


void RMCRunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue) { 
  if (command == OutputTreesCmd)
    runAction->SetOutputTrees(OutputTreesCmd->GetNewBoolValue(newValue) );

  if (command == setAutoSeedCmd)
    runAction->SetAutoSeed(setAutoSeedCmd->GetNewBoolValue(newValue) );

  if (command == setRunFileName) runAction->SetDataFileNamePrefix(newValue);

  if (command == EventFilterCmd)
    runAction->SetEventSelector(MakeEventSelector(newValue));

  if (command == RunNumberCmd)
    G4RunManager::GetRunManager()->SetRunIDCounter(RunNumberCmd->GetNewIntValue(newValue));
}


// Create event-filter corresponding to name string

RMCEventSelector* RMCRunActionMessenger::MakeEventSelector(const G4String& name) const {
  RMCEventSelector* filter = 0;

  // NOTE:  G4String::compareTo() like strcmp(): -1, 0, +1 lexical order


  return filter;
}
