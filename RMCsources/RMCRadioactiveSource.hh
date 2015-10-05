////////////////////////////////////////////////////////////////////////
//  File:        RMCRadioactiveSource.hh                              //
//																      //
//  Description: General base class for radioactive sources			  //
//                                                                    //
//  Author:      Adam Anderson (MIT)					 			  //
//																	  //
//  Date:        20 March 2012                                        //
////////////////////////////////////////////////////////////////////////

#ifndef RMCRadioactiveSource_hh
#define RMCRadioactiveSource_hh 1

#include "RMCg4base/RMCVSourceConstruction.hh"
#include "RMCsources/RMCRadioactiveSourceMessenger.hh"
#include "RMCsources/RMCBinnedSpectrum.hh"
#include "RMCgeometry/detectors/RMCVesselConstruction.hh"
#include "RMCgeometry/detectors/RMCShieldConstruction.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"

class RMCRadioactiveSourceMessenger;
class G4ParticleGun;
class G4Event;

class RMCRadioactiveSource : public RMCVSourceConstruction
{
public:
	RMCRadioactiveSource();
	~RMCRadioactiveSource();

	void GeneratePrimaries(G4Event* thisEvent);
	void SetParticleType(G4ParticleDefinition* partDef);
	void SetParticleSpectrum(G4String filename);
	void SetVolumeToVessels();
	void SetVolumeToInnerPoly();
	void SetVolumeToInnerLead();
	void SetVolumeToOuterPoly();
	void SetVolumeToOuterLead();
	
private:
	void SetUpShield();

	G4int particlesPerEvent;
	RMCBinnedSpectrum* sourceSpectrum;
	G4String sourceName;
	G4ParticleDefinition* particleType;
	G4String spectrumFile;
	G4ThreeVector direction;
	G4ThreeVector position;
	G4double energy;
	G4ParticleTable* particleTable;
	RMCRadioactiveSourceMessenger* theMessenger;
	RMCVesselConstruction* theVessel;
	RMCShieldConstruction* theShield;

	G4double innerRad;
	G4double deltaRad;
	G4double totalVolume;
	std::vector<G4double> vesselThickness;
	std::vector<G4double> vesselHeight;
	std::vector<G4double> rmax;
	std::vector<G4double> rmin;
	std::vector<G4double> vesselVolume;
	G4int nVessels;
};

#endif	/* RMCRadioactiveSource_hh */
