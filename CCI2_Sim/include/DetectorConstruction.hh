//	GeExampleEventAction.hh


#ifndef GeExampleDetectorConstruction_h
#define GeExampleDetectorConstruction_h 1

#include "GeExampleMySD.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4OpticalSurface.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

#include "globals.hh"

#include <vector>

class G4Box;
class G4Tubs;
class G4Ellipsoid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class GeExampleMaterials;

class GeExampleDetectorConstruction : public G4VUserDetectorConstruction
{	
public:

	//Constructor/Destructor
	GeExampleDetectorConstruction();
	~GeExampleDetectorConstruction();
	void UpdateGeometry();

	//File's accesible functions
	G4VPhysicalVolume* Construct();
	G4double GetDetectorX() {return detectorX; };
	G4double GetDetectorY() {return detectorY; };
	G4double GetDetectorZ() {return detectorZ; };
	
	//Get references to volumes
	vector<G4VPhysicalVolume*> *GetDetPhys() {return GeDet_phys; };
	vector<G4ThreeVector> *GetDetPos() {return GeDet_Position; };
    
private:
 
    //declare the function to create the detectors
    void CreateGeDetector(G4RotationMatrix *detRot, const G4ThreeVector &detPos, G4VPhysicalVolume *parentVolume);
    
    //declare the pointer to the materials
    GeExampleMaterials *materials;
	
	//Define the physical volume of the world/lead
    vector<G4VPhysicalVolume*> *G10Box_phys;
	vector<G4VPhysicalVolume*> *GeDet_phys;
	vector<G4ThreeVector> *GeDet_Position;
    vector<G4VPhysicalVolume*> *TopContacts;
    vector<G4VPhysicalVolume*> *BottomContacts;
    G4VPhysicalVolume *ColdFinger_Phys;
    
    //Define the logical volume for germanium
    G4LogicalVolume* GeDet_Logic;
    
	//Define standard size and counters
	G4double detectorX;
	G4double detectorY;
	G4double detectorZ;
    G4double plasticBoxX;
    G4double plasticBoxY;
    G4double plasticBoxZ;
    

	//Define the constuction and sensitive detector
	G4VPhysicalVolume* ConstructDetector();	 
	GeExampleMySD * pmtSD;
    
public:
    GeExampleMaterials* GetMaterials() {return materials; };

};

#endif

