//******************************************************************************
// RMCCRYGenerator.cc
//
// 1.00 JMV, LLNL, Jan-2007:  First version.
//******************************************************************************
//

#include <iomanip>
#include "RMCCRYGenerator.hh"
using namespace std;

#include "G4Event.hh"

//----------------------------------------------------------------------------//
RMCCRYGenerator::RMCCRYGenerator(const char *inputfile) : RMCVSourceConstruction()
{
  // define a particle gun
  particleGun = new G4ParticleGun();

  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(inputfile,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    if( *inputfile !=0)  //....only complain if a filename was given
      G4cout << "RMCCRYGenerator: Failed to open CRY input file= " << inputfile << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup=new CRYSetup(setupString,"/net/t2srv0008/app/d-Chooz/Software/cry_v1.6/data");

    gen = new CRYGenerator(setup);

    // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
  }
  // create a vector to store the CRY particle properties
  vect=new std::vector<CRYParticle*>;

  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

  // Create the messenger file
  gunMessenger = new RMCCRYGeneratorMessenger(this);
}

//----------------------------------------------------------------------------//
RMCCRYGenerator::~RMCCRYGenerator()
{
}

//----------------------------------------------------------------------------//
void RMCCRYGenerator::InputCRY()
{
  InputState=1;
}

//----------------------------------------------------------------------------//
void RMCCRYGenerator::UpdateCRY(std::string* MessInput)
{
  CRYSetup *setup=new CRYSetup(*MessInput,"/net/t2srv0008/app/d-Chooz/Software/cry_v1.6/data");

  gen = new CRYGenerator(setup);

  // set random number generator
  RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
  setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
  InputState=0;

}

//----------------------------------------------------------------------------//
void RMCCRYGenerator::CRYFromFile(G4String newValue)
{
  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(newValue,std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    G4cout << "Failed to open input file " << newValue << G4endl;
    G4cout << "Make sure to define the cry library on the command line" << G4endl;
    InputState=-1;
  }else{
    std::string setupString("");
    while ( !inputFile.getline(buffer,1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup=new CRYSetup(setupString,"/net/t2srv0008/app/d-Chooz/Software/cry_v1.6/data");

    gen = new CRYGenerator(setup);

  // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;
  }
}

//----------------------------------------------------------------------------//
void RMCCRYGenerator::GeneratePrimaries(G4Event* anEvent)
{ 
  if (InputState != 0) {
    G4String* str = new G4String("CRY library was not successfully initialized");
    G4Exception(*str);
  }
  G4String particleName;
  vect->clear();
  gen->genEvent(vect);

std::cout<<"The cosmic ray generation corresponds to an exposure of "<<gen->timeSimulated()<<" seconds"<<std::endl;

  //....debug output
  G4cout << "\nEvent=" << anEvent->GetEventID() << " "
	 << "CRY generated nparticles=" << vect->size()
	 << G4endl;

  for ( unsigned j=0; j<vect->size(); j++) {
    particleName=CRYUtils::partName((*vect)[j]->id());

    //....debug output  
    cout << "  "          << particleName << " "
	 << "charge="      << (*vect)[j]->charge() << " "
	 << setprecision(4)
	 << "energy (MeV)=" << (*vect)[j]->ke()*MeV << " "
	 << "pos (m)"         << G4ThreeVector((*vect)[j]->x()+3.5, (*vect)[j]->y(), (*vect)[j]->z()+10.) << " "
	 << "direction cosines " << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())<< " "
	 << endl;
    	  		
    	  		particleGun->SetParticleDefinition(particleTable->FindParticle((*vect)[j]->PDGid()));
particleGun->SetParticleEnergy((*vect)[j]->ke()*MeV);
particleGun->SetParticlePosition(G4ThreeVector(((*vect)[j]->x()+3.5)*m, (*vect)[j]->y()*m, ((*vect)[j]->z()+10.)*m));//plus 10 meters in Z corresponds to the surface of the ground, the Z coordinate origin for CRY
particleGun->SetParticleMomentumDirection(G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w()));

//for testing purposes:
// particleGun->SetParticlePosition(G4ThreeVector(4.5*m, 0.*m, 1.8*m));
// particleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1));

particleGun->SetParticleTime((*vect)[j]->t());
particleGun->GeneratePrimaryVertex(anEvent);
    delete (*vect)[j];
  }
}
