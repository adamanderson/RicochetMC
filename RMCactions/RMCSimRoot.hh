////////////////////////////////////////////////////////////////////////
//                                                                    //
//  File:        RMCSimRoot.hh                                        //
//  Description: class to output G4 hit collections to Root trees     //
//		 DMC units are eV and meters.                                 //
//                                                                    //
//                                                                    //
//  Author:      Dennis Wright (SLAC)                                 //
//  Date:        15 July 2010                                         //
//                                                                    //
//////////////////////////////////////////////////////////////////////// 

#ifndef RMCSimRoot_hh
#define RMCSimRoot_hh 1

#include "RMCgeometry/detectors/RMCZipHit.hh"
#include "RMCgeometry/detectors/RMCVetoHit.hh"
#include <string>
#include <vector>
const int maxentries=10000;
class G4Event;
class TFile;
class TTree;


class RMCSimRoot
{
public:
  RMCSimRoot(int verbose=0);
  virtual ~RMCSimRoot();
  
  void SetupFile(const G4String& prefix, int runNumber, int labCode);
  void ProcessHits(const G4Event*, RMCZipHitsCollection*);
  void ProcessHits(const G4Event*, RMCVetoHitsCollection*);
  void ProcessEventInfo(const G4Event*);
  void WriteAndClose();  
  void SetVerboseLevel(int verbose=1) { verboseLevel = verbose; }
  G4bool GetVerboseLevel() const { return verboseLevel; }

private:
  G4String FileName(const G4String& prefix, int runNumber, int labCode) const;

  double d3[maxentries];
  double e1[maxentries];
  double px1[maxentries];
  double py1[maxentries];
  double pz1[maxentries];
  double x1[maxentries];
  double y1[maxentries];
  double z1[maxentries];
  double t1[maxentries];
  std::vector<std::string> procID; 
  int detnum[maxentries];
  int nVetoHits;
  void CreateEventTree();
  void CreateVetoTree();
  void CreateRunTree();
  void CreateAZipTree(int i);

  void LoadHitData(const G4Event* evt, const RMCZipHit* ZipHit, int entry);
  void LoadHitData(const G4Event* evt, const RMCVetoHit* VetoHit);
  void LoadEventData(const G4Event* evt);

  int verboseLevel;
  
  // Leaf variables for zip trees 
  int nhit;
  int nvhit;
  int ev;
  int ts;
  double p[maxentries];
  double par_initpx[maxentries];
  double par_initpy[maxentries];
  double par_initpz[maxentries];
  double par_initx[maxentries];
  double par_inity[maxentries];
  double par_initz[maxentries];
  std::vector<std::string> parentprocID;
  double type[maxentries];

  // Leaf variables for veto tree 
  int vev;
  int vdetnum;
  int vts;
  double vtype;
  double ve1;
  double vd3;
  double vpx3;
  double vpy3;
  double vpz3;
  double vx3;
  double vy3;
  double vz3;
  double vt3;
  double vpx1;
  double vpy1;
  double vpz1;
  double vx1;
  double vy1;
  double vz1;
  double vt1;

  // Leaf variables for event tree
  int evnum;
  int zppevt;
  std::string primaryVolume;
  double primaryPDG;
  double primaryX;
  double primaryY;
  double primaryZ;
  double primaryTime;
  double previousTime;
  double primaryPx;
  double primaryPy;
  double primaryPz;
  double primaryKE;
  
  double exposureTime;
  
  enum {NZIPS = 12};
  
  // TFile and TTrees
  TFile* tfile;
  TTree* eventTree;
  TTree* vetoTree;
  TTree* runTree;
  TTree* zipTree[RMCSimRoot::NZIPS];
};

#endif
