#include "DetectorConstruction.hh"

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


//==================================================================================================

DetectorConstruction* DetectorConstruction::fgInstance = 0;

//==================================================================================================

DetectorConstruction* DetectorConstruction::Instance() {
    
	return fgInstance;
}

//==================================================================================================

DetectorConstruction::DetectorConstruction()
: worldPhys(0), mworld(0), mdetector(0),
  world_dim(0.5*CLHEP::m),                                                     // default world is a 0.5 m radius sphere
  detector_dim(G4ThreeVector(1.0*CLHEP::cm, 1.0*CLHEP::cm, 1.0*CLHEP::cm)),    // default detector is 1 cc cube
  detector_pos(G4ThreeVector(0.))                                              // default position at 0, 0, 0
{
	fgInstance = this;
}

//==================================================================================================

DetectorConstruction::~DetectorConstruction() {
    
	fgInstance = 0;
}

//==================================================================================================

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    ConstructMaterials();
    return ConstructWorld();
}

//==================================================================================================

void DetectorConstruction::ConstructMaterials() {
    
    // Define CZT material
    G4Element* elCd = new G4Element("Cadmium"  ,"Cd", 48., 112.411*CLHEP::g/CLHEP::mole);
    G4Element* elZn = new G4Element("Zinc"     ,"Zn", 30., 65.39  *CLHEP::g/CLHEP::mole);
    G4Element* elTe = new G4Element("Tellurium","Te", 52., 127.6  *CLHEP::g/CLHEP::mole);
    
    G4Material* CZT = new G4Material("CZT", 5.78*CLHEP::g/CLHEP::cm3, 3);
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

//==================================================================================================

G4VPhysicalVolume* DetectorConstruction::ConstructWorld() {
    
    world_dim    = 0.5*CLHEP::m;                                                  // set a world volume sphere with 0.5m radius
    detector_dim = G4ThreeVector(0.5*CLHEP::cm, 0.5*CLHEP::cm, 0.5*CLHEP::cm);    // detector detectors are 1 cm cubes
    
    
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
    std::vector<G4LogicalVolume*> logvols;
    for (int i = 0; i < 192; i++){
        logvols.push_back(new G4LogicalVolume(detectorSolid,    // target solid
                                              mdetector,        // target material
                                              "DetectorLog"     // name
                                              ));
    }
    
   
    // Pull in detector center verticies from file
    std::vector<G4ThreeVector> centers;
    std::ifstream myfile("geo/centervertices.txt");
    double x_, y_, z_;
    std::string line;
    if (myfile.is_open()){
        while (getline(myfile,line)){
            std::istringstream(line) >> x_ >> y_ >> z_;
            centers.push_back(G4ThreeVector(x_*CLHEP::mm, y_*CLHEP::mm ,z_*CLHEP::mm));
        }
        myfile.close();
    }
    
/*
    std::vector<G4ThreeVector> rotations;
    std::ifstream myfile2("../geo/rotationangles.txt");
    if (myfile2.is_open()){
        while (getline(myfile2,line)){
            std::istringstream(line) >> x_ >> y_ >> z_;
            rotations.push_back(G4ThreeVector(x_, y_ ,z_));
        }
        myfile2.close();
    }
 
    detector_rot = G4RotationMatrix();
    detector_rot.rotateX(rotations[i].x());
    detector_rot.rotateY(rotations[i].y());
    detector_rot.rotateZ(rotations[i].z());
*/
    
    // Pull in rotation matrices from file
    std::vector<G4RotationMatrix> rotationmat;
    std::ifstream myfile3("geo/rotationmatrices.txt");
    double x1_,x2_,x3_,y1_,y2_,y3_,z1_,z2_,z3_;
    if (myfile3.is_open()){
        while (getline(myfile3,line)){
            std::istringstream(line) >> x1_ >> x2_ >> x3_ >> y1_ >> y2_ >> y3_ >> z1_ >> z2_ >> z3_;
            rotationmat.push_back(G4RotationMatrix(G4ThreeVector(x1_,x2_,x3_), G4ThreeVector(y1_,y2_,y3_), G4ThreeVector(z1_,z2_,z3_)));
        }
        myfile3.close();
    }
    
    
    // Place 192 detectors
    for (int i = 0; i < 192; i++){
        std::string name = std::to_string(i+1);
        
        detector_pos = centers[i];
        G4Transform3D transform = G4Transform3D(rotationmat[i],detector_pos);
        
        new G4PVPlacement(transform,                // the placement of the volume in center
                          name,                     // name
                          logvols[i],              // the corresponding logical volume -- gives
                                                   //   volume the material (and the dimensions
                                                   //   via the solid assigned to the logical volume)
                          worldPhys,               // inside the world so the world is the mother
                                                   //   physical volume,
                          false,                   // many = false (no copies)
                          0                        // the replica id number (only > 0 if copies exist)
                          );
    }


    
    // ---------------------------------------
    // Set up visualization of geometry
    // ---------------------------------------
    
    for (int i = 0; i < 192; i++){
        G4VisAttributes* detector_vis_att = new G4VisAttributes(G4Color(0.,0.5,(i+1)/192.,1.0));
        detector_vis_att->SetForceSolid(true);
        detector_vis_att->SetVisibility(true);
        logvols[i]->SetVisAttributes(detector_vis_att);
    }


    
    
    return worldPhys;
}

//==================================================================================================


