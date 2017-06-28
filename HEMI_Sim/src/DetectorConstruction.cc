#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
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


//==================================================================================================

DetectorConstruction* DetectorConstruction::fgInstance = 0;

//==================================================================================================

DetectorConstruction* DetectorConstruction::Instance()
{
	return fgInstance;
}

//==================================================================================================

DetectorConstruction::DetectorConstruction()
: worldPhys(0), mworld(0), mbackplane(0),
  world_dim(G4ThreeVector(0.5*CLHEP::m, 0.5*CLHEP::m, 0.5*CLHEP::m)),          // default world is 1 m cube
  backplane_dim(G4ThreeVector(1.0*CLHEP::cm, 1.0*CLHEP::cm, 1.0*CLHEP::cm)),   // default backplane is 2 cm cube
  backplane_pos(G4ThreeVector(0.)),                                            // default position at 0, 0, 0
  mask_dim(G4ThreeVector(1.0*CLHEP::cm, 1.0*CLHEP::cm, 1.0*CLHEP::cm)),        // default mask is 2 cm cube
  mask_pos(G4ThreeVector(0.))                                                  // default position at 0, 0, 0
{
	fgInstance = this;
}

//==================================================================================================

DetectorConstruction::~DetectorConstruction()
{
	fgInstance = 0;
}

//==================================================================================================

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    ConstructMaterials();
    return ConstructWorld();
}

//==================================================================================================

void DetectorConstruction::ConstructMaterials()
{

    // Define CZT material
    G4Element* elCd = new G4Element("Cadmium"  ,"Cd", 48., 112.411*CLHEP::g/CLHEP::mole);
    G4Element* elZn = new G4Element("Zinc"     ,"Zn", 30., 65.39  *CLHEP::g/CLHEP::mole);
    G4Element* elTe = new G4Element("Tellurium","Te", 52., 127.6  *CLHEP::g/CLHEP::mole);
    
    G4Material* CZT = new G4Material("CZT", 5.78*CLHEP::g/CLHEP::cm3, 3);
    CZT->AddElement(elCd, 9);
    CZT->AddElement(elZn, 1);
    CZT->AddElement(elTe, 10);
    
    // Assign to detector material (mask and backplane)
    mbackplane = CZT;
    mmask = CZT;
    
    // the NIST manager has simple materials
    G4NistManager* nist = G4NistManager::Instance();
    
    // Make the world a vacuum
    mworld = nist->FindOrBuildMaterial("G4_Galactic");
    //mworld = nist->FindOrBuildMaterial("G4_AIR");
    
    
}

//==================================================================================================

G4VPhysicalVolume* DetectorConstruction::ConstructWorld()
{
    
    
    world_dim     = G4ThreeVector(0.3*CLHEP::m,  0.3*CLHEP::m,  0.3*CLHEP::m);     // set a world volume cube with 0.2x0.2x0.5
    backplane_dim = G4ThreeVector(0.5*CLHEP::cm, 0.5*CLHEP::cm, 0.5*CLHEP::cm);    // backplane detectors are 1 cm cubes
    mask_dim      = G4ThreeVector(0.5*CLHEP::cm, 0.5*CLHEP::cm, 0.5*CLHEP::cm);    // mask detectors are 1x1x1 cm cubes
    
    
    // ---------------------------------------
    // Create world volume
    // ---------------------------------------
    
    G4VSolid* worldSolid = new G4Box("World",  // name
                                     world_dim.x(), // x half-length
                                     world_dim.y(), // y half-length
                                     world_dim.z()  // z half-length
                                     );
    G4LogicalVolume* worldLog = new G4LogicalVolume(worldSolid, // corresponding solid volume
                                                    mworld,     // material
                                                    "World"     // name
                                                    );

    G4RotationMatrix* rotation = 0;
    worldPhys = new G4PVPlacement(rotation,             // no rotations so give it a null pointer
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
    // Create back plane detector, will then place it multiple times
    // ---------------------------------------
    
    G4VSolid* backplaneSolid = new G4Box("BackPlane",  // name
                                      backplane_dim.x(), // x half-length
                                      backplane_dim.y(), // y half-length
                                      backplane_dim.z()  // z half-length
                                      );

    G4LogicalVolume* backplaneLog = new G4LogicalVolume(backplaneSolid, // target solid
                                                        mbackplane,     // target material
                                                        "BackPlane"     // name
                                                        );

    
    // define backplane matrix
    // Need to update dead detectors???
    int bp_array[8][8] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}
    };
    
    
    std::string backplane_name;
    G4int count = 1;
    int xcount = 0;
    int ycount = 0;
    for (double x = -4.9; x < 4.9; x += 1.4){
        for (double y = -4.9; y < 4.9; y += 1.4){
        
            if(bp_array[ycount][xcount] == 1){
                
                // Define backplane detector positions from the loop variables and with z against back of world volume
                // shift x and y so everything is xcentered
                backplane_pos = G4ThreeVector(x*CLHEP::cm, y*CLHEP::cm,  -7.5*CLHEP::cm);
            
                backplane_name = "backplane[";
                backplane_name += std::to_string(int(x));
                backplane_name += ", ";
                backplane_name += std::to_string(int(y));
                backplane_name += "]";
            
                new G4PVPlacement(0,                           // no rotations so give it a null pointer
                                  backplane_pos,               // the placement of the volume in center
                                  "Backplane",                 // name
                                  backplaneLog,                // the corresponding logical volume -- gives
                                                                //   volume the material (and the dimensions
                                                                //   via the solid assigned to the logical volume)
                                  worldPhys,                   // inside the world so the world is the mother
                                                                //   physical volume,
                                  false,                       // many = false (no copies)
                                  count                          // the replica id number (only > 0 if copies exist)
                                  );
            
                count++;
                
            }
            
            ycount++;
        }
        
        ycount = 0;
        xcount++;
    }

    
    // ---------------------------------------
    // Create mask detector, will then place it multiple times
    // ---------------------------------------
    
    G4VSolid* maskSolid = new G4Box("Mask",       // name
                                    mask_dim.x(), // x half-length
                                    mask_dim.y(), // y half-length
                                    mask_dim.z()  // z half-length
                                    );
    
    G4LogicalVolume* maskLog = new G4LogicalVolume(maskSolid, // target solid
                                                   mmask,     // target material
                                                   "Mask"     // name
                                                   );
    
    // define mask matrix
    // Need to update mask???
    int mask_array[8][8] =
    {
        {1, 1, 0, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1, 0, 1},
        {1, 1, 0, 0, 1, 0, 1, 0},
        {0, 0, 1, 1, 1, 0, 1, 1},
        {1, 0, 1, 0, 1, 0, 1, 0},
        {1, 0, 0, 1, 0, 1, 0, 1},
        {1, 1, 0, 1, 0, 0, 1, 1},
        {0, 1, 1, 0, 1, 1, 0, 1}
    };
    

    
    std::string mask_name;
    //count = 0;
    xcount = 0;
    ycount = 0;
    for (double x = -4.9; x < 4.9; x += 1.4){
        for (double y = -4.9; y < 4.9; y += 1.4){
        
        //G4cout << xcount << "\t" << ycount  << "\t" << mask_array[ycount][xcount] << "\n";
        if(mask_array[ycount][xcount] == 1){
            
            // Define backplane detector positions from the loop variables and 7.5 cm shifted in z
            // shift x and y such that everything is centered
            mask_pos = G4ThreeVector(x*CLHEP::cm, y*CLHEP::cm, 0.0*CLHEP::m);
            
            mask_name = "mask[";
            mask_name += std::to_string(int(x));
            mask_name += ", ";
            mask_name += std::to_string(int(y));
            mask_name += "]";
            
            new G4PVPlacement(0,                           // no rotations so give it a null pointer
                              mask_pos,                    // the placement of the volume in center
                              "Mask",                      // name
                              maskLog,                     // the corresponding logical volume -- gives
                                                           //   volume the material (and the dimensions
                                                           //   via the solid assigned to the logical volume)
                              worldPhys,                   // inside the world so the world is the mother
                                                           //   physical volume,
                              false,                       // many = false (no copies)
                              count                        // the replica id number (only > 0 if copies exist)
                              );
            
            count++;
        }
            
            ycount++;
        }
        
        ycount = 0;
        xcount++;
    }
    
    
    
    // ---------------------------------------
    // Set up visualization of geometry
    // ---------------------------------------
    
    G4VisAttributes* backplane_vis_att = new G4VisAttributes(G4Color(1.,0.,0.,0.8));
    backplane_vis_att->SetForceSolid(true);
    backplane_vis_att->SetVisibility(true);
    backplaneLog->SetVisAttributes(backplane_vis_att);
    
    G4VisAttributes* mask_vis_att = new G4VisAttributes(G4Color(0.,1.,0., 0.5));
    mask_vis_att->SetForceSolid(true);
    mask_vis_att->SetVisibility(true);
    maskLog->SetVisAttributes(mask_vis_att);
    

    // because the mother volumes keep track of their daughters, we have a heirarchy
    // so the uppermost volume in the heirarchy (the world) is the only volume
    // we need to carry around
    
    // When Geant4 exits, it deletes the world volume and the world volume
    // deletes it's daughters, then those daughters delete their daughters and so on
    // so we don't ever need to call delete on any of the volumes (or materials)
    
    return worldPhys;

    
}

//==================================================================================================


