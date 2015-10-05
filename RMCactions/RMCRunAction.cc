////////////////////////////////////////////////////////////////////////
//                                                                    //
//  File:        RMCRunAction.cc                                      //
//  Description: class to specify user actions taken before and after //
//               the run                                              //
//                                                                    //
//  Author:      Adam Anderson (MIT)                                  //
//               (adapted from Dennis Wright (SLAC))                  //
//  Date:        8 January 2012                                       //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include "RMCactions/RMCRunAction.hh"
#include "RMCactions/RMCRunActionMessenger.hh"

#include "RMCactions/RMCEventSelector.hh"
#include "RMCgeometry/detectors/RMCZipHit.hh"
#include "RMCgeometry/detectors/RMCVetoHit.hh"

#include "G4HCofThisEvent.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4LogicalVolumeStore.hh"


RMCRunAction::RMCRunAction()
  : zipCollID(-1), vetoCollID(-1), filePrefix("G4MC"), verboseLevel(0),
    AutoSeed(false), OutputTrees(true),
    EventSelector(0)
{
  runActionMessenger = new RMCRunActionMessenger(this);  
}

RMCRunAction::~RMCRunAction()
{
  delete EventSelector; EventSelector=0;
  delete runActionMessenger; runActionMessenger=0;
}


// Pass verbosity level to I/O functions

void RMCRunAction::SetVerboseLevel(int verbose) {
  verboseLevel = verbose;
  rootOut.SetVerboseLevel(verbose);
}


// Replace event selection with new one (may be null pointer!)

void RMCRunAction::SetEventSelector(RMCEventSelector* val) {
  delete EventSelector;
  EventSelector = val;

  if (EventSelector) EventSelector->SetVerboseLevel(verboseLevel);
}


// Main processing

void RMCRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "RMCRunAction::BeginOfRunAction" << G4endl;

  if (!aRun) return;			// Sanity check

  int runN = aRun->GetRunID();
 
  if (verboseLevel) G4cout << "\n>>> Begin Run : " << runN << G4endl;

  int labCode = GetLaboratoryCode();

  if (OutputTrees) rootOut.SetupFile(filePrefix, runN, labCode);
  
  // Automatic time-based random seeds for each run
  if (AutoSeed) {
    if (verboseLevel) {
      G4cout << "\n*******************\n*** AUTOSEED ON ***"
	     << "\n*******************" << G4endl;
    }
    
    long seeds[2];
    time_t systime = time(NULL);
    seeds[0] = (long) systime;
    seeds[1] = (long) (systime*G4UniformRand() );
    CLHEP::HepRandom::setTheSeeds(seeds);
    CLHEP::HepRandom::showEngineStatus();
  }

  // Make sure hit collections are present, and get indices
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if (zipCollID) zipCollID = SDman->GetCollectionID("zipHitsCollection");
  if (vetoCollID) vetoCollID = SDman->GetCollectionID("vetoHitsCollection");
}


void RMCRunAction::transferEvent(const G4Event* evt)
{
  if (verboseLevel>1)
    G4cout << ">>> transferEvent #" << evt->GetEventID() << G4endl;

  // Apply event selection filter ("trigger") before any processing
  /*if (EventSelector && EventSelector->reject(evt)) {
    if (verboseLevel>1) 
      G4cout << " Rejected by selector " << EventSelector->GetName() << G4endl;

    return;
  }*/

 
  
  if (vetoCollID > -1) {
    G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
    RMCVetoHitsCollection* VHC
      = HCE ? (RMCVetoHitsCollection*)(HCE->GetHC(vetoCollID) ) : 0;

    if (VHC) {
      if (OutputTrees) rootOut.ProcessHits(evt, VHC);
    }
  }
  
 if (zipCollID > -1) {
    G4HCofThisEvent* HCE = evt->GetHCofThisEvent();

    RMCZipHitsCollection* ZHC
      = HCE ? (RMCZipHitsCollection*)(HCE->GetHC(zipCollID) ) : 0;

    if (ZHC) {
      if (OutputTrees) rootOut.ProcessHits(evt, ZHC);
    }
  }  

  if(OutputTrees)
  {
    rootOut.ProcessEventInfo(evt);
  }
}


void RMCRunAction::EndOfRunAction(const G4Run* aRun)
{
  if (verboseLevel) G4cout << ">>> End Run : " << aRun->GetRunID() << G4endl;

  if (OutputTrees) rootOut.WriteAndClose();
}


// Determine laboratory code by inspecting geometry

int RMCRunAction::GetLaboratoryCode() const {
  static const G4String labName[] =
    {"NoLab", "MITReactor", "SONGS",""};
  
  if (verboseLevel) G4cout << ">>> RMCRunAction::GetLaboratoryCode" << G4endl;

  G4bool labFound = false;
  int labCode = 9;

  for (int i=0; !labName[i].empty(); i++) {
    if (G4LogicalVolumeStore::GetInstance()->GetVolume(labName[i],false) ) {
      labCode = i;
      labFound = true;
      break;
    }
  }

  if (!labFound) {
    G4cerr << " Lab geometry not found, setting code to 9 " << G4endl;
  } else if (verboseLevel > 1) {
    G4cout << " Lab geometry " << labName[labCode]
	   << " found, setting code to " << labCode << G4endl;
  }

  return labCode;
}
