#ifndef detectorconstruction_hh
#define detectorconstruction_hh 1

#include "globals.hh" 
#include "G4VUserDetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "DetectorConstructionMessenger.hh"

class G4Box;
class G4Tubs;
class G4Sphere;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4TwoVector.hh"


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
    
    std::vector<G4int> GetRandomMask();
    void SetMask(std::vector<G4int>);
    G4String BinToHex(std::vector<G4int>);
    std::vector<G4int> HexToBin(G4String);
    
    void UpdateGeometry();
    void SetDetDim(G4ThreeVector);

    void CheckOverlapsOn();
    
    
protected:
    virtual G4VPhysicalVolume* ConstructWorld();
    virtual void ConstructMaterials();

protected:
    // Physical volume for world
    G4VPhysicalVolume* worldPhys;
    // world material
    G4Material* mworld;
    // backplane detector material
    G4Material* mdetector;

    // World is a sphere so we need radius
    G4double world_dim;
    // detector are cubes, need xyz
    G4ThreeVector detector_dim;
    // detector position
    G4ThreeVector detector_pos;
    // detector rotation
    G4RotationMatrix detector_rot;
    
    // bool to check overlapping geometry (can be time consuming... default is false)
    //   can be turned on with CheckOverlapsOn()
    bool _checkoverlaps;
    

private:
	static DetectorConstruction* fgInstance;
    
    std::vector<G4int> _mask;


};

#endif
