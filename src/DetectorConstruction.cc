#include "DetectorConstruction.hh"

#include "G4SDManager.hh"
#include "SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Orb.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UnitsTable.hh"
#include "G4Material.hh"

using namespace std;
using namespace CLHEP;

 // Solids ////////////////////////////////////////////////////////////////////

 DetectorConstruction* DetectorConstruction::fgInstance = 0;

 //==================================================================================================

 DetectorConstruction* DetectorConstruction::Instance() {

 	return fgInstance;
 }

 DetectorConstruction::DetectorConstruction()
 : worldPhys(0),
   mworld(0),
   mdetector(0),
   world_dim(10*m),                                                      // default world is a 10 m radius sphere
   detector_dim(G4ThreeVector(0.5*cm, 0.5*cm, 0.5*cm)),                  // default detector is 1 cc cube (use half sizes)
   detector_pos(G4ThreeVector(0.))                                      // default position at 0, 0, 0
{
   // Create a new messenger class
    detectorconstructionmessenger = new DetectorConstructionMessenger(this);

	fgInstance = this;
}

//==================================================================================================

DetectorConstruction::~DetectorConstruction() {

    // Delete messenger class
    delete detectorconstructionmessenger;

	fgInstance = 0;
}

//==================================================================================================

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    ConstructMaterials();
    return ConstructWorld();
}

// Specifying the Material ////////////////////////////////////////////////////

// Creating CZT detector element;
void DetectorConstruction::ConstructMaterials(){
    G4Element* elCd = new G4Element("Cadmium"  ,"Cd", 48., 112.411*g/mole);
    G4Element* elZn = new G4Element("Zinc"     ,"Zn", 30., 65.39  *g/mole);
    G4Element* elTe = new G4Element("Tellurium","Te", 52., 127.6  *g/mole);

    G4Material* CZT = new G4Material("CZT", 5.78*g/cm3, 3);
    CZT->AddElement(elCd, 9);
    CZT->AddElement(elZn, 1);
    CZT->AddElement(elTe, 10);

    // Assign to detector material
    mdetector = CZT;

    // the NIST manager has simple materials
    G4NistManager* nist = G4NistManager::Instance();

    // Make the world a vacuum
    mworld = nist->FindOrBuildMaterial("G4_Galactic");
    //mworld = nist->FindOrBuildMaterial("G4_AIR");
}

G4VPhysicalVolume* DetectorConstruction::ConstructWorld() {


    // ---------------------------------------
    // Create world volume
    // ---------------------------------------

    G4VSolid* worldSolid = new G4Orb("World",  // name
                                     world_dim
                                     );

    G4LogicalVolume* worldLog = new G4LogicalVolume(worldSolid, // corresponding solid volume
                                                    mworld,     // material
                                                    "World"     // name
                                                    );

    worldPhys = new G4PVPlacement(0,                    // no rotations so give it a null pointer
                                  G4ThreeVector(0.),    // the placement of the volume at (0, 0, 0)
                                  "World",              // name
                                  worldLog,             // the corresponding logical volume -- gives
                                                        //   volume the material (and the dimensions
                                                        //   via the solid assigned to the logical volume)
                                  0,                    // the mother physical volume (the world doesn't
                                                        //   have a mother so pass it a null pointer
                                  false,                // many = false (no copies)
                                  0                     // the replica id number (only > 0 if copies exist)
                                  );


    // ---------------------------------------
    // Create detectors
    // ---------------------------------------

    G4VSolid* detectorSolid = new G4Box("DetectorSolid",    // name
                                      detector_dim.x(),     // x half-length
                                      detector_dim.y(),     // y half-length
                                      detector_dim.z()      // z half-length
                                      );

    // Create 192 logical volumes (so we can have different colors)
    G4LogicalVolume* logvol = new G4LogicalVolume(detectorSolid,    // target solid
                                                    mdetector,        // target material
                                                    "DetectorLog"     // name
                                                    );

    G4RotationMatrix* rotD3 = new G4RotationMatrix();
    rotD3 -> rotateX(0.*deg);

    G4PVPlacement *p = new G4PVPlacement(rotD3,               // the placement of the volume in center
                                         G4ThreeVector(),                   // name
                                         logvol,              // the corresponding logical volume -- gives
                                                                  //   volume the material (and the dimensions
                                                                  //   via the solid assigned to the logical volume)
                                         "det1",
                                         worldLog,               // inside the world so the world is the mother
                                                                  //   physical volume,
                                         false,                   // many = false (no copies)
                                         0                        // the replica id number (only > 0 if copies exist)
                                         );
    // ---------------------------------------
    // Set up visualization of geometry
    // ---------------------------------------

    G4VisAttributes* detector_vis_att = new G4VisAttributes(G4Color(0.,1.,56.,1.));
    detector_vis_att->SetForceSolid(true);
    detector_vis_att->SetVisibility(true);
    logvol -> SetVisAttributes(detector_vis_att);

    G4VisAttributes* world_vis_att = new G4VisAttributes(G4Color(1.,0.,0.));
    world_vis_att->SetForceWireframe(true);
    world_vis_att->SetVisibility(true);
    worldLog->SetVisAttributes(world_vis_att);

    return worldPhys;
}

void DetectorConstruction::ConstructSDandField(){

    // Sensitive detectors
    G4String SDname = "PRISM_SIM/SD";
    SensitiveDetector * SD = new SensitiveDetector(SDname,"fHitsCollection");
    SetSensitiveDetector("DetectorLog", SD, true);
}

void DetectorConstruction::UpdateGeometry(){

    G4RunManager::GetRunManager()->ReinitializeGeometry(true);   // pass it "true" so it clear the geometry first...
}

//==================================================================================================

void DetectorConstruction::SetDetDim(G4ThreeVector dim){

    detector_dim = G4ThreeVector((dim.x()/2.)*cm, (dim.y()/2.)*cm, (dim.z()/2.)*cm);
}
