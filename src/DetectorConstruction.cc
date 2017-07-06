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
   masic(0),
   mcopperasic(0),
   mmotherb(0),
   mcoppermother(0),
   mdethousing(0),
   mbox(0),
   mcomp1(0),
   mtable(0),
   mwall(0),
   world_dim(10*m),                                               // default world is a 10 m radius sphere
   detector_dim(G4ThreeVector(0.5*cm, 0.5*cm, 0.5*cm)),           // default detector is 1 cc cube (use half sizes)
   detector_pos(G4ThreeVector(0.)),                               // default position at 0, 0, 0

   // Immediate environment
   asic_dim(G4ThreeVector(0.6*cm, 0.6*cm, 0.04*cm)),
   asic_pos(G4ThreeVector(0.*cm, 0.*cm, -0.84*cm)),
   copperasic_dim(G4ThreeVector(0.6*cm, 0.6*cm, 0.00175*cm)),
   copperasic_pos(G4ThreeVector(0.*cm, 0.*cm, -0.83825*cm)),
   motherb_dim(G4ThreeVector(0.6*cm, 0.6*cm, 0.04*cm)),
   motherb_pos(G4ThreeVector(0.*cm, 0.*cm, -1.14*cm)),
   coppermother_dim(G4ThreeVector(0.6*cm, 0.6*cm, 0.00175*cm)),
   coppermother_pos(G4ThreeVector(0.*cm, 0.*cm, -1.13825*cm)),
   dethousing_dim(G4ThreeVector(0.7*cm, 0.7*cm, 0.9*cm)),
   dethousing_pos(G4ThreeVector(0.*cm, 0.*cm, -0.2*cm)),

   // Greater environment
   boxBot_dim(G4ThreeVector(12.*cm, 8.*cm, 3.35*cm)),                  // box with dimensions 10cm by 5cm by 2 cm
   boxBot_pos(G4ThreeVector(7.936*cm, 3.428*cm, 0.*cm)),                  // box positioned at 3cm along x-axis
   boxTop_dim(G4ThreeVector(12.*cm, 8.*cm, 1.65*cm)),
   boxTop_pos(G4ThreeVector(7.936*cm, 3.428*cm, 5.*cm)),
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

    // NIST
    G4NistManager* nist = G4NistManager::Instance();

    // Environment materials

    G4Element* H  = new G4Element("Hydrogen", "H", 1., 1.01*g/mole);
    G4Element* C  = new G4Element("Carbon","C", 6., 12.01*g/mole);
    G4Element* O  = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
    G4Element* Si = new G4Element("Silicon", "Si", 14., 28.09*g/mole);

    G4Material* Epoxy = new G4Material("Epoxy", 1.2*g/cm3, 2);
    Epoxy->AddElement(H, 2);
    Epoxy->AddElement(C, 2);

    G4Material* SiO2 = new G4Material("quartz", 2.200*g/cm3, 2);
    SiO2->AddElement(Si, 1);
    SiO2->AddElement(O , 2);

    // FR4 (Glass & Epoxy)
    G4Material* FR4 = new G4Material("FR4", 1.86*g/cm3, 2);
    FR4->AddMaterial(SiO2, 0.528);
    FR4->AddMaterial(Epoxy, 0.472);

    masic = FR4;
    mmotherb = FR4;

    G4Material* Cu = new G4Material("Copper", 29., 63.546*g/mole, 8.96*g/cm3);

    mcopperasic = Cu;
    mcoppermother = Cu;

    // Lexan for detector housing
    mdethousing = nist->FindOrBuildMaterial("G4_POLYCARBOANTE");

    G4Material* Al = new G4Material("Aluminum", 13., 26.982*g/mole, 2.70*g/cm3);

    mbox = Al;
    mtable = Al;
    mcomp1 = Al;

    mwall = nist->FindOrBuildMaterial("G4_CONCRETE");

    // Fill the world with air
    //mworld = nist->FindOrBuildMaterial("G4_Galactic");
    mworld = nist->FindOrBuildMaterial("G4_AIR");
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
    // Create immediate environment
    // ---------------------------------------

    // ASIC
    G4VSolid* asicSolid = new G4Box("asicSolid",
                                    asic_dim.x(),
                                    asic_dim.y(),
                                    asic_dim.z()
                                    );

    // Create LV for asic
    G4LogicalVolume* asicLogVol = new G4LogicalVolume(asicSolid,
                                                      masic,
                                                      "asicLogVol"
                                                      );

    G4PVPlacement *pasic = new G4PVPlacement(0,
                                             G4ThreeVector(asic_pos.x(), asic_pos.y(), asic_pos.z()),
                                             "asic",
                                             asicLogVol,
                                             worldPhys,
                                             false,
                                             0
                                             );

    // Motherboard
    G4VSolid* motherbSolid = new G4Box("motherbSolid",
                                    motherb_dim.x(),
                                    motherb_dim.y(),
                                    motherb_dim.z()
                                    );

    // Create LV for motherboard
    G4LogicalVolume* motherbLogVol = new G4LogicalVolume(motherbSolid,
                                                         mmotherb,
                                                         "motherbLogVol"
                                                         );

    G4PVPlacement *pmb = new G4PVPlacement(0,
                                           G4ThreeVector(motherb_pos.x(), motherb_pos.y(), motherb_pos.z()),
                                           "motherb",
                                           motherbLogVol,
                                           worldPhys,
                                           false,
                                           0
                                           );

    // ---------------------------------------
    // Create greater environment
    // ---------------------------------------

    // Hollow box bottom
    G4Box* outerBoxBot = new G4Box("outerBoxBot",
                                boxBot_dim.x(),
                                boxBot_dim.y(),
                                boxBot_dim.z()
                                );

    G4Box* innerBoxBot = new G4Box("innerBoxBot",
                                boxBot_dim.x() - 0.65*cm,
                                boxBot_dim.y() - 0.65*cm,
                                boxBot_dim.z() - 0.65*cm
                                );

    G4SubtractionSolid* boxBotHollow = new G4SubtractionSolid("boxBotHollow",
                                                           outerBoxBot,
                                                           innerBoxBot
                                                           );

    G4Box* boxLidRemBot = new G4Box("boxLidRem",
                                 boxBot_dim.x() - 0.65*cm,
                                 boxBot_dim.y() - 0.65*cm,
                                 0.65*cm
                                 );

    G4SubtractionSolid* boxHollowBotNoLid = new G4SubtractionSolid("boxBotHollowNoLid",
                                                              boxBotHollow,
                                                              boxLidRemBot,
                                                              false,
                                                              G4ThreeVector(0.*cm, 0.*cm, 3.35*cm)
                                                              );

    // Create LV for box
    G4LogicalVolume* boxLogBot = new G4LogicalVolume(boxHollowBotNoLid,
                                                     mbox,
                                                     "boxBotLog"
                                                     );

    G4PVPlacement *pbot = new G4PVPlacement(0,
                                          G4ThreeVector(boxBot_pos.x(), boxBot_pos.y(), boxBot_pos.z()),
                                          "boxBot",
                                          boxLogBot,
                                          worldPhys,
                                          false,
                                          0
                                          );

    // Hollow box top
    G4Box* outerBoxTop = new G4Box("outerBoxBot",
                                   boxTop_dim.x(),
                                   boxTop_dim.y(),
                                   boxTop_dim.z()
                                   );

    G4Box* innerBoxTop = new G4Box("innerBoxTop",
                                   boxTop_dim.x() - 0.8*cm,
                                   boxTop_dim.y() - 0.8*cm,
                                   boxTop_dim.z() - 0.8*cm
                                   );

    G4SubtractionSolid* boxTopHollow = new G4SubtractionSolid("boxTopHollow",
                                                              outerBoxTop,
                                                              innerBoxTop
                                                              );

    G4Box* boxLidRemTop = new G4Box("boxLidRem",
                                    boxTop_dim.x() - 0.8*cm,
                                    boxTop_dim.y() - 0.8*cm,
                                    0.8*cm
                                    );

    G4SubtractionSolid* boxHollowTopNoLid = new G4SubtractionSolid("boxTopHollowNoLid",
                                                              boxTopHollow,
                                                              boxLidRemTop,
                                                              false,
                                                              G4ThreeVector(0.*cm, 0.*cm, -1.65*cm)
                                                              );

    // Create LV for box
    G4LogicalVolume* boxLogTop = new G4LogicalVolume(boxHollowTopNoLid,
                                                     mbox,
                                                     "boxTopLog"
                                                     );

    G4PVPlacement *ptop = new G4PVPlacement(0,
                                          G4ThreeVector(boxTop_pos.x(), boxTop_pos.y(), boxTop_pos.z()),
                                          "boxTop",
                                          boxLogTop,
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
    G4LogicalVolume* comp1LogVol = new G4LogicalVolume(comp1Solid,
                                                       mcomp1,
                                                       "comp1LogVol"
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
    G4LogicalVolume* tableLogVol = new G4LogicalVolume(tableSolid,
                                                       mtable,
                                                       "tableLog"
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
    G4LogicalVolume* wallLogVol = new G4LogicalVolume(wallSolid,
                                                       mwall,
                                                       "wallLog"
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

    G4VisAttributes* asic_vis_att = new G4VisAttributes(G4Color(100., 0., 100., 0.30));
    asic_vis_att->SetForceSolid(true);
    asic_vis_att->SetVisibility(true);
    asicLogVol -> SetVisAttributes(asic_vis_att);

    G4VisAttributes* motherb_vis_att = new G4VisAttributes(G4Color(100., 0., 100., 0.30));
    motherb_vis_att->SetForceSolid(true);
    motherb_vis_att->SetVisibility(true);
    motherbLogVol -> SetVisAttributes(motherb_vis_att);

    G4VisAttributes* boxBot_vis_att = new G4VisAttributes(G4Color(200., 200., 0., 0.10));
    boxBot_vis_att->SetForceSolid(true);
    boxBot_vis_att->SetVisibility(true);
    boxLogBot -> SetVisAttributes(boxBot_vis_att);

    G4VisAttributes* boxTop_vis_att = new G4VisAttributes(G4Color(200., 200., 0., 0.10));
    boxTop_vis_att->SetForceSolid(true);
    boxTop_vis_att->SetVisibility(true);
    boxLogTop -> SetVisAttributes(boxTop_vis_att);

    G4VisAttributes* comp1_vis_att = new G4VisAttributes(G4Color(0., 0., 25., 0.30));
    comp1_vis_att->SetForceSolid(true);
    comp1_vis_att->SetVisibility(true);
    comp1LogVol -> SetVisAttributes(comp1_vis_att);

    G4VisAttributes* table_vis_att = new G4VisAttributes(G4Color(25., 1., 0., 0.10));
    table_vis_att->SetForceSolid(true);
    table_vis_att->SetVisibility(true);
    tableLogVol -> SetVisAttributes(table_vis_att);

    // G4VisAttributes* wall_vis_att = new G4VisAttributes(G4Color(50., 1., 1., 0.10));
    // wall_vis_att->SetForceSolid(true);
    // wall_vis_att->SetVisibility(true);
    // wallLogVol -> SetVisAttributes(wall_vis_att);

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
