#ifndef detectorconstruction_hh
#define detectorconstruction_hh

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "DetectorConstructionMessenger.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4TwoVector.hh"

using namespace std;

class G4Box;
class G4Tubs;
class G4Torus;
class G4Sphere;
class G4SubtractionSolid;
class G4UnionSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	~DetectorConstruction();

	static DetectorConstruction* Instance();

    DetectorConstructionMessenger * detectorconstructionmessenger;


public:
	G4VPhysicalVolume* Construct();

    inline const G4double& GetWorldDimensions() const { return world_dim; }
    inline const G4ThreeVector& GetTargetPosition() const { return detector_pos; }
    inline const G4ThreeVector& GetTargetDimension() const { return detector_dim; }

    virtual void ConstructSDandField();

    void UpdateGeometry();
    void SetDetDim(G4ThreeVector);

    vector<G4ThreeVector> centers;
    inline vector<G4ThreeVector> GetDetCenters(){return centers;}

		vector<G4RotationMatrix> rotationmat;
		inline vector<G4RotationMatrix> GetRotationMat(){return rotationmat;}


		G4String detindexing;
		inline void SetDetIndexing(G4String di){detindexing = di;}
		inline G4String GetDetIndexing(){return detindexing;}

protected:
    virtual G4VPhysicalVolume* ConstructWorld();
    virtual void ConstructMaterials();

protected:
    G4VPhysicalVolume* worldPhys;
    G4Material* mworld;
    G4Material* mdetector;
		G4Material* masic;
		G4Material* mcopperasic;
		G4Material* mmotherb;
		G4Material* mcoppermother;
		G4Material* mdethousing;
		G4Material* musbinsul;
		G4Material* mwireinsul;
		G4Material* mwireconn;
		G4Material* mwirewire;
		G4Material* mbox;
		G4Material* mthread;
		G4Material* mboxnubs;
		G4Material* mtable;
		G4Material* mwall;

    G4double world_dim;          // World is a sphere so we need radius

    G4ThreeVector detector_dim;  // detector are cubes, need xyz dimensions
    G4ThreeVector detector_pos;
		G4RotationMatrix detector_rot;

		// ---------------------------------------
    // Immediate environment
    // ---------------------------------------

		G4ThreeVector asic_dim;
		G4ThreeVector asic_pos;

		G4ThreeVector copperasic_dim;
		G4ThreeVector copperasic_pos;

		G4ThreeVector motherb_dim;
		G4ThreeVector motherb_pos;

		G4ThreeVector motherbsec2_dim;

		G4ThreeVector motherbsec3_dim;

		G4ThreeVector coppermother_dim;
		G4ThreeVector coppermother_pos;

		G4ThreeVector dethousing_dim;
		G4ThreeVector dethousing_pos;

		// For series of shapes to build usb wire-hoop
    G4ThreeVector usbHoopTorusWest_dim;
		G4ThreeVector usbHoopTorusWest_pos;
		G4ThreeVector usbHoopStraightNorth_dim;
		G4ThreeVector usbHoopStraightNorth_pos;
		G4ThreeVector usbHoopTorusEast_dim;
		G4ThreeVector usbHoopTorusEast_pos;
		G4ThreeVector usbHoopStraightSouth_dim;
		G4ThreeVector usbHoopStraightSouth_pos;

		// For wiring on the inside of the hoop
		G4ThreeVector usbHoopTorusWireWest_dim;
		G4ThreeVector usbHoopStraightWireNorth_dim;
		G4ThreeVector usbHoopTorusWireEast_dim;
		G4ThreeVector usbHoopStraightWireSouth_dim;

		G4ThreeVector usbConn_dim;

		// ---------------------------------------
    // Greater environment
    // ---------------------------------------

		G4ThreeVector boxBot_dim;
		G4ThreeVector boxBot_pos;

		G4ThreeVector boxTop_dim;
		G4ThreeVector boxTop_pos;

		G4ThreeVector threadHole1_dim;
		G4ThreeVector threadHole1_pos;

		G4ThreeVector threadHole2_dim;

		G4ThreeVector threadHole3_dim;
		G4ThreeVector threadHole3_pos;

		G4ThreeVector threadHole4_dim;

		G4ThreeVector threadHole5_dim;

		G4ThreeVector threadHole6_dim;

		G4ThreeVector boxNub1_dim;
		G4ThreeVector boxNub1_pos;

		G4ThreeVector boxNub2_dim;
	  G4ThreeVector boxNub2_pos;

		G4ThreeVector tableTop_dim;
		G4ThreeVector tableTop_pos;

		G4ThreeVector wall_dim;
		G4ThreeVector wall_pos;

private:
	static DetectorConstruction* fgInstance;

};

#endif
