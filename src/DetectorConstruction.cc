#include "DetectorConstruction.hh"

#include "G4SDManager.hh"
#include "SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
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
   mbox(0),
   mcomp1(0),
   mtable(0),
   mwall(0),
   world_dim(10*m),                                               // default world is a 10 m radius sphere
   detector_dim(G4ThreeVector(0.5*cm, 0.5*cm, 0.5*cm)),           // default detector is 1 cc cube (use half sizes)
   detector_pos(G4ThreeVector(0.)),                               // default position at 0, 0, 0
   box_dim(G4ThreeVector(10.*cm, 5.*cm, 2.*cm)),                  // box with dimensions 10cm by 5cm by 2 cm
   box_pos(G4ThreeVector(4.*cm, 1.5*cm, 0.*cm)),                  // box positioned at 3cm along x-axis
   comp1_dim(G4ThreeVector(4.*cm, 2.*cm, 0.5*cm)),                // component1 in box with dimensions 4cm by 2cm by 0.5cm
   comp1_pos(G4ThreeVector(6.*cm, 0.*cm, -0.5*cm)),               // component1 in box positioned at -4cm along x-axis and -0.5cm along z-axis
   tableTop_dim(G4ThreeVector(60.*cm, 25.*cm, 3.*cm)),            // tabletop with dimensions 60cm by 0.5m by 3cm
   tableTop_pos(G4ThreeVector(0.*cm, 0.*cm, -5*cm)),              // tabletop positioned at -5cm along z-axis
   wall_dim(G4ThreeVector(80.*cm, 10.*cm, 200.*cm)),              // wall with dimensions 80cm by 10cm by 200cm
   wall_pos(G4ThreeVector(0.*cm, -200*cm, 0.*cm))                 // wall positioned at -35cm along the y-axis
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

void DetectorConstruction::ConstructMaterials(){
    // Creating CZT detector element;
    G4Element* elCd = new G4Element("Cadmium"  ,"Cd", 48., 112.411*g/mole);
    G4Element* elZn = new G4Element("Zinc"     ,"Zn", 30., 65.39  *g/mole);
    G4Element* elTe = new G4Element("Tellurium","Te", 52., 127.6  *g/mole);

    G4Material* CZT = new G4Material("CZT", 5.78*g/cm3, 3);
    CZT->AddElement(elCd, 9);
    CZT->AddElement(elZn, 1);
    CZT->AddElement(elTe, 10);

    // Assign to detector material
    mdetector = CZT;

    G4NistManager* nist = G4NistManager::Instance();

    // Environment materials
    G4Material* Al = new G4Material("Aluminum", 13., 26.982*g/mole, 2.70*g/cm3);

    mbox = Al;
    mtable = Al;
    mcomp1 = Al;

    mwall = nist->FindOrBuildMaterial("G4_CONCRETE");

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

    G4String centfilename = "geo/centervertices_";
    centfilename.append("1Det").append(".txt");
    ifstream centerfile(centfilename);
    double x_, y_, z_;
    string line;
    centers.clear();
    if (centerfile.is_open()){
        while (getline(centerfile,line)){
            istringstream(line) >> x_ >> y_ >> z_;
            centers.push_back(G4ThreeVector(x_*mm, y_*mm ,z_*mm));
        }
        centerfile.close();
    }

    // Pull in rotation matrices from file
    G4String rotfilename = "geo/rotationmatrices_";
    rotfilename.append("1Det").append(".txt");
    ifstream myfile3(rotfilename);
    double x1_,x2_,x3_,y1_,y2_,y3_,z1_,z2_,z3_;
    rotationmat.clear();
    if (myfile3.is_open()){
      while (getline(myfile3,line)){
        istringstream(line) >> x1_ >> x2_ >> x3_ >> y1_ >> y2_ >> y3_ >> z1_ >> z2_ >> z3_;
        rotationmat.push_back(G4RotationMatrix(G4ThreeVector(x1_,x2_,x3_), G4ThreeVector(y1_,y2_,y3_), G4ThreeVector(z1_,z2_,z3_)));
      }
      myfile3.close();
    }



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
    // Create detector
    // ---------------------------------------

    G4VSolid* detectorSolid = new G4Box("DetectorSolid",    // name
                                      detector_dim.x(),     // x half-length
                                      detector_dim.y(),     // y half-length
                                      detector_dim.z()      // z half-length
                                      );

    // Create the logical volume for detector
    G4LogicalVolume* logvol = new G4LogicalVolume(detectorSolid,      // target solid
                                                    mdetector,        // target material
                                                    "DetectorLog"     // name
                                                    );

    detector_pos = centers[1];
    G4Transform3D transform = G4Transform3D(rotationmat[1],detector_pos);

    G4PVPlacement *p = new G4PVPlacement(transform,               // the placement of the volume in center
                                         1,                       // name
                                         logvol,                  // the corresponding logical volume -- gives
                                                                  //   volume the material (and the dimensions
                                                                  //   via the solid assigned to the logical volume)
                                         worldPhys,               // inside the world so the world is the mother
                                                                  //   physical volume,
                                         false,                   // many = false (no copies)
                                         0                        // the replica id number (only > 0 if copies exist)
                                         );

    // ---------------------------------------
    // Create environment
    // ---------------------------------------

    // Hollow box
    G4Box* outerBox = new G4Box("Outer Box",
                                box_dim.x(),
                                box_dim.y(),
                                box_dim.z()
                                );

    G4Box* innerBox = new G4Box("Inner Box",
                                box_dim.x() - 0.25*cm,
                                box_dim.y() - 0.25*cm,
                                box_dim.z() - 0.25*cm
                                );

    G4SubtractionSolid* boxHollow = new G4SubtractionSolid("boxHollow",
                                                           outerBox,
                                                           innerBox
                                                           );

    // Create LV for box
    G4LogicalVolume* boxLog = new G4LogicalVolume(boxHollow,      // target solid
                                                     mbox,        // target material
                                                     "boxLog"     // name
                                                     );

    G4PVPlacement *pb = new G4PVPlacement(0,
                                          G4ThreeVector(box_pos.x(), box_pos.y(), box_pos.z()),
                                          "box",
                                          boxLog,
                                          worldPhys,
                                          false,
                                          0
                                          );

    // Component inside box
    G4VSolid* comp1Solid = new G4Box("comp1Solid",
                                     comp1_dim.x(),
                                     comp1_dim.y(),
                                     comp1_dim.z()
                                     );

    // Create LV for component1
    G4LogicalVolume* comp1LogVol = new G4LogicalVolume(comp1Solid,    // target solid
                                                       mcomp1,        // target material
                                                       "comp1Log"     // name
                                                       );

    G4PVPlacement *pc1 = new G4PVPlacement(0,
                                           G4ThreeVector(comp1_pos.x(), comp1_pos.y(), comp1_pos.z()),
                                           "comp1",
                                           comp1LogVol,
                                           worldPhys,
                                           false,
                                           0
                                           );

    // Tabletop
    G4VSolid* tableSolid = new G4Box("tableSolid",
                                     tableTop_dim.x(),
                                     tableTop_dim.y(),
                                     tableTop_dim.z()
                                     );

    // Create LV for tabletop
    G4LogicalVolume* tableLogVol = new G4LogicalVolume(tableSolid,    // target solid
                                                       mtable,        // target material
                                                       "tableLog"     // name
                                                       );

    G4PVPlacement *ptt = new G4PVPlacement(0,
                                           G4ThreeVector(tableTop_pos.x(), tableTop_pos.y(), tableTop_pos.z()),
                                           "tableTop",
                                           tableLogVol,
                                           worldPhys,
                                           false,
                                           0
                                           );

    // Wall
    G4VSolid* wallSolid = new G4Box("wallSolid",
                                     wall_dim.x(),
                                     wall_dim.y(),
                                     wall_dim.z()
                                     );

    // Create LV for wall
    G4LogicalVolume* wallLogVol = new G4LogicalVolume(wallSolid,    // target solid
                                                       mwall,        // target material
                                                       "wallLog"     // name
                                                       );

    G4PVPlacement *pw = new G4PVPlacement(0,
                                           G4ThreeVector(wall_pos.x(), wall_pos.y(), wall_pos.z()),
                                           "wall",
                                           wallLogVol,
                                           worldPhys,
                                           false,
                                           0
                                           );


    // ---------------------------------------
    // Set up visualization of geometry
    // ---------------------------------------

    G4VisAttributes* detector_vis_att = new G4VisAttributes(G4Color(0.,1.,56.,1.));
    detector_vis_att->SetForceSolid(true);
    detector_vis_att->SetVisibility(true);
    logvol -> SetVisAttributes(detector_vis_att);

    // G4VisAttributes* box_vis_att = new G4VisAttributes(G4Color(0., 0., 0., 0.));
    // box_vis_att->SetForceSolid(true);
    // box_vis_att->SetVisibility(true);
    // boxLog -> SetVisAttributes(box_vis_att);

    G4VisAttributes* comp1_vis_att = new G4VisAttributes(G4Color(0., 0., 25., 0.30));
    comp1_vis_att->SetForceSolid(true);
    comp1_vis_att->SetVisibility(true);
    comp1LogVol -> SetVisAttributes(comp1_vis_att);

    G4VisAttributes* table_vis_att = new G4VisAttributes(G4Color(25., 1., 0., 0.10));
    table_vis_att->SetForceSolid(true);
    table_vis_att->SetVisibility(true);
    tableLogVol -> SetVisAttributes(table_vis_att);

    G4VisAttributes* wall_vis_att = new G4VisAttributes(G4Color(50., 1., 1., 0.10));
    wall_vis_att->SetForceSolid(true);
    wall_vis_att->SetVisibility(true);
    wallLogVol -> SetVisAttributes(wall_vis_att);

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
