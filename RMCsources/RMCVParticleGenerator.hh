#ifndef RMCVParticleGenerator_hh
#define RMCVParticleGenerator_hh 1
////////////////////////////////////////////////////////////////////////
//  File:        RMCVParticleGenerator.hh                             //
//																`	  //
//  Description: base class for generating background particles with  //
//               specific characteristics (radiogenic or cosmogenic)  //
//                                                                    //
//  Author:      Adapted by: Adam Anderson (MIT)					  //
//				 Adapted from: Michael Kelsey (SLAC)                  //
//																	  //
//  Date:        26 March 2012                                        //
//////////////////////////////////////////////////////////////////////// 

#include "G4LorentzVector.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class G4ParticleDefinition;


class RMCVParticleGenerator {
public:
	RMCVParticleGenerator(const G4String& name="RMCVParticleGenerator",
			 G4ParticleDefinition* type=0);
	virtual ~RMCVParticleGenerator();
	
	// Subclasses MUST implement functions to generate kinematic distributions
	virtual G4double shootKineticEnergy() const = 0;
	virtual G4ThreeVector shootDirection() const = 0;
	
	// Subclasses MAY implement function to call SetParticle at generation
	virtual G4ParticleDefinition* shootParticle() const { return particle; }
	virtual void SetVerboseLevel(G4int verbose) { verboseLevel = verbose; }
	virtual void SetParticle(G4ParticleDefinition* type) { particle = type; }
	virtual G4ParticleDefinition* GetParticle() const { return particle; }
	virtual const G4LorentzVector& shoot() const;
	virtual void shoot(G4LorentzVector& mom, G4ParticleDefinition*& type) const;
	virtual void shoot(G4double& ekin, G4ThreeVector& dir,
			 G4ParticleDefinition*& type) const;
	virtual const G4LorentzVector& GetMomentum() const { return lastShoot; }
	virtual const G4String& GetName() const { return sourceName; }
	virtual void Print(std::ostream& os) const;

private:
	G4String sourceName;
	G4ThreeVector direction;
	mutable G4ParticleDefinition* particle;	// Can overwrite each event
	mutable G4LorentzVector lastShoot;

protected:
  G4int verboseLevel;			// For subclass convenience

  static const G4ThreeVector origin;	// For convenience reference (0,0,0)
};

#endif	/* RMCVParticleGenerator_hh */
