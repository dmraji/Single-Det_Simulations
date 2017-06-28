// Originally developed by Caleb Roecker at UCB (now at LANL)
//

#include "GeExampleDetectorConstruction.hh"
#include "GeExampleMaterials.hh"
#include "GeExampleMySD.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Ellipsoid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"
#include "G4UnionSolid.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
#include "G4VSDFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Transform3D.hh"
#include "G4UserLimits.hh"

#include "globals.hh"

#include <CLHEP/Units/PhysicalConstants.h>

#include <iostream>
#include <sstream>

using namespace std;

GeExampleDetectorConstruction::GeExampleDetectorConstruction()
{
	//initialization of several things

	//setup materials first, optical properties to be set after
	//geometry is constructed to support borders
	materials = new GeExampleMaterials(this);

    G10Box_phys = new vector<G4VPhysicalVolume*>;
    GeDet_phys = new vector<G4VPhysicalVolume*>;
    GeDet_Position = new vector<G4ThreeVector>;
    TopContacts = new vector<G4VPhysicalVolume*>;
    BottomContacts = new vector<G4VPhysicalVolume*>;
}

GeExampleDetectorConstruction::~GeExampleDetectorConstruction()
{
	delete materials;
    delete ColdFinger_Phys;
    delete G10Box_phys;
    delete GeDet_phys;
    delete TopContacts;
    delete BottomContacts;
}


G4VPhysicalVolume* GeExampleDetectorConstruction::Construct()
{
	return ConstructDetector();
}

G4VPhysicalVolume* GeExampleDetectorConstruction::ConstructDetector()
{
    G4RotationMatrix *rotZ90 = new G4RotationMatrix();
	rotZ90->rotateZ(90.*deg);
    G4RotationMatrix *rotX90 = new G4RotationMatrix();
	rotX90->rotateX(90.*deg);
    G4RotationMatrix *rotY90 = new G4RotationMatrix();
	rotY90->rotateY(90.*deg);

	//perform general cleaning of geometry
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	//double in = cm * 2.54;

	//define world volume full widths
	G4double worldX = 1.*m;
	G4double worldY = 1.*m;
	G4double worldZ = 1.*m;

	//The world volume, make of rock
	G4Box* world_solid = new G4Box("world_solid",worldX/2.0,worldY/2.0,worldZ/2.0);
	G4LogicalVolume* world_logic = new G4LogicalVolume(world_solid,materials->Air(), "world_logic");
	world_logic -> SetVisAttributes(materials->AirVis());
	G4VPhysicalVolume* world_phys = new G4PVPlacement(0,G4ThreeVector(),world_logic,"World",0,true,0);

    //Create the box to hold the detector
    G4double CaseX = 10.0*2.54*cm;
    G4double CaseY = 8.0*2.54*cm;
    G4double CaseZ = 12.0*cm+2.0*cm;
    G4Box *DetectorCase = new G4Box("DetectorCase", CaseX/2.0, CaseY/2.0, CaseZ/2.0);
    G4LogicalVolume *Case_Logic = new G4LogicalVolume(DetectorCase,materials->Aluminum(), "Case_Logic");
    Case_Logic->SetVisAttributes(materials->AluminumVis());
    G4VPhysicalVolume *Case_phys = new G4PVPlacement(rotZ90,G4ThreeVector(),"Case_phys",Case_Logic,world_phys,0,0,true);

    //Create the air inside of the box for the detector
    G4double AirCaseX = CaseX-0.5*cm;
    G4double AirCaseY = CaseY-0.5*cm;
    G4double AirCaseZ = CaseZ-0.5*cm;
    G4Box *CaseAirBox = new G4Box("CaseAirBox", AirCaseX/2.0, AirCaseY/2.0, AirCaseZ/2.0);
    G4LogicalVolume *CaseAir_Logic = new G4LogicalVolume(CaseAirBox, materials->Air(),"CaseAir_Logic");
    G4VPhysicalVolume *CaseAir_Phys = new G4PVPlacement(0,G4ThreeVector(),"CaseAir_Phys",CaseAir_Logic,Case_phys,0,0,true);

    //create a detector
    detectorZ = 15.0*mm;
    detectorY = 82.*mm;
    detectorX = 82.*mm;
    plasticBoxX = 100.0*mm;
    plasticBoxY = 100.0*mm;
    plasticBoxZ = GetDetectorZ();

    G4double DetXPosition = 0.75*2.54*cm;
    G4double DetYPosition = -0.5*2.54*cm;
    G4double DetZPosition = 0.5*cm + GetDetectorZ()/2.0;

    //Create the Ge detectors
    CreateGeDetector(0,G4ThreeVector(DetXPosition-(plasticBoxX-detectorX)/2.0,DetYPosition+(plasticBoxY-detectorY)/2.0,-(DetZPosition)-4.0*cm),CaseAir_Phys);
    CreateGeDetector(0,G4ThreeVector(DetXPosition-(plasticBoxX-detectorX)/2.0,DetYPosition+(plasticBoxY-detectorY)/2.0,DetZPosition-4.0*cm),CaseAir_Phys);

    //Create the baseplate assume copper 1cm thick and make the cold finger
    G4double ColdPlateX = 1.0*cm;
    G4double ColdPlateY = detectorY;
    G4double ColdPlateZ = 12.0*cm;//2.0*DetZPosition+GetDetectorZ();
    G4double ColdRodRadius = 2.0*cm;
    G4double ColdRodZ = AirCaseX/2.0-DetXPosition-GetDetectorX()/2.0-ColdPlateX;//4.0*cm;
    G4Box *ColdPlateSolid = new G4Box("ColdPlateSolid",ColdPlateZ/2.0,ColdPlateY/2.0,ColdPlateX/2.0);
    G4Tubs *ColdPlateRod = new G4Tubs("ColdPlateRod",0,ColdRodRadius,ColdRodZ/2.0,0*deg,360*deg);
    G4UnionSolid *ColdFinger = new G4UnionSolid("ColdFinger", ColdPlateRod,ColdPlateSolid, 0, G4ThreeVector(0,0,ColdPlateX/2.0+ColdRodZ/2.0));

    G4LogicalVolume *ColdFinger_Logic = new G4LogicalVolume(ColdFinger,materials->Copper(),"ColdFinger_Logic");
    ColdFinger_Phys = new G4PVPlacement(rotY90,G4ThreeVector(ColdPlateX+ColdRodZ/2.0+GetDetectorX()/2.0+DetXPosition,DetYPosition,0),"ColdFinger_Phys",ColdFinger_Logic,CaseAir_Phys,0,0,true);

	//Create the sensitive detector which is based off of the pmt logical
	//The below order is very picky and I would not change it. Even if the geant4
	//examples show something else in my experience it isn't guaranteed to work
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	SDman = G4SDManager::GetSDMpointer();
	G4String name="/GeExampleDet/pmtSD";
	pmtSD = new GeExampleMySD(name, this);
	SDman->AddNewDetector(pmtSD);
	GeDet_Logic->SetSensitiveDetector(pmtSD);

	//Apply filter to sensitive detector to only accept optical photons
	G4VSDFilter * ElectronFilter = new G4SDParticleFilter("ElectronFilter","e-");
	pmtSD->SetFilter(ElectronFilter);


	//Return the physical World
	return world_phys;
}

//----------------------------------------------------------------------------------------------------

void GeExampleDetectorConstruction::CreateGeDetector(G4RotationMatrix *detRot, const G4ThreeVector &detPos, G4VPhysicalVolume *parentVolume)
{
    //Create detector
    G4Tubs *DetEdges = new G4Tubs("DetEdges",47.*mm,130*mm,detectorZ,0*deg,360*deg);
    G4Box* GeDetBox = new G4Box("GeDetBox",detectorX/2.0,detectorY/2.0,detectorZ/2.0);
    G4SubtractionSolid *GeDetRoundedSides = new G4SubtractionSolid("GeDetRoundedSides",GeDetBox,DetEdges);
    G4UnionSolid* GeDet;

    //Create Detector readout
    G4Box *G10DetectorBox = new G4Box("G10DetectorBox",plasticBoxX/2.0,plasticBoxY/2.0,plasticBoxZ/2.0);
    G4Box *G10GeDetBox = new G4Box("G10GeDetBox",detectorX/2.0+1.0*mm,detectorY/2.0+1.0*mm,detectorZ/2.0+1.0*mm);
    G4SubtractionSolid *G10Box = new G4SubtractionSolid("G10Box",G10DetectorBox,G10GeDetBox,0,G4ThreeVector((plasticBoxX-GetDetectorX())/2.0+1.0*mm,-(plasticBoxY-GetDetectorY())/2.0-1.0*mm,0));
    G4LogicalVolume *G10Box_Logic = new G4LogicalVolume(G10Box,materials->G10(),"G10Box_Logic");
    stringstream *ss = new stringstream;
    *ss<<"G10Box_phys_"<<G10Box_phys->size();
    G10Box_phys->push_back(new G4PVPlacement( detRot,detPos, ss->str().c_str(),G10Box_Logic,parentVolume,  0, 0, true ));
    delete ss;

    //create the strips
    double stripZ=10.0*um;
    double stripY=77.*mm;
    double stripX=1.5*mm;
    double XStripPos=-detectorX/2.0+2*mm;

    G4Tubs *StripEdges = new G4Tubs("StripEdges",45.*mm,130*mm,stripZ,0*deg,360*deg);
    G4Box *SingleStripBox = new G4Box("GeStripBox",stripX/2.0,stripY/2.0,stripZ/2.0);
    G4Box *BottomSingleStripBox = new G4Box("BottomSingleStripBox",stripY/2.0,stripX/2.0,stripZ/2.0);
    G4SubtractionSolid *TopStrips[38];
    G4SubtractionSolid *BottomStrips[38];

    G4LogicalVolume* TopStrips_Logic[38];
    G4LogicalVolume* BottomStrips_Logic[38];

    stringstream *ss1 = new stringstream;
    stringstream *ss2 = new stringstream;
    stringstream *TopStripsName = new stringstream;
    stringstream *BottomStripsName = new stringstream;

    //increase top sides
    for(int i=0;i<38;i++)
    {
        XStripPos+=2.*mm;
        //create the names
        *ss1<<"TopStrips_"<<i;
        *ss2<<"BottomStrips_"<<i;

        //create the create strips for each position
        TopStrips[i] = new G4SubtractionSolid(ss1->str().c_str(),SingleStripBox,StripEdges,0,G4ThreeVector(-XStripPos,0,0));
        BottomStrips[i] = new G4SubtractionSolid(ss2->str().c_str(),BottomSingleStripBox,StripEdges,0,G4ThreeVector(0,-XStripPos,0));
        if(i==0)
        {
            GeDet = new G4UnionSolid("TopSide", GeDetRoundedSides, TopStrips[i], 0, G4ThreeVector(XStripPos,0,detectorZ/2.0+stripZ/2.0));
        }
        else
        {
            GeDet = new G4UnionSolid("TopSide", GeDet, TopStrips[i], 0, G4ThreeVector(XStripPos,0,detectorZ/2.0+stripZ/2.0));
        }
        GeDet = new G4UnionSolid("BottomSide", GeDet, BottomStrips[i], 0, G4ThreeVector(0,XStripPos,-detectorZ/2.0-stripZ/2.0));
        ss1->str("");
        ss2->str("");
    }

    GeDet_Logic = new G4LogicalVolume(GeDet,materials->Germanium(), "GeDet_Logic");
	GeDet_Logic->SetVisAttributes(materials->LeadVis());
    stringstream *detectorName = new stringstream;
	*detectorName << "GeDetector_"<< GeDet_phys->size();
	GeDet_phys->push_back(new G4PVPlacement(0,G4ThreeVector(detPos.x()+(plasticBoxX-GetDetectorX())/2.0,detPos.y()+-(plasticBoxY-GetDetectorY())/2.0,detPos.z()),detectorName->str().c_str(),GeDet_Logic,parentVolume,0,0,true));
    GeDet_Position->push_back(G4ThreeVector(detPos.x()+(plasticBoxX-GetDetectorX())/2.0,detPos.y()+-(plasticBoxY-GetDetectorY())/2.0,detPos.z()));
    delete detectorName;


    XStripPos=-detectorX/2.0+2*mm;
    //place strips
    for(int i=0;i<38;i++)
    {
        XStripPos+=2.*mm;
        //create the names
        *ss1<<"TopStrips_"<<i;
        *ss2<<"BottomStrips_"<<i;
        *TopStripsName<<"TopContact_"<<TopContacts->size();
        *BottomStripsName<<"BottomContact_"<<BottomContacts->size();

        //set the logical volume for each strip
        TopStrips_Logic[i]= new G4LogicalVolume(TopStrips[i],materials->Silicon(), (string("Logic_")+ss1->str()).c_str()/*"TopStrips_Logic"*/);
        TopStrips_Logic[i]->SetVisAttributes(materials->SteelVis());
        BottomStrips_Logic[i]= new G4LogicalVolume(BottomStrips[i],materials->Silicon(),  (string("Logic_")+ss2->str()).c_str()/*"BottomStrips_Logic"*/);
        BottomStrips_Logic[i]->SetVisAttributes(materials->SteelVis());

        //create the placement holding the volume
        TopContacts->push_back(new G4PVPlacement( 0,G4ThreeVector(XStripPos,0,detectorZ/2.0+stripZ/2.0), TopStrips_Logic[i], TopStripsName->str().c_str(),GeDet_Logic,  0, 0, true ));
        BottomContacts->push_back(new G4PVPlacement( 0,G4ThreeVector(0,XStripPos,-detectorZ/2.0-stripZ/2.0), BottomStrips_Logic[i], BottomStripsName->str().c_str(),GeDet_Logic,  0, 0, true ));
        ss1->str("");
        ss2->str("");
        TopStripsName->str("");
        BottomStripsName->str("");
    }


}


//----------------------------------------------------------------------------------------------------
#include "G4RunManager.hh"
void GeExampleDetectorConstruction::UpdateGeometry()
{
	G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());
}
