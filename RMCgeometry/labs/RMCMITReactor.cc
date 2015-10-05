////////////////////////////////////////////////////////////////////////
//                                                                    //
//  File:        RMCMITReactor.cc                                     //
//  Description: MIT reactor environment                              //
//                                                                    //
//  Author:      Adam Anderson (MIT)                                  //
//  Date:        28 Adam Anderson 2011                                //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include "RMCgeometry/labs/RMCMITReactor.hh"
#include "RMCgeometry/labs/RMCMITReactorMessenger.hh"
#include "RMCg4base/RMCMaterialTable.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"


RMCMITReactor::RMCMITReactor()
: RMCVLabConstruction("MITReactor"),
domeRadius(10.*m), domeHeight(10.*m), domeThickness(1.0*cm),
coreRadius(2.*m), coreHeight(5.*m),
roomXDim(2.8*m), roomYDim(2.8*m), roomZDim(2.8*m), roomWallThickness(121.92*cm),
theMessenger(new RMCMITReactorMessenger(this) )
{}

RMCMITReactor::~RMCMITReactor() {}


G4LogicalVolume* RMCMITReactor::BuildGeometry()
{
    G4cout << "RMCMITReactor::BuildGeometry()" << G4endl;
    
    // Get the necessary materials
    G4Material* iron = RMCMaterialTable::GetMaterial("G4_Fe");
    G4Material* air = RMCMaterialTable::GetMaterial("G4_AIR");
    G4Material* concrete = RMCMaterialTable::GetMaterial("G4_CONCRETE");
    G4Material* HDconcrete = new G4Material("HDconcrete", 4.0*g/cm3, 10);
    HDconcrete->AddMaterial(concrete, 1.0);
    
    
    // Build the reactor core
    G4Tubs* reactorCoreCyl = new G4Tubs("reactorCoreCyl", 0., coreRadius, coreHeight/2., 0.*deg, 360.*deg);
    G4LogicalVolume* reactorCoreLog = new G4LogicalVolume(reactorCoreCyl, concrete, "reactorCoreLog");
    G4VisAttributes* reactorCoreAttributes = new G4VisAttributes(G4Color(0.0,1.0,1.0));
    //G4VisAttributes* reactorCoreAttributes = new G4VisAttributes(G4VisAttributes::GetInvisible());
    reactorCoreAttributes->SetForceSolid(true);
    reactorCoreLog->SetVisAttributes(reactorCoreAttributes);
    
    
    // Build the fission converter room
    G4Box* fissionConverterCeiling = new G4Box("fissionConverterWall", (roomXDim/2.0)+roomWallThickness,
                                            (roomYDim/2.0)+roomWallThickness, roomWallThickness/2.);
    G4Box* fissionConverterWallNorth = new G4Box("fissionConverterWall", (roomXDim/2.0),
                                            roomWallThickness/2., (roomZDim/2.0));
    G4Box* fissionConverterWallEast = new G4Box("fissionConverterWall", roomWallThickness/2., 
                                            (roomXDim/2.0)+roomWallThickness, (roomZDim/2.0));
    G4Box* fissionConverterWallSouth = new G4Box("fissionConverterWall", (roomXDim/2.0),
                                            roomWallThickness/2., (roomZDim/2.0));
	G4Box* fissionConverterWallWest = new G4Box("fissionConverterWall", roomWallThickness/2., 
                                            (roomXDim/2.0)+roomWallThickness, (roomZDim/2.0));
    
    G4LogicalVolume* fissionConverterCeilingLog = new G4LogicalVolume(fissionConverterCeiling, concrete,
                                                                   "fissionConverterRoomLog");
    G4LogicalVolume* fissionConverterWallNorthLog = new G4LogicalVolume(fissionConverterWallNorth, concrete,
                                                                   "fissionConverterRoomLog");
    G4LogicalVolume* fissionConverterWallEastLog = new G4LogicalVolume(fissionConverterWallEast, concrete,
                                                                   "fissionConverterRoomLog");
    G4LogicalVolume* fissionConverterWallSouthLog = new G4LogicalVolume(fissionConverterWallSouth, concrete,
                                                                   "fissionConverterRoomLog");
    G4LogicalVolume* fissionConverterWallWestLog = new G4LogicalVolume(fissionConverterWallWest, concrete,
                                                                   "fissionConverterRoomLog");
    G4VisAttributes* fissionConverterCeilingAtt = new G4VisAttributes(G4Color(0.0, 0.0, 1.0));
    G4VisAttributes* fissionConverterNorthAtt = new G4VisAttributes(G4Color(0.0, 1.0, 0.0));
    G4VisAttributes* fissionConverterEastAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0));
    G4VisAttributes* fissionConverterSouthAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0));
    G4VisAttributes* fissionConverterWestAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0));
    //G4VisAttributes* fissionConverterRoomAttributes = new G4VisAttributes(G4VisAttributes::GetInvisible());
    fissionConverterCeilingAtt->SetForceSolid(true);
    fissionConverterNorthAtt->SetForceSolid(true);
    fissionConverterEastAtt->SetForceSolid(true);
    fissionConverterSouthAtt->SetForceSolid(true);
    fissionConverterWestAtt->SetForceSolid(true);
    fissionConverterCeilingLog->SetVisAttributes(fissionConverterCeilingAtt);
    fissionConverterWallNorthLog->SetVisAttributes(fissionConverterNorthAtt);
    fissionConverterWallEastLog->SetVisAttributes(fissionConverterEastAtt);
    fissionConverterWallSouthLog->SetVisAttributes(fissionConverterSouthAtt);
    fissionConverterWallWestLog->SetVisAttributes(fissionConverterWestAtt);
    
    
    // Build the reactor dome
    G4Tubs* reactorDomeCyl = new G4Tubs("reactorDomeCyl", domeRadius, domeRadius+domeThickness,
                                        domeHeight/2., 0.*deg, 360.*deg);
    G4LogicalVolume* reactorDomeLog = new G4LogicalVolume(reactorDomeCyl, iron, "reactorDomeLog");
    G4VisAttributes* domeAttributes = new G4VisAttributes(G4Color(0.0,1.0,1.0));
    //G4VisAttributes* domeAttributes = new G4VisAttributes(G4VisAttributes::GetInvisible());
    domeAttributes->SetForceSolid(true);
    reactorDomeLog->SetVisAttributes(domeAttributes);
    
    
    // Build dome ceiling
    G4Tubs* reactorDomeCeiling = new G4Tubs("reactorDomeCeiling", 0., domeRadius+domeThickness,
                                            domeThickness/2., 0.*deg, 360.*deg);
    G4LogicalVolume* reactorDomeCeilingLog = new G4LogicalVolume(reactorDomeCeiling, iron, "RMCMITReactorDomeCeiling");
    reactorDomeCeilingLog->SetVisAttributes(domeAttributes);
    
    
    // Build the sky volume.  
    G4double skyRadius = sqrt(pow(domeRadius,2) + pow(domeHeight,2));
    G4Sphere* skyHemisphere = new G4Sphere("SkyHemisphere", 0.0, skyRadius, 0.0, 360.0*deg, 0.0, 90.0*deg);
    G4LogicalVolume* skyHemisphereLog = new G4LogicalVolume(skyHemisphere, air, GetName());
    G4VisAttributes* skyAttributes = new G4VisAttributes(G4Color(1., 0., 0.));
    //G4VisAttributes* skyAttributes = new G4VisAttributes(G4VisAttributes::GetInvisible());
    skyAttributes->SetForceWireframe(true);
    skyHemisphereLog->SetVisAttributes(skyAttributes);
    
    
    // Insert dome and ceiling in the sky volume
    new G4PVPlacement(0, G4ThreeVector(0., 0., domeHeight/2.), reactorDomeLog,
                      "reactorDomePlace", skyHemisphereLog, false, 0);
    new G4PVPlacement(0, G4ThreeVector(0., 0., domeHeight+(domeThickness/2.)), reactorDomeCeilingLog,
                      "reactorDomeCeilingPlace", skyHemisphereLog, false, 0);
    new G4PVPlacement(0, G4ThreeVector(0., 0., coreHeight/2.), reactorCoreLog,
                      "rectorCorePlace", skyHemisphereLog, false, 0);
    new G4PVPlacement(0, G4ThreeVector(coreRadius+roomWallThickness+(roomXDim/2.0), 0., roomZDim+(roomWallThickness/2.0)),
                      fissionConverterCeilingLog, "fissionConverterRoomPlaceCeiling", skyHemisphereLog, false, 0);
    new G4PVPlacement(0, G4ThreeVector(coreRadius+(roomXDim/2.0)+roomWallThickness, (roomYDim/2.0)+(roomWallThickness/2), (roomZDim/2.0)),
                      fissionConverterWallNorthLog, "fissionConverterRoomPlaceWallNorth", skyHemisphereLog, false, 0);
    new G4PVPlacement(0, G4ThreeVector(coreRadius+3.0*(roomWallThickness/2.0)+roomXDim, 0., (roomZDim/2.0)),
                      fissionConverterWallEastLog, "fissionConverterRoomPlaceWallEast", skyHemisphereLog, false, 0);
    new G4PVPlacement(0, G4ThreeVector(coreRadius+(roomXDim/2.0)+roomWallThickness, -(roomYDim/2.0)-(roomWallThickness/2), (roomZDim/2.0)),
                      fissionConverterWallSouthLog, "fissionConverterRoomPlaceWallSouth", skyHemisphereLog, false, 0);
    new G4PVPlacement(0, G4ThreeVector(coreRadius+(roomWallThickness/2.0), -(roomYDim/2.0)+(roomWallThickness/2), (roomZDim/2.0)),
                      fissionConverterWallWestLog, "fissionConverterRoomPlaceWallWest", skyHemisphereLog, false, 0);
    
    return skyHemisphereLog;
}
