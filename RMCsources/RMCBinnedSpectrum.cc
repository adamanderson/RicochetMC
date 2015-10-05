////////////////////////////////////////////////////////////////////////
//  File:        RMCBinnedSpectrum.cc                                 //
//																	  //
//  Description: Generate particles with specified energy spectrum.   //
//                                                                    //
//  Author:      Adapted by: Adam Anderson (MIT)					  //
//				 Adapted from: Michael Kelsey (SLAC)                  //
//																	  //
//  Date:        20 March 2012                                        //
//////////////////////////////////////////////////////////////////////// 

#include "RMCsources/RMCBinnedSpectrum.hh"
#include "Randomize.hh"
//#include <algorithm>
#include <fstream>
#include <vector>

// Constructor and destructor

RMCBinnedSpectrum::RMCBinnedSpectrum(const G4String& name,
				       G4ParticleDefinition* particle,
				       const G4String& file)
  : RMCVSpectrum(name, particle) {
  ReadFile(file);
}


// Load spectrum from specified file (no action if file not readable)

void RMCBinnedSpectrum::ReadFile(const G4String& file) {
	if (file.empty()) return;	// No filename, no action
	
	if (verboseLevel) G4cout << GetName() << "::ReadFile " << file << G4endl;
	
	std::ifstream input(file);
	if (!input)
	{
		G4cerr << GetName() << " ERROR reading " << file << G4endl;
		return;
	}
	
	// Discard previous spectrum and reload
	binEnergy.clear();
	binProb.clear();
	
	G4double e, prob;
	while (!input.eof())
	{
		input >> e >> prob;
		binEnergy.push_back(e*MeV);
		binProb.push_back(prob);
	}
	
	if (verboseLevel>1)
	G4cout << " Read " << binEnergy.size() << " bins from " << file << G4endl;
	
	Normalize();			// Normalize spectrum for proper selection
	
	lastFile = file;		// Record filename for diagnostic output
}


// Select particle (kinetic) energy from spectrum
G4double RMCBinnedSpectrum::shootKineticEnergy() const {
  if (nBins() == 0) return 0.;			// No data, no action

  if (verboseLevel>2) G4cout << GetName() << "::shootKineticEnergy" << G4endl;

  return interpolate(selectBin(), selectFraction());
}


// Generate random direction; isotropic for now (FIXME!)
G4ThreeVector RMCBinnedSpectrum::shootDirection() const {
	G4ThreeVector direction(0., 0., 0.);
	direction.setTheta(pi * G4UniformRand());
	direction.setPhi(2. * pi * G4UniformRand());
	
	return direction;
}


size_t RMCBinnedSpectrum::selectBin() const {
  if (verboseLevel>2) G4cout << GetName() << "::selectBin" << G4endl;

  G4double binP = G4UniformRand();		// To choose bin from CDF

  // Find bin index corresponding to selected random value
  return (std::lower_bound(binCProb.begin(), binCProb.end(), binP)
	  - binCProb.begin() - 1);		// We return bin _below_
}

G4double RMCBinnedSpectrum::selectFraction() const {
  if (verboseLevel>2) G4cout << GetName() << "::selectFraction" << G4endl;

  // For now, just throw flat; old RMC code doesn't use slope of CDF
  return G4UniformRand();
}

G4double RMCBinnedSpectrum::interpolate(size_t bin, G4double frac) const {
  if (verboseLevel>2) 
    G4cout << GetName() << "::interpolate " << bin << " " << frac << G4endl;

  if (bin >= nBins()-1) return -1.;		// Sanity checks
  if (frac < 0 || frac > 1.) return -1.;

  G4double deltaE = binEnergy[bin+1] - binEnergy[bin];
  
  return binEnergy[bin] + frac*deltaE;
}


// Convert input PDF to CDF distribution allowing for non-uniform bins
void RMCBinnedSpectrum::Normalize() {
	if (verboseLevel>1) G4cout << GetName() << "::Normalize" << G4endl;
	
	binCProb.clear();
	binCProb.resize(binEnergy.size(),0.);
	
	// Integrate the probability distribution (trapezoid integral)
	G4double dP, dE;
	for (size_t bin=1; bin<binEnergy.size(); ++bin)
	{ 
		dP = binProb[bin]-binProb[bin-1];
		dE = binEnergy[bin]-binEnergy[bin-1];
		binCProb[bin] = binCProb[bin-1] + dE*(binProb[bin-1] + dP/2.);
	}
	
	// Normalize the cumulative distribution
	for (size_t bin=0; bin<binCProb.size(); ++bin)
	{
		binCProb[bin] /= binCProb.back();
	}
}

// Dump spectrum in same format as input file

void RMCBinnedSpectrum::Print(std::ostream& os) const {
  RMCVParticleGenerator::Print(os);

  os << " File: " << lastFile << G4endl;
  for (size_t i=0; i<nBins(); i++) {
    os << binEnergy[i] << " " << binProb[i] << G4endl;
  }
}
