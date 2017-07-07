#include "DetectorConstruction.hh"

#include "G4SDManager.hh"
#include "SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
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
   mthread(0),
   mboxnubs(0),
   mtable(0),
   mwall(0),
   world_dim(10*m),                                               // default world is a 10 m radius sphere
   detector_dim(G4ThreeVector(0.5*cm, 0.5*cm, 0.5*cm)),           // default detector is 1 cc cube (use half sizes)
   detector_pos(G4ThreeVector(0.)),                               // default position at 0, 0, 0

   // ---------------------------------------
   // Immediate environment
   // ---------------------------------------

   // ASIC board and copper film
   asic_dim(G4ThreeVector(0.6*cm, 0.6*cm, 0.04*cm)),
   asic_pos(G4ThreeVector(0.*cm, 0.*cm, -0.84*cm)),

   copperasic_dim(G4ThreeVector(0.6*cm, 0.6*cm, 0.00175*cm)),
   copperasic_pos(G4ThreeVector(0.*cm, 0.*cm, -0.83825*cm)),

   // Motherboard and copper film
   motherb_dim(G4ThreeVector(1.6*cm, 0.6*cm, 0.04*cm)),
   motherb_pos(G4ThreeVector(-0.4*cm, 0.*cm, -1.14*cm)),
   motherbsec2_dim(G4ThreeVector(3.5*cm, 2.1*cm, 0.04*cm)),
   motherbsec3_dim(G4ThreeVector(0.25*cm, 0.8*cm, 0.04*cm)),

   coppermother_dim(G4ThreeVector(0.6*cm, 0.6*cm, 0.00175*cm)),
   coppermother_pos(G4ThreeVector(0.*cm, 0.*cm, -1.13825*cm)),

   // Lexan housing of detector
   dethousing_dim(G4ThreeVector(0.7*cm, 0.7*cm, 0.9*cm)),
   dethousing_pos(G4ThreeVector(0.*cm, 0.*cm, -0.2*cm)),

   // USB cord components


   // ---------------------------------------
   // Greater environment
   // ---------------------------------------

   // Box geometry
   boxBot_dim(G4ThreeVector(12.*cm, 8.*cm, 3.35*cm)),        // box bottom with dimensions 12cm by 8cm by 3.35cm
   boxBot_pos(G4ThreeVector(7.936*cm, 3.428*cm, 0.*cm)),     // box bottom positioned at 7.936cm along x-axis and 3.428cm along y-axis

   boxTop_dim(G4ThreeVector(12.*cm, 8.*cm, 1.65*cm)),        // box top with dimensions 12cm by 8cm by 1.65cm
   boxTop_pos(G4ThreeVector(7.936*cm, 3.428*cm, 5.*cm)),     // box bottom positions at 7.936cm along x-axis, 3.428cm along y-axis, 5cm along z-axis

   // Thread holes in box
   threadHole1_dim(G4ThreeVector(0.33*cm, 0.39*cm, 1.0*cm)),
   threadHole1_pos(G4ThreeVector(19.036*cm, 10.528*cm, -2.35*cm)),

   threadHole2_dim(G4ThreeVector(0.37*cm, 0.39*cm, 2.35*cm)),

   threadHole3_dim(G4ThreeVector(0.37*cm, 0.39*cm, 1.35*cm)),
   threadHole3_pos(G4ThreeVector(19.036*cm, 10.528*cm, 4.85*cm)),

   threadHole4_dim(G4ThreeVector(0.33*cm, 0.39*cm, 0.15*cm)),

   threadHole5_dim(G4ThreeVector(0.36*cm, 0.39*cm, 0.15*cm)),

   // Aluminum nubs on base of box
   boxNub1_dim(G4ThreeVector(0.4*cm, 0.8*cm, 0.7*cm)),
   boxNub1_pos(G4ThreeVector(18.936*cm, 3.428*cm, -2.70*cm)),

   boxNub2_dim(G4ThreeVector(0.4*cm, 0.7*cm, 0.7*cm)),
   boxNub2_pos(G4ThreeVector(18.936*cm, 9.368*cm, -2.70*cm)),

   // Other environment components

   tableTop_dim(G4ThreeVector(60.*cm, 25.*cm, 3.*cm)),       // tabletop with dimensions 60cm by 25cm by 3cm

   tableTop_pos(G4ThreeVector(0.*cm, 0.*cm, -20*cm)),        // tabletop positioned at -5cm along z-axis

   wall_dim(G4ThreeVector(80.*cm, 10.*cm, 200.*cm)),         // wall with dimensions 80cm by 10cm by 200cm

   wall_pos(G4ThreeVector(0.*cm, -200*cm, 0.*cm))            // wall positioned at -35cm along the y-axis
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
    G4Element* Cd = new G4Element("Cadmium", "Cd", 48., 112.411*g/mole);
    G4Element* Zn = new G4Element("Zinc", "Zn", 30., 65.39*g/mole);
    G4Element* Te = new G4Element("Tellurium", "Te", 52., 127.6*g/mole);

    G4Material* CZT = new G4Material("CZT", 5.78*g/cm3, 3);
    CZT->AddElement(Cd, 9);
    CZT->AddElement(Zn, 1);
    CZT->AddElement(Te, 10);

    // Assign to detector material
    mdetector = CZT;

    // NIST
    G4NistManager* nist = G4NistManager::Instance();

    // Immediate environment materials

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

    // FR4 (Glass & Epoxy) for PCBs
    G4Material* FR4 = new G4Material("FR4", 1.86*g/cm3, 2);
    FR4->AddMaterial(SiO2, 0.528);
    FR4->AddMaterial(Epoxy, 0.472);

    masic = FR4;
    mmotherb = FR4;

    G4Material* Cu = new G4Material("Copper", 29., 63.546*g/mole, 8.96*g/cm3);

    mcopperasic = Cu;
    mcoppermother = Cu;

    // Lexan for detector housing
    mdethousing = nist->FindOrBuildMaterial("G4_POLYCARBONATE");

    // Materials for wiring
    musbinsul = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    mwireinsul = nist->FindOrBuildMaterial("G4_TEFLON");

    mwirewire = Cu;

    G4Material* brass = new G4Material("Brass", 8.40*g/cm3, 2);
    brass -> AddElement(Zn, 0.30);
    brass -> AddElement(Cu, 0.70);

    mwireconn = brass;

    // Greater environment materials

    G4Material* Al = new G4Material("Aluminum", 13., 26.982*g/mole, 2.70*g/cm3);

    mbox = Al;
    mthread = Al;
    mboxnubs = Al;
    mtable = Al;

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

    G4VSolid* motherbsec2Solid = new G4Box("motherbsec2Solid",
                                           motherbsec2_dim.x(),
                                           motherbsec2_dim.y(),
                                           motherbsec2_dim.z()
                                           );

    G4UnionSolid* motherbp1Solid = new G4UnionSolid("motherbp1Solid",
                                                    motherbSolid,
                                                    motherbsec2Solid,
                                                    0,
                                                    G4ThreeVector(5.1*cm, 0.*cm, 0.*cm)
                                                    );

    G4VSolid* motherbsec3Solid = new G4Box("motherbsec3Solid",
                                           motherbsec3_dim.x(),
                                           motherbsec3_dim.y(),
                                           motherbsec3_dim.z()
                                           );

    G4UnionSolid* motherbfullSolid = new G4UnionSolid("motherbp2Solid",
                                                      motherbp1Solid,
                                                      motherbsec3Solid,
                                                      0,
                                                      G4ThreeVector(8.85*cm, 0.*cm, 0.*cm)
                                                      );

    // Create LV for motherboard
    G4LogicalVolume* motherbLogVol = new G4LogicalVolume(motherbfullSolid,
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

    // Detector housing
    G4Box* outerdethousing = new G4Box("outerdethousing",
                                       dethousing_dim.x(),
                                       dethousing_dim.y(),
                                       dethousing_dim.z()
                                       );

    G4Box* innerdethousing = new G4Box("innerdethousing",
                                       dethousing_dim.x() - 0.1*cm,
                                       dethousing_dim.y() - 0.1*cm,
                                       dethousing_dim.z()
                                       );

    G4SubtractionSolid* dethousingHollow = new G4SubtractionSolid("dethousingHollow",
                                                                  outerdethousing,
                                                                  innerdethousing
                                                                  );

    // Create LV for detector housing
    G4LogicalVolume* dethousingLogVol = new G4LogicalVolume(dethousingHollow,
                                                            mdethousing,
                                                            "dethousingLogVol"
                                                            );

    G4PVPlacement *pdeths = new G4PVPlacement(0,
                                            G4ThreeVector(dethousing_pos.x(), dethousing_pos.y(), dethousing_pos.z()),
                                            "dethousing",
                                            dethousingLogVol,
                                            worldPhys,
                                            false,
                                            0
                                            );

    // ---------------------------------------
    // Create greater environment
    // ---------------------------------------

    // Thread holes in box
    G4Tubs* threadHole1 = new G4Tubs("threadHole1",
                                     threadHole1_dim.x(),
                                     threadHole1_dim.y(),
                                     threadHole1_dim.z(),
                                     0,
                                     360
                                     );

    G4Tubs* threadHole2 = new G4Tubs("threadHole2",
                                     threadHole2_dim.x(),
                                     threadHole2_dim.y(),
                                     threadHole2_dim.z(),
                                     0,
                                     360
                                     );

    G4UnionSolid* thread12 = new G4UnionSolid("thread12",
                                              threadHole1,
                                              threadHole2,
                                              0,
                                              G4ThreeVector(0.*cm, 0.*cm, 3.35*cm)
                                              );

    G4Tubs* threadHole3 = new G4Tubs("threadHole3",
                                     threadHole3_dim.x(),
                                     threadHole3_dim.y(),
                                     threadHole3_dim.z(),
                                     0,
                                     360
                                     );

    G4Tubs* threadHole4 = new G4Tubs("threadHole4",
                                     threadHole4_dim.x(),
                                     threadHole4_dim.y(),
                                     threadHole4_dim.z(),
                                     0,
                                     360
                                     );

    G4UnionSolid* thread34 = new G4UnionSolid("thread34",
                                              threadHole3,
                                              threadHole4,
                                              0,
                                              G4ThreeVector(0.*cm, 0.*cm, 1.5*cm)
                                              );

    G4Tubs* threadHole5 = new G4Tubs("threadHole5",
                                     threadHole5_dim.x(),
                                     threadHole5_dim.y(),
                                     threadHole5_dim.z(),
                                     0,
                                     360
                                     );

    G4UnionSolid* thread345 = new G4UnionSolid("thread345",
                                              thread34,
                                              threadHole5,
                                              0,
                                              G4ThreeVector(0.*cm, 0.*cm, 1.65*cm)
                                              );

    // Hollow box bottom
    G4Box* outerBoxBot = new G4Box("outerBoxBot",
                                   boxBot_dim.x(),
                                   boxBot_dim.y(),
                                   boxBot_dim.z()
                                   );

    G4Box* innerBoxBot = new G4Box("innerBoxBot",
                                   boxBot_dim.x() - 0.55*cm,
                                   boxBot_dim.y() - 0.55*cm,
                                   boxBot_dim.z() - 0.40*cm
                                   );

    G4SubtractionSolid* boxBotHollow = new G4SubtractionSolid("boxBotHollow",
                                                              outerBoxBot,
                                                              innerBoxBot
                                                              );

    G4Box* boxLidRemBot = new G4Box("boxLidRem",
                                    boxBot_dim.x() - 0.55*cm,
                                    boxBot_dim.y() - 0.55*cm,
                                    0.40*cm
                                    );

    G4SubtractionSolid* boxHollowBotNoLid = new G4SubtractionSolid("boxBotHollowNoLid",
                                                                   boxBotHollow,
                                                                   boxLidRemBot,
                                                                   false,
                                                                   G4ThreeVector(0.*cm, 0.*cm, 3.35*cm)
                                                                   );

    G4SubtractionSolid* boxHollowBotFinal = new G4SubtractionSolid("boxHollowBotFinal",
                                                                   boxHollowBotNoLid,
                                                                   thread12,
                                                                   false,
                                                                   G4ThreeVector(11.1*cm, 7.10*cm, 1.65*cm)
                                                                   );

    // Create LV for box bottom
    G4LogicalVolume* boxLogBot = new G4LogicalVolume(boxHollowBotFinal,
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
                                   boxTop_dim.x() - 0.80*cm,
                                   boxTop_dim.y() - 0.80*cm,
                                   boxTop_dim.z() - 0.40*cm
                                   );

    G4SubtractionSolid* boxTopHollow = new G4SubtractionSolid("boxTopHollow",
                                                              outerBoxTop,
                                                              innerBoxTop
                                                              );

    G4Box* boxLidRemTop = new G4Box("boxLidRem",
                                    boxTop_dim.x() - 0.80*cm,
                                    boxTop_dim.y() - 0.80*cm,
                                    0.40*cm
                                    );

    G4SubtractionSolid* boxHollowTopNoLid = new G4SubtractionSolid("boxTopHollowNoLid",
                                                                   boxTopHollow,
                                                                   boxLidRemTop,
                                                                   false,
                                                                   G4ThreeVector(0.*cm, 0.*cm, -1.65*cm)
                                                                   );

    G4SubtractionSolid* boxHollowTopFinal = new G4SubtractionSolid("boxHollowTopFinal",
                                                                   boxHollowTopNoLid,
                                                                   thread345,
                                                                   false,
                                                                   G4ThreeVector(11.1*cm, 7.10*cm, -1.65*cm)
                                                                   );

    // Create LV for box top
    G4LogicalVolume* boxLogTop = new G4LogicalVolume(boxHollowTopFinal,
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

    // Now can build thread holes, since space has been made in box bottom and top

    // Bottom box thread holes
    G4LogicalVolume* thread12LogVol = new G4LogicalVolume(thread12,
                                                          mthread,
                                                          "thread12"
                                                          );

    G4PVPlacement *pthread12sw = new G4PVPlacement(0,
                                                   G4ThreeVector(threadHole1_pos.x(), threadHole1_pos.y(), threadHole1_pos.z()),
                                                   "threadHoleBotSW",
                                                   thread12LogVol,
                                                   worldPhys,
                                                   false,
                                                   0
                                                   );

    G4PVPlacement *pthread12se = new G4PVPlacement(0,
                                                   G4ThreeVector(threadHole1_pos.x() - 22.2*cm, threadHole1_pos.y(), threadHole1_pos.z()),
                                                   "threadHoleBotSW",
                                                   thread12LogVol,
                                                   worldPhys,
                                                   false,
                                                   0
                                                   );

    G4PVPlacement *pthread12nw = new G4PVPlacement(0,
                                                   G4ThreeVector(threadHole1_pos.x(), threadHole1_pos.y() - 14.2*cm, threadHole1_pos.z()),
                                                   "threadHoleBotSW",
                                                   thread12LogVol,
                                                   worldPhys,
                                                   false,
                                                   0
                                                   );

    G4PVPlacement *pthread12ne = new G4PVPlacement(0,
                                                   G4ThreeVector(threadHole1_pos.x() - 22.2*cm, threadHole1_pos.y() - 14.2*cm, threadHole1_pos.z()),
                                                   "threadHoleBotSW",
                                                   thread12LogVol,
                                                   worldPhys,
                                                   false,
                                                   0
                                                   );

    // Top box thead holes
    G4LogicalVolume* thread345LogVol = new G4LogicalVolume(thread345,
                                                           mthread,
                                                           "thread345"
                                                           );

    G4PVPlacement *pthread345sw = new G4PVPlacement(0,
                                                    G4ThreeVector(threadHole3_pos.x(), threadHole3_pos.y(), threadHole3_pos.z()),
                                                    "threadHoleTopSW",
                                                    thread345LogVol,
                                                    worldPhys,
                                                    false,
                                                    0
                                                    );

    G4PVPlacement *pthread345se = new G4PVPlacement(0,
                                                     G4ThreeVector(threadHole3_pos.x() - 22.2*cm, threadHole3_pos.y(), threadHole3_pos.z()),
                                                     "threadHoleTopSE",
                                                     thread345LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    G4PVPlacement *pthread345nw = new G4PVPlacement(0,
                                                     G4ThreeVector(threadHole3_pos.x(), threadHole3_pos.y() - 14.2*cm, threadHole3_pos.z()),
                                                     "threadHoleTopNW",
                                                     thread345LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    G4PVPlacement *pthread345ne = new G4PVPlacement(0,
                                                     G4ThreeVector(threadHole3_pos.x() - 22.2*cm, threadHole3_pos.y() - 14.2*cm, threadHole3_pos.z()),
                                                     "threadHoleTopNE",
                                                     thread345LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    // Nubs of aluminum on bases of far sides of box
    G4VSolid* boxNub1Solid = new G4Box("boxNub1Solid",
                                  boxNub1_dim.x(),
                                  boxNub1_dim.y(),
                                  boxNub1_dim.z()
                                  );

    G4LogicalVolume* boxNub1LogVol = new G4LogicalVolume(boxNub1Solid,
                                                         mboxnubs,
                                                         "boxNub1LogVol"
                                                         );

    G4PVPlacement *pboxnub1w = new G4PVPlacement(0,
                                                G4ThreeVector(boxNub1_pos.x(), boxNub1_pos.y(), boxNub1_pos.z()),
                                                "boxNub1W",
                                                boxNub1LogVol,
                                                worldPhys,
                                                false,
                                                0
                                                );

    G4PVPlacement *pboxnub1e = new G4PVPlacement(0,
                                                G4ThreeVector(boxNub1_pos.x() - 22.0*cm, boxNub1_pos.y(), boxNub1_pos.z()),
                                                "boxNub1E",
                                                boxNub1LogVol,
                                                worldPhys,
                                                false,
                                                0
                                                );

    // Nubs of aluminum in each corner of box, top and bottom
    G4VSolid* boxNub2Solid = new G4Box("boxNub2Solid",
                                       boxNub2_dim.x(),
                                       boxNub2_dim.y(),
                                       boxNub2_dim.z()
                                       );

    G4LogicalVolume* boxNub2LogVol = new G4LogicalVolume(boxNub2Solid,
                                                         mboxnubs,
                                                         "boxNub2LogVol"
                                                         );

    G4PVPlacement* pboxNub2botSW = new G4PVPlacement(0,
                                                     G4ThreeVector(boxNub2_pos.x(), boxNub2_pos.y(), boxNub2_pos.z()),
                                                     "boxNub2botSW",
                                                     boxNub2LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    G4PVPlacement* pboxNub2botSE = new G4PVPlacement(0,
                                                     G4ThreeVector(boxNub2_pos.x() - 22.0*cm, boxNub2_pos.y(), boxNub2_pos.z()),
                                                     "boxNub2botSE",
                                                     boxNub2LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    G4PVPlacement* pboxNub2botNW = new G4PVPlacement(0,
                                                     G4ThreeVector(boxNub2_pos.x(), boxNub2_pos.y() - 11.8*cm, boxNub2_pos.z()),
                                                     "boxNub2botNW",
                                                     boxNub2LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    G4PVPlacement* pboxNub2botNE = new G4PVPlacement(0,
                                                     G4ThreeVector(boxNub2_pos.x() - 22.0*cm, boxNub2_pos.y() - 11.8*cm, boxNub2_pos.z()),
                                                     "boxNub2botNE",
                                                     boxNub2LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    G4PVPlacement* pboxNub2topSW = new G4PVPlacement(0,
                                                     G4ThreeVector(boxNub2_pos.x(), boxNub2_pos.y(), boxNub2_pos.z() + 8.7*cm),
                                                     "boxNub2topSW",
                                                     boxNub2LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    G4PVPlacement* pboxNub2topSE = new G4PVPlacement(0,
                                                     G4ThreeVector(boxNub2_pos.x() - 22.0*cm, boxNub2_pos.y(), boxNub2_pos.z() + 8.7*cm),
                                                     "boxNub2topSE",
                                                     boxNub2LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    G4PVPlacement* pboxNub2topNW = new G4PVPlacement(0,
                                                     G4ThreeVector(boxNub2_pos.x(), boxNub2_pos.y() - 11.8*cm, boxNub2_pos.z() + 8.7*cm),
                                                     "boxNub2topNW",
                                                     boxNub2LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    G4PVPlacement* pboxNub2topNE = new G4PVPlacement(0,
                                                     G4ThreeVector(boxNub2_pos.x() - 22.0*cm, boxNub2_pos.y() - 11.8*cm, boxNub2_pos.z() + 8.7*cm),
                                                     "boxNub2topNE",
                                                     boxNub2LogVol,
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

    G4VisAttributes* dethousing_vis_att = new G4VisAttributes(G4Color(20., 20., 40., 0.10));
    dethousing_vis_att->SetForceSolid(true);
    dethousing_vis_att->SetVisibility(true);
    dethousingLogVol -> SetVisAttributes(dethousing_vis_att);

    G4VisAttributes* boxBot_vis_att = new G4VisAttributes(G4Color(200., 200., 0., 0.10));
    boxBot_vis_att->SetForceSolid(true);
    boxBot_vis_att->SetVisibility(true);
    boxLogBot -> SetVisAttributes(boxBot_vis_att);

    G4VisAttributes* boxTop_vis_att = new G4VisAttributes(G4Color(200., 200., 0., 0.10));
    boxTop_vis_att->SetForceSolid(true);
    boxTop_vis_att->SetVisibility(true);
    boxLogTop -> SetVisAttributes(boxTop_vis_att);

    G4VisAttributes* thread12_vis_att = new G4VisAttributes(G4Color(200., 0., 0., 0.50));
    thread12_vis_att->SetForceSolid(true);
    thread12_vis_att->SetVisibility(true);
    thread12LogVol -> SetVisAttributes(thread12_vis_att);

    G4VisAttributes* thread345_vis_att = new G4VisAttributes(G4Color(200., 0., 0., 0.30));
    thread345_vis_att->SetForceSolid(true);
    thread345_vis_att->SetVisibility(true);
    thread345LogVol -> SetVisAttributes(thread345_vis_att);

    G4VisAttributes* boxNub1_vis_att = new G4VisAttributes(G4Color(0., 150., 0., 0.50));
    boxNub1_vis_att->SetForceSolid(true);
    boxNub1_vis_att->SetVisibility(true);
    boxNub1LogVol -> SetVisAttributes(boxNub1_vis_att);

    G4VisAttributes* boxNub2_vis_att = new G4VisAttributes(G4Color(0., 150., 0., 0.50));
    boxNub2_vis_att->SetForceSolid(true);
    boxNub2_vis_att->SetVisibility(true);
    boxNub2LogVol -> SetVisAttributes(boxNub2_vis_att);

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
