//******************************************************************************
// RMCCRYGenerator.hh
//
// This class is a class derived from G4VUserRMCCRYGenerator for 
// constructing the process used to generate incident particles.
//
// 1.00 JMV, LLNL, JAN-2007:  First version.
//******************************************************************************
// 
#ifndef RMCCRYGenerator_h
#define RMCCRYGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYUtils.h"
#include "vector"
#include "RNGWrapper.hh"
#include "RMCCRYGeneratorMessenger.hh"
#include "RMCg4base/RMCVSourceConstruction.hh"

class G4Event;

class RMCCRYGenerator : public RMCVSourceConstruction
{
  public:
    RMCCRYGenerator(const char * filename);
    ~RMCCRYGenerator();

  public:
    void GeneratePrimaries(G4Event* anEvent);
    void InputCRY();
    void UpdateCRY(std::string* MessInput);
    void CRYFromFile(G4String newValue);

  private:
    std::vector<CRYParticle*> *vect; // vector of generated particles
    G4ParticleTable* particleTable;
    G4ParticleGun* particleGun;
    CRYGenerator* gen;
    G4int InputState;
    RMCCRYGeneratorMessenger* gunMessenger;
};

#endif
