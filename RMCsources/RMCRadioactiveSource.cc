////////////////////////////////////////////////////////////////////////
//  File:        RMCRadioactiveSource.cc                              //
//																      //
//  Description: General base class for radioactive sources			  //
//                                                                    //
//  Author:      Adam Anderson (MIT)					 			  //
//																	  //
//  Date:        20 March 2012                                        //
////////////////////////////////////////////////////////////////////////

#include "RMCsources/RMCRadioactiveSource.hh"
#include "RMCsources/RMCRadioactiveSourceMessenger.hh"
#include "RMCgeometry/detectors/RMCVesselConstruction.hh"
#include "RMCgeometry/detectors/RMCShieldConstruction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include <math.h>

// Constructor
RMCRadioactiveSource::RMCRadioactiveSource() : particlesPerEvent(1), sourceSpectrum(0),
											   sourceName(""), particleType(0), spectrumFile(""),
											   theMessenger(new RMCRadioactiveSourceMessenger(this))
{
	particleTable = G4ParticleTable::GetParticleTable();
	SetParticleType(particleTable->FindParticle("gamma"));
	G4ThreeVector direction(0., 0., 0.);
	
	// Fix the global setings for the particleGun
	particleGun->SetNumberOfParticles(particlesPerEvent);
	particleGun->SetParticleDefinition(particleType);	

	// Just some code for debugging
	/*for(int jVessel = 0; jVessel < nVessels; jVessel++)
	{
		G4cout << "rmin[" << jVessel << "] = " << rmin[jVessel] << G4endl;
		G4cout << "rmax[" << jVessel << "] = " << rmax[jVessel] << G4endl;
		G4cout << "vesselThickness[" << jVessel << "] = " << vesselThickness[jVessel] << G4endl;
	}	*/		   
}


RMCRadioactiveSource::~RMCRadioactiveSource()
{
	/*delete sourceSpectrum;
	delete particleType;
	delete particleTable;
	delete theMessenger;*/
}


// Set the particle type
void RMCRadioactiveSource::SetParticleType(G4ParticleDefinition* partDef)
{
	particleType = partDef;
}


// Set the particle spectrum
void RMCRadioactiveSource::SetParticleSpectrum(G4String filename)
{
	spectrumFile = filename;
	sourceSpectrum = new RMCBinnedSpectrum("default", particleType, spectrumFile);
}


// Set the volume to the Cu vessels
void RMCRadioactiveSource::SetVolumeToVessels()
{
	theVessel = new RMCVesselConstruction();
	theVessel->FillExtraParameters();

	// Get information from vessel:
	nVessels = theVessel->GetNStages();
	innerRad = theVessel->GetVessel0Rad();
	deltaRad = theVessel->GetVesselDeltaRad();
	vesselThickness = theVessel->GetVesselThick();
	vesselHeight = theVessel->GetVesselHeight();
	vesselVolume.reserve(nVessels);
	rmax.reserve(nVessels);
	rmin.reserve(nVessels);
	totalVolume = 0.0;

	// Compute the volume of each of the layers:
	rmin[0] = innerRad;
	rmax[0] = innerRad + vesselThickness[0];
	vesselVolume[0] = pi * (pow(rmax[0],2) - pow(rmin[0],2));
	totalVolume += vesselVolume[0];
	for(int jVessel = 1; jVessel < nVessels; jVessel++)
	{
		rmin[jVessel] = rmax[jVessel-1] + deltaRad;
		rmax[jVessel] = rmin[jVessel] + vesselThickness[jVessel];
		vesselVolume[jVessel] = pi * (pow(rmax[jVessel],2) - pow(rmin[jVessel],2)) * vesselHeight[jVessel] +
								2.0 * pi * pow(rmax[jVessel],2) * vesselThickness[jVessel];
		totalVolume += vesselVolume[jVessel];
	}
}


// Set up the shield to be used by the event injectors
void RMCRadioactiveSource::SetUpShield()
{
	nVessels = 1;
	rmin.reserve(1);
	rmax.reserve(1);
	vesselHeight.reserve(1);
	vesselVolume.reserve(1);
	vesselThickness.reserve(1);
	theShield = new RMCShieldConstruction();
	theShield->FillExtraParameters();
}


// Set the volume to the inner poly
void RMCRadioactiveSource::SetVolumeToInnerPoly()
{
	SetUpShield();
	vesselThickness[0] = theShield->GetInnerPolyParams().SideThick;
	rmin[0] = theShield->GetInnerPolyParams().Radius - vesselThickness[0];
	rmax[0] = theShield->GetInnerPolyParams().Radius;
	vesselHeight[0] = 2.0 * theShield->GetInnerPolyParams().Length;
	vesselVolume[0] = pi * (pow(rmax[0],2) - pow(rmin[0],2)) * vesselHeight[0] +
								2.0 * pi * pow(rmax[0],2) * vesselThickness[0];
	totalVolume = vesselVolume[0];
}


// Set the volume to the inner poly
void RMCRadioactiveSource::SetVolumeToInnerLead()
{
	SetUpShield();
	vesselThickness[0] = theShield->GetInnerLeadParams().SideThick;
	rmin[0] = theShield->GetInnerLeadParams().Radius - vesselThickness[0];
	rmax[0] = theShield->GetInnerLeadParams().Radius;
	vesselHeight[0] = 2.0 * theShield->GetInnerLeadParams().Length;
	vesselVolume[0] = pi * (pow(rmax[0],2) - pow(rmin[0],2)) * vesselHeight[0] +
								2.0 * pi * pow(rmax[0],2) * vesselThickness[0];
	totalVolume = vesselVolume[0];
}


// Set the volume to the inner poly
void RMCRadioactiveSource::SetVolumeToOuterPoly()
{
	SetUpShield();
	vesselThickness[0] = theShield->GetOuterPolyParams().SideThick;
	rmin[0] = theShield->GetOuterPolyParams().Radius - vesselThickness[0];
	rmax[0] = theShield->GetOuterPolyParams().Radius;
	vesselHeight[0] = 2.0 * theShield->GetOuterPolyParams().Length;
	vesselVolume[0] = pi * (pow(rmax[0],2) - pow(rmin[0],2)) * vesselHeight[0] +
								2.0 * pi * pow(rmax[0],2) * vesselThickness[0];
	totalVolume = vesselVolume[0];
}


// Set the volume to the inner poly
void RMCRadioactiveSource::SetVolumeToOuterLead()
{
	SetUpShield();
	vesselThickness[0] = theShield->GetOuterLeadParams().SideThick;
	rmin[0] = theShield->GetOuterLeadParams().Radius - vesselThickness[0];
	rmax[0] = theShield->GetOuterLeadParams().Radius;
	vesselHeight[0] = 2.0 * theShield->GetOuterLeadParams().Length;
	vesselVolume[0] = pi * (pow(rmax[0],2) - pow(rmin[0],2)) * vesselHeight[0] +
								2.0 * pi * pow(rmax[0],2) * vesselThickness[0];
	totalVolume = vesselVolume[0];
}


// Mandatory function for generating primary events
void RMCRadioactiveSource::GeneratePrimaries(G4Event* anEvent)
{
	// choose volume
	G4double randVol = G4UniformRand();
	G4double cumulVolume = vesselVolume[0];
	G4int volToPick = 0;
	while(randVol * totalVolume > cumulVolume && volToPick < nVessels)
	{
		volToPick++;
		cumulVolume += vesselVolume[volToPick];
	}

	// choose whether to inject in sides or top/bottom
	randVol = G4UniformRand();
	G4double sidesVol = pi * (pow(rmax[volToPick],2) - pow(rmin[volToPick],2)) * vesselHeight[volToPick];
	G4double endsVol = 2.0 * pi * pow(rmax[volToPick],2) * vesselThickness[volToPick];
	G4double randPosR, randPosPhi, randPosZ;

	// choose sides
	if(randVol < sidesVol / totalVolume)
	{
		randPosR = sqrt((pow(rmax[volToPick],2) - pow(rmin[volToPick],2)) * G4UniformRand() + pow(rmin[volToPick],2));
		randPosPhi = 2.0 * pi * G4UniformRand();
		randPosZ = vesselHeight[volToPick] * (G4UniformRand() - 0.5);
	}
	// choose ends
	else
	{
		randPosR = sqrt(pow(rmax[volToPick],2) * G4UniformRand());
		randPosPhi = 2.0 * pi * G4UniformRand();
		// choose top by default, but switch to bottom half the time
		randPosZ = (vesselHeight[volToPick] / 2) + G4UniformRand() * vesselThickness[volToPick];
		G4double topOrBottom = G4UniformRand() - 0.5;
		if (topOrBottom < 0)
			randPosZ = -1.0 * randPosZ;
	}
	// set the particle position information
	position.setRhoPhiZ(randPosR, randPosPhi, randPosZ);
	position = position + G4ThreeVector(4.5*m, 0.0*m, 1.5*m);
	//G4cout << "(rho,phi,z) = (" << randPosR << "," << randPosPhi << "," << randPosZ << ")" << G4endl;
	particleGun->SetParticlePosition(position);
	
	
	// Set particle momentum direction
	// This is isotropic for radiogenic backgrounds
	G4double randDirTheta = acos(2.0 * (G4UniformRand() - 0.5));
	G4double randDirPhi = 2.0 * pi * G4UniformRand();
	direction.setTheta(randDirTheta);
	direction.setPhi(randDirPhi);
	direction.setRThetaPhi(1, randDirTheta, randDirPhi);
	particleGun->SetParticleMomentumDirection(direction);
	

	// Calculate the particle energy
	energy = sourceSpectrum->shootKineticEnergy();
	particleGun->SetParticleEnergy(energy);
	

	// Set the event
	particleGun->GeneratePrimaryVertex(anEvent);
}
