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

    inline const G4ThreeVector& GetWorldDimensions() const { return world_dim; }
    inline const G4ThreeVector& GetTargetPosition() const { return backplane_pos; }
    inline const G4ThreeVector& GetTargetDimension() const { return backplane_dim; }

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
    G4Material* mbackplane;
    // mask material
    G4Material* mmask;

    // world is a box so we need X, Y, Z
    G4ThreeVector world_dim;
    // backplane detector is a box, so we need x, y , z
    G4ThreeVector backplane_dim;
    // backplane detector position
    G4ThreeVector backplane_pos;
    
    // mask detector is a box, so we need x, y , z
    G4ThreeVector mask_dim;
    // mask detector position
    G4ThreeVector mask_pos;

    std::set<G4VPhysicalVolume*> scoring_volumes;
    std::vector<std::pair<std::string, G4ThreeVector> > backplane_locations;


private:
	static DetectorConstruction* fgInstance;



};

#endif
