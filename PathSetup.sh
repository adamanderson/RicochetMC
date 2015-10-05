export G4WORKDIR=$PWD
export G4INSTALL=/net/t2srv0008/app/d-Chooz/Software/geant4/test/geant4.9.4.p03/
export ROOTSYS=/net/t2srv0008/app/d-Chooz/Software/root/root
export G4LEVELGAMMADATA=$G4INSTALL
export G4LEDATA=$G4INSTALL/data/G4EMLOW6.2
export LD_LIBRARY_PATH=/app/d-Chooz/Software/CLHEP/CLHEP2042/lib:/app/d-Chooz/Software/root/root/lib:/usr/lib:/app/d-Chooz/Software/geant4/test/geant4.9.4.p03/extralibs:$LD_LIBRARY_PATH
export G4INCLUDE=$G4INSTALL/include 
. RMCbuild/g4setup.sh
./RMCbuild/setup
export G4NEUTRONHPDATA=$G4INSTALL/data/G4NDL3.14
export G4RADIOACTIVEDATA=$G4INSTALL/data/RadioactiveDecay3.2
export G4ABLADATA=$G4INSTALL/data/G4ABLA3.0
export G4LIB=$G4INSTALL/lib
export CRYHOME=/net/t2srv0008/app/d-Chooz/Software/cry_v1.6