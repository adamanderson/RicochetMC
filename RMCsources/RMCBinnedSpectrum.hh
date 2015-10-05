#ifndef RMCBinnedSpectrum_hh
#define RMCBinnedSpectrum_hh 1
////////////////////////////////////////////////////////////////////////
//  File:        RMCBinnedSpectrum.hh                                 //
//																	  //
//  Description: Generate particles with specified energy spectrum    //
//                                                                    //
//  Author:      Adapted by: Adam Anderson (MIT)					  //
//               Adpated from: Michael Kelsey (SLAC)                  //
//																	  //
//  Date:        20 March 2012                                        //
//////////////////////////////////////////////////////////////////////// 

#include "RMCsources/RMCVSpectrum.hh"
#include <vector>

class G4ParticleDefinition;


class RMCBinnedSpectrum : public RMCVSpectrum {
public:
	RMCBinnedSpectrum(const G4String& name, G4ParticleDefinition* particle,
		     		  const G4String& file="");

	virtual ~RMCBinnedSpectrum() {}

  	virtual G4double shootKineticEnergy() const;	// Generate decay from spectrum
  	virtual G4ThreeVector shootDirection() const;   

  	virtual void Print(std::ostream& os) const;	// Report spectrum data

protected:
	void ReadFile(const G4String& file);		// Overwrites previous content

private:
	void Normalize();			// Normalize PDF values for bin width

	size_t selectBin() const;		// Choose random bin from spectrum
	G4double selectFraction() const;	// Choose random interpolation in bin
	G4double interpolate(size_t bin, G4double frac) const;
	
	size_t nBins() const { return binEnergy.size(); }
	
	G4String lastFile;			// Filename for diagnostic messages
	std::vector<G4double> binEnergy;	// Energy bin edges
	std::vector<G4double> binProb;	// Input probability bin-edge values
	std::vector<G4double> binCProb;	// Cumulative probability values
};

#endif /* RMCBinnedSpectrum_hh */
