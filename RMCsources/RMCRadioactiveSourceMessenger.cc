////////////////////////////////////////////////////////////////////////
//  File:        RMCRadioactiveSourceMessenger.hh                     //
//																      //
//  Description: Messenger class for radioactive sources			  //
//                                                                    //
//  Author:      Adam Anderson (MIT)					 			  //
//																	  //
//  Date:        26 March 2012                                        //
////////////////////////////////////////////////////////////////////////

#include "RMCsources/RMCRadioactiveSourceMessenger.hh"
#include "RMCsources/RMCRadioactiveSource.hh"
#include "G4ParticleTable.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include <string.h>



RMCRadioactiveSourceMessenger::RMCRadioactiveSourceMessenger(RMCRadioactiveSource* srcToSet) : 
theSource(srcToSet)
{
	particleTable = G4ParticleTable::GetParticleTable();
	
	// Set up the source control directory
	gunDirectory = new G4UIdirectory("/RMC/RadSrcControl/");
  	gunDirectory->SetGuidance("Radioactive sources control commands.");
  	
  	// set particle  
	particleCmd = new G4UIcmdWithAString("/RMC/RadSrcControl/particle",this);
	particleCmd->SetGuidance("Sets particle to be generated.");
	particleCmd->SetGuidance("(geantino is default)");
	particleCmd->SetParameterName("particleName",true,false);
	particleCmd->SetDefaultValue("geantino");
	particleCmd->SetCandidates("gamma beta");

	// set the energy spectrum file
	energyCmd = new G4UIcmdWithAString("/RMC/RadSrcControl/ESpectrum",this);
	energyCmd->SetGuidance("Sets energy distribution type");
	energyCmd->SetParameterName("EnergyDis",false);
	
	// set the volume on which to inject primaries
	volumeCmd = new G4UIcmdWithAString("/RMC/RadSrcControl/volume", this);
	volumeCmd->SetGuidance("Sets volume on which to inject radioactive contamination.");
	volumeCmd->SetGuidance("(vessels is default)");
	volumeCmd->SetParameterName("volumeName",true,false);
	volumeCmd->SetDefaultValue("vessels");
	volumeCmd->SetCandidates("vessels innerpoly innerlead outerpoly outerlead");
}

RMCRadioactiveSourceMessenger::~RMCRadioactiveSourceMessenger()
{
	delete theSource;
	delete particleCmd;
	delete energyCmd;
	delete volumeCmd;
	delete particleTable;
}

void RMCRadioactiveSourceMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
	if(command == particleCmd)
	{
		G4ParticleDefinition* pd = particleTable->FindParticle(newValues);
      	if(pd != NULL)
  		{
  			theSource->SetParticleType(pd);
  		}
	}
	else if(command == energyCmd)
	{
		theSource->SetParticleSpectrum(newValues);
	}  
	else if(command == volumeCmd)
	{
		if(strcmp("vessels", newValues) == 0)
			theSource->SetVolumeToVessels();
		else if(strcmp("innerpoly", newValues) == 0)
			theSource->SetVolumeToInnerPoly();
		else if(strcmp("innerlead", newValues) == 0)
			theSource->SetVolumeToInnerLead();
		else if(strcmp("outerpoly", newValues) == 0)
			theSource->SetVolumeToOuterPoly();
		else if(strcmp("outerlead", newValues) == 0)
			theSource->SetVolumeToOuterLead();
	}
	else
	G4cout << "Error entering command" << G4endl;
}
