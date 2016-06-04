#ifndef detectorconstruction_hh
#define detectorconstruction_hh 1

#include "globals.hh" 
#include "G4VUserDetectorConstruction.hh"

class G4Box;
class G4Tubs;
class G4Sphere;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4TwoVector.hh"

#include <set>

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	~DetectorConstruction();

	static DetectorConstruction* Instance();

public:
    // Public functions
	G4VPhysicalVolume* Construct();

    inline const G4double& GetWorldDimensions() const { return world_dim; }
    inline const G4ThreeVector& GetTargetPosition() const { return detector_pos; }
    inline const G4ThreeVector& GetTargetDimension() const { return detector_dim; }

protected:
    // Protected functions
    virtual G4VPhysicalVolume* ConstructWorld();
    virtual void ConstructMaterials();

protected:
    // Protected variables
    G4VPhysicalVolume* worldPhys;
    // world material
    G4Material* mworld;
    // backplane detector material
    G4Material* mdetector;

    // world is a sphere so we need radius
    G4double world_dim;
    // backplane detector is a box, so we need x, y , z
    G4ThreeVector detector_dim;
    // backplane detector position
    G4ThreeVector detector_pos;
    G4RotationMatrix detector_rot;
    

    std::set<G4VPhysicalVolume*> scoring_volumes;
    std::vector<std::pair<std::string, G4ThreeVector> > detector_locations;


private:
	static DetectorConstruction* fgInstance;



};

#endif
