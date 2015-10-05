////////////////////////////////////////////////////////////////////////
//                                                                    //
//  File:        RMCSimRoot.cc                                       //
//  Description: class to fill Root trees from G4 hit collections.    //
//		 DMC units are eV and meters.                         //
//                                                                    //
//  Author:      Dennis Wright (SLAC)                                 //
//  Date:        15 July 2010                                         //
//                                                                    //
////////////////////////////////////////////////////////////////////////
 
#include "RMCactions/RMCSimRoot.hh"
#include "G4RunManager.hh"
#include "G4VTrajectory.hh"
#include "G4VTrajectoryPoint.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Event.hh"
#include "G4TransportationManager.hh" 
#include "G4Navigator.hh" 
#include "TFile.h"
#include "TTree.h"

#include <sstream>
#include <iomanip>
#include <time.h>

RMCSimRoot::RMCSimRoot(int verbose)
  : verboseLevel(verbose), tfile(0), eventTree(0), vetoTree(0), runTree(0) {
  for (int i = 0; i < RMCSimRoot::NZIPS; i++) zipTree[i] = 0;
}

RMCSimRoot::~RMCSimRoot()
{
  WriteAndClose();	
}


void RMCSimRoot::ProcessEventInfo(const G4Event* evt)
{
  if (!evt) return;	// Sanity check

  evnum = evt->GetEventID();
  zppevt = 0;

  if (verboseLevel>1)
    G4cout << ">>> ProcessEventInfo for Event #" << evnum << G4endl;

  LoadEventData(evt);
 if(!eventTree) CreateEventTree();
 eventTree->Fill();
}


void RMCSimRoot::ProcessHits(const G4Event* evt, RMCZipHitsCollection* ZHC)
{
  if (!evt || !ZHC) return;	// Sanity check

  const int nZipHits = ZHC->entries();
  nhit = nZipHits;
  nvhit = nVetoHits;
  if (verboseLevel>1)
    G4cout << ">>> ProcessHits with " << nZipHits << " ZIP hits" << G4endl;

  // Fill the zip hits
  // Start with the data common to each event
  if(nZipHits > 0)
  {
	ev = evt->GetEventID();
	
	for (int i = 0; i < nZipHits; i++)
	{
	p[i] = (double)0.; 
    par_initpx[i] = (double)-1.;
    par_initpy[i] = (double)-1.;
    par_initpz[i] = (double)-1.;
    par_initx[i] = (double)-1.;
    par_inity[i] = (double)-1.;
    par_initz[i] = (double)-1.;
		
	LoadHitData(evt, (*ZHC)[i], i);
    detnum[i]=(*ZHC)[i]->GetReplicaNum();
    type[i] = (*ZHC)[i]->GetPID();      
	parentprocID.push_back((*ZHC)[i]->GetParentProcID());
	G4int parentID = (*ZHC)[i]->GetParentID();
    G4TrajectoryContainer* container = 	G4RunManager::GetRunManager()->GetCurrentEvent()->GetTrajectoryContainer();
    
  	if(container!=0) 
  	{
  	std::size_t nTraj = container->size();
  	 for(std::size_t j=0;j<nTraj;j++)
 	 {
     G4VTrajectory* tr1 = (G4VTrajectory*)((*container)[j]);
     if(tr1->GetTrackID()==parentID)
     {
     p[i] = tr1->GetPDGEncoding(); 
     par_initpx[i] = (double)(tr1->GetInitialMomentum().x()/eV);
     par_initpy[i] = (double)(tr1->GetInitialMomentum().y()/eV);
     par_initpz[i] = (double)(tr1->GetInitialMomentum().z()/eV);
     par_initx[i] = (double)(tr1->GetPoint(0)->GetPosition().x()/meter);
     par_inity[i] = (double)(tr1->GetPoint(0)->GetPosition().y()/meter);
     par_initz[i] = (double)(tr1->GetPoint(0)->GetPosition().z()/meter); 
     }   
  	 }   
 	}
	  	 
	}
        
	  if (!zipTree[0]) CreateAZipTree(0);
	  zipTree[0]->Fill();
	  procID.clear();
	  parentprocID.clear();
  }
}

void RMCSimRoot::ProcessHits(const G4Event* evt, RMCVetoHitsCollection* VHC)
{
  if (!evt || !VHC) return;	// Sanity check

  nVetoHits = VHC->entries();

  if (verboseLevel>1)
    G4cout << ">>> ProcessHits with " << nVetoHits << " veto hits" << G4endl;

  for (int i = 0; i < nVetoHits; i++) 
  {
  LoadHitData(evt, (*VHC)[i]);

  if (!vetoTree) CreateVetoTree();
  vetoTree->Fill(); 
  }
}

void RMCSimRoot::LoadHitData(const G4Event* evt, const RMCZipHit* ZipHit, int entry)
{
  if (!evt || !ZipHit) return;	// Sanity check
  
  e1[entry] = ZipHit->GetPreStepKE()/eV;
  d3[entry] = (double)(ZipHit->GetEdep()/eV);  
  px1[entry] = (double)(ZipHit->GetPreStepMomentum().x()/eV);
  py1[entry] = (double)(ZipHit->GetPreStepMomentum().y()/eV);
  pz1[entry] = (double)(ZipHit->GetPreStepMomentum().z()/eV);
  x1[entry] = (double)(ZipHit->GetPreStepPosition().x()/meter);
  y1[entry] = (double)(ZipHit->GetPreStepPosition().y()/meter);
  z1[entry] = (double)(ZipHit->GetPreStepPosition().z()/meter);
  t1[entry] = (double)(ZipHit->GetPreStepTime()/ns);
  procID.push_back(ZipHit->GetProcID());
}

void RMCSimRoot::LoadHitData(const G4Event* evt, const RMCVetoHit* VetoHit) {
  if (!evt || !VetoHit) return;	// Sanity check

  vev = evt->GetEventID();
  vdetnum = VetoHit->GetReplicaNum();
  vts = VetoHit->GetTrackID()*100000 + VetoHit->GetStepNum();
  vtype = VetoHit->GetPID();
  ve1 = VetoHit->GetPreStepKE()/eV;
  vd3 = VetoHit->GetEdep()/eV;
  vpx1 = VetoHit->GetPreStepMomentum().x()/eV;
  vpy1 = VetoHit->GetPreStepMomentum().y()/eV;
  vpz1 = VetoHit->GetPreStepMomentum().z()/eV;
  vx1 = VetoHit->GetPreStepPosition().x()/meter;
  vy1 = VetoHit->GetPreStepPosition().y()/meter;
  vz1 = VetoHit->GetPreStepPosition().z()/meter;
  vt1 = VetoHit->GetPreStepTime()/ns;
}

void RMCSimRoot::LoadEventData(const G4Event* evt)
{
  if (!evt) return;
  
  G4PrimaryVertex* thisPrimaryVertex = evt->GetPrimaryVertex();
  G4PrimaryParticle* thisPrimaryParticle = thisPrimaryVertex->GetPrimary();
  primaryPDG = thisPrimaryParticle->GetPDGcode();
  primaryX = thisPrimaryVertex->GetX0();
  primaryY = thisPrimaryVertex->GetY0();
  primaryZ = thisPrimaryVertex->GetZ0();
  G4Navigator* theNavigator =   	G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4ThreeVector location(primaryX,primaryY,primaryZ);
  G4VPhysicalVolume* myVolume =  theNavigator->LocateGlobalPointAndSetup(location);
  primaryVolume = myVolume->GetName();
  primaryTime = thisPrimaryVertex->GetT0()-previousTime;
  previousTime = thisPrimaryVertex->GetT0();
  primaryPx = thisPrimaryParticle->GetPx();
  primaryPy = thisPrimaryParticle->GetPy();
  primaryPz = thisPrimaryParticle->GetPz();
  primaryKE = sqrt(pow(primaryPx,2) + pow(primaryPy,2) + pow(primaryPz,2) + pow(thisPrimaryParticle->GetMass(),2)) - thisPrimaryParticle->GetMass();
}


void RMCSimRoot::WriteAndClose()
{
	if (verboseLevel) G4cout << ">>> RMCSimRoot::WriteAndClose" << G4endl;

	for (int i = 0; i < 1; i++) {
		if (zipTree[i]) {
			zipTree[i]->Write();
			delete zipTree[i];
			zipTree[i] = 0;
		}
	}

	if (vetoTree) {
		vetoTree->Write();
		delete vetoTree;
		vetoTree = 0;
	}

	if (eventTree) {
		eventTree->Write();
		delete eventTree;
		eventTree = 0;
	}
	
	if (runTree) {
	    exposureTime=primaryTime+previousTime;
	    runTree->Fill();
		runTree->Write();
		delete runTree;
		runTree = 0;
	}

	if (tfile) {
		tfile->Write("", TObject::kOverwrite);
		tfile->Close();
		delete tfile;
		tfile = 0;
	}
}


void RMCSimRoot::SetupFile(const G4String& prefix,int runNumber,
			    int labCode) {
  if (tfile) WriteAndClose();			// Replace previous data file

  G4String fileName = FileName(prefix, runNumber, labCode);

  if (verboseLevel)
    G4cout << " Creating RMC simulation ROOT file " << fileName << G4endl;

  tfile = new TFile("myG4output.root","RECREATE");	// Overwrite pervious version
  tfile->mkdir("G4SimDir");
  tfile->mkdir("G4SettingsInfoDir");
  tfile->cd("G4SimDir");
  previousTime=0.;
  CreateEventTree();
  CreateRunTree();
}


// Filename format is PREFIX_LLYYMMDD_RRRR.root (LL is 10 + lab code)

G4String RMCSimRoot::FileName(const G4String& prefix, int runNumber,
			       int labCode) const {
  time_t now;		  // Get <time.h> structure with local calendar data
  time(&now);
  struct tm* sNow = localtime(&now);
  int yymmdd = 100*(100*(sNow->tm_year%100)+(sNow->tm_mon+1))+sNow->tm_mday;
  std::ostringstream fn;

  // NOTE: The lab code must be one digit, and gets a prefixed "1"
  fn << prefix << "_" << std::setfill('0') << std::setw(2) << 10+(labCode%10)
     << std::setw(6) << yymmdd << "_" << std::setw(4) << runNumber << ".root";

  G4String fileName(fn.str());
  return fileName;
}


void RMCSimRoot::CreateEventTree()
{
  if (eventTree) return;	// Avoid leaks

  if (verboseLevel)
    G4cout << " eventName = event"
	   << " eventDescription = G4 simulated event info" << G4endl;

  eventTree = new TTree("primary", "G4 simulated event info");
  eventTree->Branch("EventNum", &evnum, "evnum/I");
  eventTree->Branch("PrimaryVolume", &primaryVolume);
  eventTree->Branch("PrimaryPDG", &primaryPDG, "primaryPDG/D");
  eventTree->Branch("PrimaryX", &primaryX, "primaryX/D");
  eventTree->Branch("PrimaryY", &primaryY, "primaryY/D");
  eventTree->Branch("PrimaryZ", &primaryZ, "primaryZ/D");
  eventTree->Branch("PrimaryTime", &primaryTime, "primaryTime/D");
  eventTree->Branch("PrimaryPx", &primaryPx, "primaryPx/D");
  eventTree->Branch("PrimaryPy", &primaryPy, "primaryPy/D");
  eventTree->Branch("PrimaryPz", &primaryPz, "primaryPz/D");
  eventTree->Branch("PrimaryKE", &primaryKE, "primaryKE/D");
}

void RMCSimRoot::CreateRunTree()
{
  if (runTree) return;	// Avoid leaks
  runTree = new TTree("runinfo", "G4 simulated run info");
  runTree->Branch("ExposureTime", &exposureTime, "exposureTime/D");
}


void RMCSimRoot::CreateVetoTree()
{ 
  if (vetoTree) return;		// Avoid leaks

  G4String vetoTreeName = "veto";
  G4String vetoTreeDescription = "G4 data for tree mcveto";

  if (verboseLevel)
    G4cout << " vetoName = " << vetoTreeName 
	   << " vetoDescription = " << vetoTreeDescription << G4endl; 

  vetoTree = new TTree(vetoTreeName, vetoTreeDescription);
  vetoTree->Branch("EventNum", &vev, "ev/I");
  vetoTree->Branch("DetNum", &vdetnum, "dt/I");
  vetoTree->Branch("PDG", &vtype, "type/D");
  vetoTree->Branch("KE", &ve1, "e1/D");
  vetoTree->Branch("Edep", &vd3, "d3/D");
  vetoTree->Branch("Xmom1", &vpx1, "px1/D");
  vetoTree->Branch("Ymom1", &vpy1, "py1/D");
  vetoTree->Branch("Zmom1", &vpz1, "pz1/D");
  vetoTree->Branch("X1", &vx1, "x1/D");
  vetoTree->Branch("Y1", &vy1, "y1/D");
  vetoTree->Branch("Z1", &vz1, "z1/D");
  vetoTree->Branch("Time1", &vt1, "t1/D");
}


void RMCSimRoot::CreateAZipTree(int i)
{
	if (zipTree[i]) return;	// Avoid leaks
	
	std::ostringstream os;
	os << i+1;
	G4String zipTreeName = "ziphit";
	G4String zipTreeDescription = "G4 data for tree " + zipTreeName;
	
	if (verboseLevel)
	G4cout << " zipName = " << zipTreeName 
	   	   << " zipDescription = " << zipTreeDescription << G4endl; 
	
	zipTree[i] = new TTree(zipTreeName, zipTreeDescription);
	zipTree[i]->Branch("EventNum", &ev, "ev/I");
	zipTree[i]->Branch("NumOfHits", &nhit, "nhit/I");
	zipTree[i]->Branch("NumOfVetoHits", &nvhit, "nvhit/I");
	zipTree[i]->Branch("ProcID", &procID);
	zipTree[i]->Branch("ParentProcID", &parentprocID);
	zipTree[i]->Branch("ParentinitXmom", &par_initpx, "par_initpx[nhit]/D");
	zipTree[i]->Branch("ParentinitYmom", &par_initpy, "par_initpy[nhit]/D");
	zipTree[i]->Branch("ParentinitZmom", &par_initpz, "par_initpz[nhit]/D");
	zipTree[i]->Branch("ParentinitX", &par_initpx, "par_initx[nhit]/D");
	zipTree[i]->Branch("ParentinitY", &par_initpy, "par_inity[nhit]/D");
	zipTree[i]->Branch("ParentinitZ", &par_initpz, "par_initz[nhit]/D");
	zipTree[i]->Branch("ParentPDG", &p, "p[nhit]/D");
	zipTree[i]->Branch("PDG", &type, "type[nhit]/D");
	zipTree[i]->Branch("DetNum", detnum, "detnum[nhit]/I");
	zipTree[i]->Branch("KE", e1, "e1[nhit]/D");
	zipTree[i]->Branch("Edep", d3, "d3[nhit]/D");
	zipTree[i]->Branch("Xmom1", px1, "px1[nhit]/D");
	zipTree[i]->Branch("Ymom1", py1, "py1[nhit]/D");
	zipTree[i]->Branch("Zmom1", pz1, "pz1[nhit]/D");
	zipTree[i]->Branch("X1", x1, "x1[nhit]/D");
	zipTree[i]->Branch("Y1", y1, "y1[nhit]/D");
	zipTree[i]->Branch("Z1", z1, "z1[nhit]/D");
	zipTree[i]->Branch("Time1", t1, "t1[nhit]/D");
}


