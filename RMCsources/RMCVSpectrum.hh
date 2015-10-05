#ifndef RMCVSpectrum_hh
#define RMCVSpectrum_hh 1
////////////////////////////////////////////////////////////////////////
//  File:        RMCVSpectrum.hh                                      //
//																      //
//  Description: Abstract base class for generated particles from     //
//               a hardwired spectrum, such as lines or a histogram.  //
//                                                                    //
//  Author:      Adapted by: Adam Anderson (MIT)					  //
//				 Adapted from: Michael Kelsey (SLAC)                  //
//																	  //
//  Date:        20 March 2012                                       //
//                                                                    //
//////////////////////////////////////////////////////////////////////// 

#include "RMCsources/RMCVParticleGenerator.hh"

class G4ParticleDefinition;


class RMCVSpectrum : public RMCVParticleGenerator {
public:
	RMCVSpectrum(const G4String& name, G4ParticleDefinition* particle)
		: RMCVParticleGenerator(name, particle) {}

	virtual ~RMCVSpectrum() {}

	virtual G4double shootKineticEnergy() const = 0;
	virtual G4ThreeVector shootDirection() const = 0;
};

#endif /* RMCVSpectrum_hh */
