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
class G4Sphere;
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
		G4Material* mbox;
		G4Material* mcomp1;
		G4Material* mtable;
		G4Material* mwall;

    G4double world_dim;          // World is a sphere so we need radius
    G4ThreeVector detector_dim;  // detector are cubes, need xyz dimensions
    G4ThreeVector detector_pos;
		G4RotationMatrix detector_rot;

		// Lab environment
		G4ThreeVector asic_dim;
		G4ThreeVector asic_pos;

		G4ThreeVector copperasic_dim;
		G4ThreeVector copperasic_pos;

		G4ThreeVector motherb_dim;
		G4ThreeVector motherb_pos;

		G4ThreeVector coppermother_dim;
		G4ThreeVector coppermother_pos;

		G4ThreeVector dethousing_dim;
		G4ThreeVector dethousing_pos;

		G4ThreeVector boxBot_dim;
		G4ThreeVector boxBot_pos;

		G4ThreeVector boxTop_dim;
		G4ThreeVector boxTop_pos;

		G4ThreeVector comp1_dim;
		G4ThreeVector comp1_pos;

		G4ThreeVector tableTop_dim;
		G4ThreeVector tableTop_pos;

		G4ThreeVector wall_dim;
		G4ThreeVector wall_pos;

private:
	static DetectorConstruction* fgInstance;

};

#endif
