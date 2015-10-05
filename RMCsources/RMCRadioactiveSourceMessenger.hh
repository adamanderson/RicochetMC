////////////////////////////////////////////////////////////////////////
//  File:        RMCRadioactiveSourceMessenger.hh                     //
//																      //
//  Description: Messenger class for radioactive sources			  //
//                                                                    //
//  Author:      Adam Anderson (MIT)					 			  //
//																	  //
//  Date:        26 March 2012                                        //
////////////////////////////////////////////////////////////////////////

#ifndef RMCRadioactiveSourceMessenger_hh
#define RMCRadioactiveSourceMessenger_hh 1

#include "G4UImessenger.hh"
#include "RMCsources/RMCRadioactiveSource.hh"
#include "globals.hh"

class G4ParticleTable;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcommand;
class RMCRadioactiveSource;

class RMCRadioactiveSourceMessenger : public G4UImessenger
{
public:
	RMCRadioactiveSourceMessenger(RMCRadioactiveSource* srcToSet);
	~RMCRadioactiveSourceMessenger();
	
	void SetNewValue(G4UIcommand *command, G4String newValues);
	
private:
	RMCRadioactiveSource* 		theSource;
	G4ParticleTable* 			particleTable;
	G4UIdirectory*             	gunDirectory;
	G4UIcmdWithAString*			particleCmd;
	G4UIcmdWithAString*			energyCmd;
	G4UIcmdWithAString*			volumeCmd;
	
};
#endif
