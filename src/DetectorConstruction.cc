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
   musbinsul(0),
   mwireinsul(0),
   mwirewire(0),
   mwireconn(0),
   mdethousing(0),
   mstand(0),
   mscrew(0),
   mbox(0),
   mthread(0),
   mboxnubs(0),
   mjack(0),
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

   // Stands and screws for motherboard
   motherStand_dim(G4ThreeVector(0.20*cm, 0.25*cm, 0.95*cm)),
   motherStand_pos(G4ThreeVector(7.75*cm, 1.75*cm, -2.13*cm)),

   motherStandScrew_dim(G4ThreeVector(0.*cm, 0.195*cm, 0.95*cm)),
   motherStandScrew_pos(G4ThreeVector(7.75*cm, 1.75*cm, -2.00*cm)),

   // Insulation and wires connecting to motherboard
   motherinsul12_dim(G4ThreeVector(0.05*cm, 0.10*cm, 1.5*cm)),
   motherinsul1_pos(G4ThreeVector(5.75*cm, 3.25*cm, -1.10*cm)),

   motherinsul2_pos(G4ThreeVector(4.75*cm, 3.25*cm, -1.10*cm)),

   motherinsul34_dim(G4ThreeVector(0.05*cm, 0.10*cm, 1.0*cm)),
   motherinsul3_pos(G4ThreeVector(3.75*cm, 2.75*cm, -1.10*cm)),

   motherinsul4_pos(G4ThreeVector(2.75*cm, 2.75*cm, -1.10*cm)),

   motherwire12_dim(G4ThreeVector(0.*cm, 0.04*cm, 1.5*cm)),
   motherwire1_pos(G4ThreeVector(5.75*cm, 3.25*cm, -1.10*cm)),

   motherwire2_pos(G4ThreeVector(4.75*cm, 3.25*cm, -1.10*cm)),

   motherwire34_dim(G4ThreeVector(0.*cm, 0.04*cm, 1.0*cm)),
   motherwire3_pos(G4ThreeVector(3.75*cm, 2.75*cm, -1.10*cm)),

   motherwire4_pos(G4ThreeVector(2.75*cm, 2.75*cm, -1.10*cm)),

   extinsul1_dim(G4ThreeVector(0.05*cm, 0.10*cm, 5.*cm)),
   extinsul1_pos(G4ThreeVector(10.75*cm, 4.75*cm, -0.35*cm)),

   extinsul2_dim(G4ThreeVector(0.05*cm, 0.10*cm, 5.5*cm)),
   extinsul2_pos(G4ThreeVector(10.25*cm, 4.75*cm, -1.95*cm)),

   extinsul3_dim(G4ThreeVector(0.05*cm, 0.10*cm, 6.*cm)),
   extinsul3_pos(G4ThreeVector(9.75*cm, 3.75*cm, -0.35*cm)),

   extinsul4_dim(G4ThreeVector(0.05*cm, 0.10*cm, 6.5*cm)),
   extinsul4_pos(G4ThreeVector(9.25*cm, 3.75*cm, -1.95*cm)),

   extwire1_dim(G4ThreeVector(0.*cm, 0.04*cm, 5.*cm)),
   extwire1_pos(G4ThreeVector(10.75*cm, 4.75*cm, -0.35*cm)),

   extwire2_dim(G4ThreeVector(0.*cm, 0.04*cm, 5.5*cm)),
   extwire2_pos(G4ThreeVector(10.25*cm, 4.75*cm, -1.95*cm)),

   extwire3_dim(G4ThreeVector(0.*cm, 0.04*cm, 6*cm)),
   extwire3_pos(G4ThreeVector(9.75*cm, 3.75*cm, -0.35*cm)),

   extwire4_dim(G4ThreeVector(0.*cm, 0.04*cm, 6.5*cm)),
   extwire4_pos(G4ThreeVector(9.25*cm, 3.75*cm, -1.95*cm)),

   // USB cord components /////////////////////////////////////////////////////

   // Wire insulation
   usbHoopTorusEast_dim(G4ThreeVector(0.25*cm, 0.5*cm, 2.3*cm)),
   usbHoopTorusEast_pos(G4ThreeVector(12.2*cm, 0.*cm, -1.14*cm)),

   usbHoopStraightNorth_dim(G4ThreeVector(0.25*cm, 0.5*cm, 1.45*cm)),
   usbHoopStraightNorth_pos(G4ThreeVector(13.65*cm, -2.3*cm, -1.14*cm)),

   usbHoopStraightSouth_dim(G4ThreeVector(0.25*cm, 0.5*cm, 1.45*cm)),
   usbHoopStraightSouth_pos(G4ThreeVector(13.65*cm, 2.3*cm, -1.14*cm)),

   usbHoopTorusWest_dim(G4ThreeVector(0.25*cm, 0.5*cm, 2.3*cm)),
   usbHoopTorusWest_pos(G4ThreeVector(15.1*cm, 0.*cm, -1.14*cm)),

   // Connectors
   usbConn_dim(G4ThreeVector(1.*cm, 0.5*cm, 0.25*cm)),
   usbConn_pos(G4ThreeVector(9.2*cm, 0.*cm, -0.64*cm)),

   // ---------------------------------------
   // Greater environment
   // ---------------------------------------

   // Box geometry
   boxBot_dim(G4ThreeVector(12.*cm, 8.*cm, 3.35*cm)),                     // box bottom with dimensions 12cm by 8cm by 3.35cm
   boxBot_pos(G4ThreeVector(7.936*cm, 3.428*cm, 0.27*cm)),                // box bottom positioned at 7.936cm along x-axis, 3.428cm along y-axis, 0.27cm along z-axis

   boxTop_dim(G4ThreeVector(12.*cm, 8.*cm, 1.65*cm)),                     // box top with dimensions 12cm by 8cm by 1.65cm
   boxTop_pos(G4ThreeVector(7.936*cm, 3.428*cm, 5.27*cm)),                // box bottom positions at 7.936cm along x-axis, 3.428cm along y-axis, 5.27cm along z-axis

   // Threaded holes in box
   threadHole1_dim(G4ThreeVector(0.33*cm, 0.39*cm, 1.0*cm)),
   threadHole1_pos(G4ThreeVector(19.036*cm, 10.528*cm, -2.08*cm)),

   threadHole2_dim(G4ThreeVector(0.37*cm, 0.39*cm, 2.35*cm)),

   threadHole3_dim(G4ThreeVector(0.37*cm, 0.39*cm, 1.35*cm)),
   threadHole3_pos(G4ThreeVector(19.036*cm, 10.528*cm, 5.12*cm)),

   threadHole4_dim(G4ThreeVector(0.33*cm, 0.39*cm, 0.15*cm)),

   threadHole5_dim(G4ThreeVector(0.36*cm, 0.39*cm, 0.15*cm)),

   // Screws for threaded holes
   threadScrew_dim(G4ThreeVector(0.*cm, 0.33*cm, 1.55*cm)),
   threadScrew_pos(G4ThreeVector(19.036*cm, 10.528*cm, 5.47*cm)),

   // Aluminum nubs on base of box
   boxNub1_dim(G4ThreeVector(0.4*cm, 0.8*cm, 0.7*cm)),
   boxNub1_pos(G4ThreeVector(18.936*cm, 3.428*cm, -2.43*cm)),

   boxNub2_dim(G4ThreeVector(0.4*cm, 0.7*cm, 0.7*cm)),
   boxNub2_pos(G4ThreeVector(18.936*cm, 9.368*cm, -2.43*cm)),

   // Other environment components

   jackStand_dim(G4ThreeVector(6.*cm, 6.*cm, 9.5*cm)),
   jackStand_pos(G4ThreeVector(13.936*cm, 3.428*cm, -12.58*cm)),

   tableTop_dim(G4ThreeVector(60.*cm, 25.*cm, 2.*cm)),                    // tabletop with dimensions 60cm by 25cm by 3cm
   tableTop_pos(G4ThreeVector(0.*cm, 0.*cm, -24.08*cm)),                  // tabletop positioned at -20.08cm along z-axis

   wall_dim(G4ThreeVector(80.*cm, 10.*cm, 200.*cm)),                      // wall with dimensions 80cm by 10cm by 200cm
   wall_pos(G4ThreeVector(0.*cm, -200*cm, 0.*cm))                         // wall positioned at -35cm along the y-axis
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

//==================================================================================================

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

    // Immediate environment materials ////////////////////////////////////////

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

    // Steel for screws and jack
    steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    mscrew = steel;
    mjack = steel;

    // Materials for wiring
    musbinsul = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

    mwireinsul = nist->FindOrBuildMaterial("G4_TEFLON");

    mwirewire = Cu;

    G4Material* brass = new G4Material("brass", 8.40*g/cm3, 2);
    brass->AddElement(Zn, 0.3);
    brass->AddMaterial(Cu, 0.7);

    mwireconn = brass;

    // Greater environment materials //////////////////////////////////////////

    G4Material* Al = new G4Material("Aluminum", 13., 26.982*g/mole, 2.70*g/cm3);

    mbox = Al;
    mthread = Al;
    mboxnubs = Al;
    mtable = Al;
    mstand = Al;

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

    new G4PVPlacement(transform,               // the placement of the volume in center
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

    new G4PVPlacement(0,
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

   new G4PVPlacement(0,
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

    new G4PVPlacement(0,
                      G4ThreeVector(dethousing_pos.x(), dethousing_pos.y(), dethousing_pos.z()),
                      "dethousing",
                      dethousingLogVol,
                      worldPhys,
                      false,
                      0
                      );

    // Stands and screws for Motherboard //////////////////////////////////////

    // Stands
    G4Tubs* motherStandSolid = new G4Tubs("motherStandSolid",
                                          motherStand_dim.x(),
                                          motherStand_dim.y(),
                                          motherStand_dim.z(),
                                          0.*deg,
                                          360.*deg
                                          );

    G4LogicalVolume* motherStandLogVol = new G4LogicalVolume(motherStandSolid,
                                                             mstand,
                                                             "motherStandLogVol"
                                                             );

    new G4PVPlacement(0,
                      G4ThreeVector(motherStand_pos.x(), motherStand_pos.y(), motherStand_pos.z()),
                      "motherStandSW",
                      motherStandLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(motherStand_pos.x() - 6.20*cm, motherStand_pos.y(), motherStand_pos.z()),
                      "motherStandSE",
                      motherStandLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(motherStand_pos.x(), motherStand_pos.y() - 3.50*cm, motherStand_pos.z()),
                      "motherStandNW",
                      motherStandLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(motherStand_pos.x() - 6.20*cm, motherStand_pos.y() - 3.50*cm, motherStand_pos.z()),
                      "motherStandNE",
                      motherStandLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(motherStand_pos.x() - 9.20*cm, motherStand_pos.y() - 1.75*cm, motherStand_pos.z()),
                      "motherStandDet",
                      motherStandLogVol,
                      worldPhys,
                      false,
                      0
                      );

    // Screws
    G4Tubs* motherStandScrewSolid = new G4Tubs("motherStandScrewSolid",
                                               motherStandScrew_dim.x(),
                                               motherStandScrew_dim.y(),
                                               motherStandScrew_dim.z(),
                                               0.*deg,
                                               360.*deg
                                               );

    G4LogicalVolume* motherStandScrewLogVol = new G4LogicalVolume(motherStandScrewSolid,
                                                                  mscrew,
                                                                  "motherStandScrewLogVol"
                                                                  );

    new G4PVPlacement(0,
                      G4ThreeVector(motherStandScrew_pos.x(), motherStandScrew_pos.y(), motherStandScrew_pos.z()),
                      "motherStandScrewSW",
                      motherStandScrewLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(motherStandScrew_pos.x() - 6.20*cm, motherStandScrew_pos.y(), motherStandScrew_pos.z()),
                      "motherStandScrewSE",
                      motherStandScrewLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(motherStandScrew_pos.x(), motherStandScrew_pos.y() - 3.50*cm, motherStandScrew_pos.z()),
                      "motherStandScrewNW",
                      motherStandScrewLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(motherStandScrew_pos.x() - 6.20*cm, motherStandScrew_pos.y() - 3.50*cm, motherStandScrew_pos.z()),
                      "motherStandScrewNE",
                      motherStandScrewLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(motherStandScrew_pos.x() - 9.20*cm, motherStandScrew_pos.y() - 1.75*cm, motherStandScrew_pos.z()),
                      "motherStandScrewDet",
                      motherStandScrewLogVol,
                      worldPhys,
                      false,
                      0
                      );

    // Insulation for wires from motherboard to external electronics
    G4Tubs* motherInsul12 = new G4Tubs("motherInsul12",
                                      motherinsul12_dim.x(),
                                      motherinsul12_dim.y(),
                                      motherinsul12_dim.z(),
                                      0.*deg,
                                      360.*deg
                                      );

    G4LogicalVolume* motherInsul12LogVol = new G4LogicalVolume(motherInsul12,
                                                              mwireinsul,
                                                              "motherInsul12LogVol"
                                                              );

    G4RotationMatrix rotins = G4RotationMatrix();
    rotins.rotateX(90.*deg);
    G4Transform3D rotins1 = G4Transform3D(rotins, motherinsul1_pos);

    new G4PVPlacement(rotins1,
                      "motherinsul1",
                      motherInsul12LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Transform3D rotins2 = G4Transform3D(rotins, motherinsul2_pos);

    new G4PVPlacement(rotins2,
                      "motherinsul2",
                      motherInsul12LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* motherInsul34 = new G4Tubs("motherInsul34",
                                      motherinsul34_dim.x(),
                                      motherinsul34_dim.y(),
                                      motherinsul34_dim.z(),
                                      0.*deg,
                                      360.*deg
                                      );

    G4LogicalVolume* motherInsul34LogVol = new G4LogicalVolume(motherInsul34,
                                                              musbinsul,
                                                              "motherInsul34LogVol"
                                                              );

    G4Transform3D rotins3 = G4Transform3D(rotins, motherinsul3_pos);

    new G4PVPlacement(rotins3,
                      "motherinsul3",
                      motherInsul34LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Transform3D rotins4 = G4Transform3D(rotins, motherinsul4_pos);

    new G4PVPlacement(rotins4,
                      "motherinsu34",
                      motherInsul34LogVol,
                      worldPhys,
                      false,
                      0
                      );

    // Wires from motherboard to external electronics
    G4Tubs* motherwire12 = new G4Tubs("motherwire12",
                                      motherwire12_dim.x(),
                                      motherwire12_dim.y(),
                                      motherwire12_dim.z(),
                                      0.*deg,
                                      360.*deg
                                      );

    G4LogicalVolume* motherwire12LogVol = new G4LogicalVolume(motherwire12,
                                                              mwirewire,
                                                              "motherwire12LogVol"
                                                              );

    G4RotationMatrix rotwire = G4RotationMatrix();
    rotwire.rotateX(90.*deg);
    G4Transform3D rotwire1 = G4Transform3D(rotwire, motherwire1_pos);

    new G4PVPlacement(rotwire1,
                      "motherwire1",
                      motherwire12LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Transform3D rotwire2 = G4Transform3D(rotwire, motherwire2_pos);

    new G4PVPlacement(rotwire2,
                      "motherwire2",
                      motherwire12LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* motherwire34 = new G4Tubs("motherwire34",
                                      motherwire34_dim.x(),
                                      motherwire34_dim.y(),
                                      motherwire34_dim.z(),
                                      0.*deg,
                                      360.*deg
                                      );

    G4LogicalVolume* motherwire34LogVol = new G4LogicalVolume(motherwire34,
                                                              mwirewire,
                                                              "motherwire34LogVol"
                                                              );

    G4Transform3D rotwire3 = G4Transform3D(rotwire, motherwire3_pos);

    new G4PVPlacement(rotwire3,
                      "motherwire3",
                      motherwire34LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Transform3D rotwire4 = G4Transform3D(rotwire, motherwire4_pos);

    new G4PVPlacement(rotwire4,
                      "motherwire4",
                      motherwire34LogVol,
                      worldPhys,
                      false,
                      0
                      );

    // Insulation for second section of wires coming from motherboard to ...
    // external electronics
    G4Tubs* extinsul1 = new G4Tubs("extinsul1",
                                   extinsul1_dim.x(),
                                   extinsul1_dim.y(),
                                   extinsul1_dim.z(),
                                   0.*deg,
                                   360.*deg
                                   );

    G4LogicalVolume* extinsul1LogVol = new G4LogicalVolume(extinsul1,
                                                              mwireinsul,
                                                              "extinsul1LogVol"
                                                              );

    G4RotationMatrix rotinsy = G4RotationMatrix();
    rotinsy.rotateY(82.*deg);
    G4Transform3D rotinsy1 = G4Transform3D(rotinsy, extinsul1_pos);

    new G4PVPlacement(rotinsy1,
                      "extinsul1",
                      extinsul1LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* extinsul2 = new G4Tubs("extinsul2",
                                   extinsul2_dim.x(),
                                   extinsul2_dim.y(),
                                   extinsul2_dim.z(),
                                   0.*deg,
                                   360.*deg
                                   );

    G4LogicalVolume* extinsul2LogVol = new G4LogicalVolume(extinsul2,
                                                              mwireinsul,
                                                              "extinsul2LogVol"
                                                              );

    rotinsy.rotateY(16.*deg);
    G4Transform3D rotinsy2 = G4Transform3D(rotinsy, extinsul2_pos);

    new G4PVPlacement(rotinsy2,
                      "extinsul2",
                      extinsul2LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* extinsul3 = new G4Tubs("extinsul3",
                                      extinsul3_dim.x(),
                                      extinsul3_dim.y(),
                                      extinsul3_dim.z(),
                                      0.*deg,
                                      360.*deg
                                      );

    G4LogicalVolume* extinsul3LogVol = new G4LogicalVolume(extinsul3,
                                                              musbinsul,
                                                              "extinsul3LogVol"
                                                              );

    rotinsy.rotateY(-16.*deg);
    G4Transform3D rotinsy3 = G4Transform3D(rotinsy, extinsul3_pos);

    new G4PVPlacement(rotinsy3,
                      "extinsul3",
                      extinsul3LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* extinsul4 = new G4Tubs("extinsul4",
                                      extinsul4_dim.x(),
                                      extinsul4_dim.y(),
                                      extinsul4_dim.z(),
                                      0.*deg,
                                      360.*deg
                                      );

    G4LogicalVolume* extinsul4LogVol = new G4LogicalVolume(extinsul4,
                                                              musbinsul,
                                                              "extinsul4LogVol"
                                                              );

    rotinsy.rotateY(16.*deg);
    G4Transform3D rotinsy4 = G4Transform3D(rotinsy, extinsul4_pos);

    new G4PVPlacement(rotinsy4,
                      "extinsul4",
                      extinsul4LogVol,
                      worldPhys,
                      false,
                      0
                      );

    // Wires from motherboard to external electronics
    G4Tubs* extwire1 = new G4Tubs("extwire1",
                                      extwire1_dim.x(),
                                      extwire1_dim.y(),
                                      extwire1_dim.z(),
                                      0.*deg,
                                      360.*deg
                                      );

    G4LogicalVolume* extwire1LogVol = new G4LogicalVolume(extwire1,
                                                          mwirewire,
                                                          "extwire1LogVol"
                                                          );

    G4RotationMatrix rotwirey = G4RotationMatrix();
    rotwirey.rotateY(82.*deg);
    G4Transform3D rotwirey1 = G4Transform3D(rotwirey, extwire1_pos);

    new G4PVPlacement(rotwirey1,
                      "extwire1",
                      extwire1LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* extwire2 = new G4Tubs("extwire2",
                                  extwire2_dim.x(),
                                  extwire2_dim.y(),
                                  extwire2_dim.z(),
                                  0.*deg,
                                  360.*deg
                                  );

    G4LogicalVolume* extwire2LogVol = new G4LogicalVolume(extwire2,
                                                          mwirewire,
                                                          "extwire2LogVol"
                                                          );

    rotwirey.rotateY(16.*deg);
    G4Transform3D rotwirey2 = G4Transform3D(rotwirey, extwire2_pos);

    new G4PVPlacement(rotwirey2,
                      "extwire2",
                      extwire2LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* extwire3 = new G4Tubs("extwire3",
                                      extwire3_dim.x(),
                                      extwire3_dim.y(),
                                      extwire3_dim.z(),
                                      0.*deg,
                                      360.*deg
                                      );

    G4LogicalVolume* extwire3LogVol = new G4LogicalVolume(extwire3,
                                                          mwirewire,
                                                          "extwire3LogVol"
                                                          );
    rotwirey.rotateY(-16.*deg);
    G4Transform3D rotwirey3 = G4Transform3D(rotwirey, extwire3_pos);

    new G4PVPlacement(rotwirey3,
                      "extwire3",
                      extwire3LogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* extwire4 = new G4Tubs("extwire4",
                                  extwire4_dim.x(),
                                  extwire4_dim.y(),
                                  extwire4_dim.z(),
                                  0.*deg,
                                  360.*deg
                                  );

    G4LogicalVolume* extwire4LogVol = new G4LogicalVolume(extwire4,
                                                          mwirewire,
                                                          "extwire4LogVol"
                                                          );

    rotwirey.rotateY(16.*deg);
    G4Transform3D rotwirey4 = G4Transform3D(rotwirey, extwire4_pos);

    new G4PVPlacement(rotwirey4,
                      "extwire4",
                      extwire4LogVol,
                      worldPhys,
                      false,
                      0
                      );

    // Analog of USB wire-loop (insulation)
    G4Torus* usbHoopTorusEastSolid = new G4Torus("usbHoopTorusEastSolid",
                                                 usbHoopTorusEast_dim.x(),
                                                 usbHoopTorusEast_dim.y(),
                                                 usbHoopTorusEast_dim.z(),
                                                 90.*deg,
                                                 180.*deg
                                                 );

    G4LogicalVolume* usbHoopTorusEastLogVol = new G4LogicalVolume(usbHoopTorusEastSolid,
                                                                  musbinsul,
                                                                  "usbHoopTorusEastLogVol"
                                                                  );

    new G4PVPlacement(0,
                      G4ThreeVector(usbHoopTorusEast_pos.x(), usbHoopTorusEast_pos.y(), usbHoopTorusEast_pos.z()),
                      "usbHoopTorusEast",
                      usbHoopTorusEastLogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* usbHoopStraightNorthSolid = new G4Tubs("usbHoopStraightNorthSolid",
                                                   usbHoopStraightNorth_dim.x(),
                                                   usbHoopStraightNorth_dim.y(),
                                                   usbHoopStraightNorth_dim.z(),
                                                   0.*deg,
                                                   360.*deg
                                                   );

    G4LogicalVolume* usbHoopStraightNorthLogVol = new G4LogicalVolume(usbHoopStraightNorthSolid,
                                                                      musbinsul,
                                                                      "usbHoopStraightNorthLogVol"
                                                                      );

    G4RotationMatrix rotm = G4RotationMatrix();
    rotm.rotateY(90*deg);
    G4Transform3D rotN = G4Transform3D(rotm, usbHoopStraightNorth_pos);

    new G4PVPlacement(rotN,
                      "usbHoopStraightNorth",
                      usbHoopStraightNorthLogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* usbHoopStraightSouthSolid = new G4Tubs("usbHoopStraightSouthSolid",
                                                   usbHoopStraightSouth_dim.x(),
                                                   usbHoopStraightSouth_dim.y(),
                                                   usbHoopStraightSouth_dim.z(),
                                                   0.*deg,
                                                   360.*deg
                                                   );

    G4LogicalVolume* usbHoopStraightSouthLogVol = new G4LogicalVolume(usbHoopStraightSouthSolid,
                                                                      musbinsul,
                                                                      "usbHoopStraightSouthLogVol"
                                                                      );

    G4Transform3D rotS = G4Transform3D(rotm, usbHoopStraightSouth_pos);

    new G4PVPlacement(rotS,
                      "usbHoopStraightSouth",
                      usbHoopStraightSouthLogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Torus* usbHoopTorusWestSolid = new G4Torus("usbHoopTorusWestSolid",
                                                 usbHoopTorusWest_dim.x(),
                                                 usbHoopTorusWest_dim.y(),
                                                 usbHoopTorusWest_dim.z(),
                                                 -90.*deg,
                                                 180.*deg
                                                 );

    G4LogicalVolume* usbHoopTorusWestLogVol = new G4LogicalVolume(usbHoopTorusWestSolid,
                                                                      musbinsul,
                                                                      "usbHoopTorusWestLogVol"
                                                                      );

    new G4PVPlacement(0,
                      G4ThreeVector(usbHoopTorusWest_pos.x(), usbHoopTorusWest_pos.y(), usbHoopTorusWest_pos.z()),
                      "usbHoopTorusWest",
                      usbHoopTorusWestLogVol,
                      worldPhys,
                      false,
                      0
                      );

    // Analog of USB wire-loop (copper internals)
    G4Torus* usbHoopTorusEastWireSolid = new G4Torus("usbHoopTorusEastWireSolid",
                                                 0.*cm,
                                                 usbHoopTorusEast_dim.y() - 0.15*cm,
                                                 usbHoopTorusEast_dim.z(),
                                                 90.*deg,
                                                 180.*deg
                                                 );

    G4LogicalVolume* usbHoopTorusEastWireLogVol = new G4LogicalVolume(usbHoopTorusEastWireSolid,
                                                                  mwirewire,
                                                                  "usbHoopTorusEastWireLogVol"
                                                                  );

    new G4PVPlacement(0,
                      G4ThreeVector(usbHoopTorusEast_pos.x(), usbHoopTorusEast_pos.y(), usbHoopTorusEast_pos.z()),
                      "usbHoopTorusEastWire",
                      usbHoopTorusEastWireLogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* usbHoopStraightNorthWireSolid = new G4Tubs("usbHoopStraightNorthWireSolid",
                                                       0.*cm,
                                                       usbHoopStraightNorth_dim.y() - 0.15*cm,
                                                       usbHoopStraightNorth_dim.z(),
                                                       0.*deg,
                                                       360.*deg
                                                       );

    G4LogicalVolume* usbHoopStraightNorthWireLogVol = new G4LogicalVolume(usbHoopStraightNorthWireSolid,
                                                                          mwirewire,
                                                                          "usbHoopStraightNorthWireLogVol"
                                                                          );

    new G4PVPlacement(rotN,
                      "usbHoopStraightNorthWire",
                      usbHoopStraightNorthWireLogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Tubs* usbHoopStraightSouthWireSolid = new G4Tubs("usbHoopStraightSouthWireSolid",
                                                       0.*cm,
                                                       usbHoopStraightSouth_dim.y() - 0.15*cm,
                                                       usbHoopStraightSouth_dim.z(),
                                                       0.*deg,
                                                       360.*deg
                                                       );

    G4LogicalVolume* usbHoopStraightSouthWireLogVol = new G4LogicalVolume(usbHoopStraightSouthWireSolid,
                                                                          mwirewire,
                                                                          "usbHoopStraightSouthWireLogVol"
                                                                          );

    new G4PVPlacement(rotS,
                      "usbHoopStraightSouthWire",
                      usbHoopStraightSouthWireLogVol,
                      worldPhys,
                      false,
                      0
                      );

    G4Torus* usbHoopTorusWestWireSolid = new G4Torus("usbHoopTorusWestWireSolid",
                                                     0.*cm,
                                                     usbHoopTorusWest_dim.y() - 0.15*cm,
                                                     usbHoopTorusWest_dim.z(),
                                                     -90.*deg,
                                                     180.*deg
                                                     );

    G4LogicalVolume* usbHoopTorusWestWireLogVol = new G4LogicalVolume(usbHoopTorusWestWireSolid,
                                                                      mwirewire,
                                                                      "usbHoopTorusWestWireLogVol"
                                                                      );

    new G4PVPlacement(0,
                      G4ThreeVector(usbHoopTorusWest_pos.x(), usbHoopTorusWest_pos.y(), usbHoopTorusWest_pos.z()),
                      "usbHoopTorusWestWire",
                      usbHoopTorusWestWireLogVol,
                      worldPhys,
                      false,
                      0
                      );

    // USB connectors to motherboard and external wiring
    G4Box* usbConnSolid = new G4Box("usbConnSolid",
                                    usbConn_dim.x(),
                                    usbConn_dim.y(),
                                    usbConn_dim.z()
                                    );

    G4LogicalVolume* usbConnLogVol = new G4LogicalVolume(usbConnSolid,
                                                         mwireconn,
                                                         "usbConnLogVol"
                                                         );

    new G4PVPlacement(0,
                      G4ThreeVector(usbConn_pos.x(), usbConn_pos.y(), usbConn_pos.z()),
                      "usbConnEast",
                      usbConnLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(usbConn_pos.x() + 8.9*cm, usbConn_pos.y(), usbConn_pos.z()),
                      "usbConnWest",
                      usbConnLogVol,
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
                                     0.*deg,
                                     360.*deg
                                     );

    G4Tubs* threadHole2 = new G4Tubs("threadHole2",
                                     threadHole2_dim.x(),
                                     threadHole2_dim.y(),
                                     threadHole2_dim.z(),
                                     0.*deg,
                                     360.*deg
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
                                     0.*deg,
                                     360.*deg
                                     );

    G4Tubs* threadHole4 = new G4Tubs("threadHole4",
                                     threadHole4_dim.x(),
                                     threadHole4_dim.y(),
                                     threadHole4_dim.z(),
                                     0.*deg,
                                     360.*deg
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
                                     0.*deg,
                                     360.*deg
                                     );

    G4UnionSolid* thread345 = new G4UnionSolid("thread345",
                                              thread34,
                                              threadHole5,
                                              0,
                                              G4ThreeVector(0.*cm, 0.*cm, 1.65*cm)
                                              );

    G4Tubs* threadScrewSolid = new G4Tubs("threadScrewSolid",
                                          threadScrew_dim.x(),
                                          threadScrew_dim.y(),
                                          threadScrew_dim.z(),
                                          0.*deg,
                                          360.*deg
                                          );

    G4LogicalVolume* threadScrewLogVol = new G4LogicalVolume(threadScrewSolid,
                                                             mscrew,
                                                             "threadScrewLogVol"
                                                             );

    new G4PVPlacement(0,
                      G4ThreeVector(threadScrew_pos.x(), threadScrew_pos.y(), threadScrew_pos.z()),
                      "threadScrewSW",
                      threadScrewLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(threadScrew_pos.x() - 22.2*cm, threadScrew_pos.y(), threadScrew_pos.z()),
                      "threadScrewSE",
                      threadScrewLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(threadScrew_pos.x(), threadScrew_pos.y() - 14.2*cm, threadScrew_pos.z()),
                      "threadScrewNW",
                      threadScrewLogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(threadScrew_pos.x() - 22.2*cm, threadScrew_pos.y() - 14.2*cm, threadScrew_pos.z()),
                      "threadScrewNE",
                      threadScrewLogVol,
                      worldPhys,
                      false,
                      0
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

    new G4PVPlacement(0,
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

    new G4PVPlacement(0,
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

    new G4PVPlacement(0,
                                                   G4ThreeVector(threadHole1_pos.x(), threadHole1_pos.y(), threadHole1_pos.z()),
                                                   "threadHoleBotSW",
                                                   thread12LogVol,
                                                   worldPhys,
                                                   false,
                                                   0
                                                   );

    new G4PVPlacement(0,
                                                   G4ThreeVector(threadHole1_pos.x() - 22.2*cm, threadHole1_pos.y(), threadHole1_pos.z()),
                                                   "threadHoleBotSW",
                                                   thread12LogVol,
                                                   worldPhys,
                                                   false,
                                                   0
                                                   );

    new G4PVPlacement(0,
                                                   G4ThreeVector(threadHole1_pos.x(), threadHole1_pos.y() - 14.2*cm, threadHole1_pos.z()),
                                                   "threadHoleBotSW",
                                                   thread12LogVol,
                                                   worldPhys,
                                                   false,
                                                   0
                                                   );

    new G4PVPlacement(0,
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

    new G4PVPlacement(0,
                                                    G4ThreeVector(threadHole3_pos.x(), threadHole3_pos.y(), threadHole3_pos.z()),
                                                    "threadHoleTopSW",
                                                    thread345LogVol,
                                                    worldPhys,
                                                    false,
                                                    0
                                                    );

    new G4PVPlacement(0,
                                                     G4ThreeVector(threadHole3_pos.x() - 22.2*cm, threadHole3_pos.y(), threadHole3_pos.z()),
                                                     "threadHoleTopSE",
                                                     thread345LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    new G4PVPlacement(0,
                                                     G4ThreeVector(threadHole3_pos.x(), threadHole3_pos.y() - 14.2*cm, threadHole3_pos.z()),
                                                     "threadHoleTopNW",
                                                     thread345LogVol,
                                                     worldPhys,
                                                     false,
                                                     0
                                                     );

    new G4PVPlacement(0,
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

    new G4PVPlacement(0,
                                                G4ThreeVector(boxNub1_pos.x(), boxNub1_pos.y(), boxNub1_pos.z()),
                                                "boxNub1W",
                                                boxNub1LogVol,
                                                worldPhys,
                                                false,
                                                0
                                                );

    new G4PVPlacement(0,
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

    new G4PVPlacement(0,
                      G4ThreeVector(boxNub2_pos.x(), boxNub2_pos.y(), boxNub2_pos.z()),
                      "boxNub2botSW",
                      boxNub2LogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(boxNub2_pos.x() - 22.0*cm, boxNub2_pos.y(), boxNub2_pos.z()),
                      "boxNub2botSE",
                      boxNub2LogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(boxNub2_pos.x(), boxNub2_pos.y() - 11.8*cm, boxNub2_pos.z()),
                      "boxNub2botNW",
                      boxNub2LogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(boxNub2_pos.x() - 22.0*cm, boxNub2_pos.y() - 11.8*cm, boxNub2_pos.z()),
                      "boxNub2botNE",
                      boxNub2LogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(boxNub2_pos.x(), boxNub2_pos.y(), boxNub2_pos.z() + 8.7*cm),
                      "boxNub2topSW",
                      boxNub2LogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(boxNub2_pos.x() - 22.0*cm, boxNub2_pos.y(), boxNub2_pos.z() + 8.7*cm),
                      "boxNub2topSE",
                      boxNub2LogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(boxNub2_pos.x(), boxNub2_pos.y() - 11.8*cm, boxNub2_pos.z() + 8.7*cm),
                      "boxNub2topNW",
                      boxNub2LogVol,
                      worldPhys,
                      false,
                      0
                      );

    new G4PVPlacement(0,
                      G4ThreeVector(boxNub2_pos.x() - 22.0*cm, boxNub2_pos.y() - 11.8*cm, boxNub2_pos.z() + 8.7*cm),
                      "boxNub2topNE",
                      boxNub2LogVol,
                      worldPhys,
                      false,
                      0
                      );

    // Jack-stand to raise box from table
    // Made hollow to approximate
    G4Box* jackStandOuter = new G4Box("jackStandOuter",
                                      jackStand_dim.x(),
                                      jackStand_dim.y(),
                                      jackStand_dim.z()
                                      );

    G4Box* jackStandInner = new G4Box("jackStandInner",
                                      jackStand_dim.x() - 2.*cm,
                                      jackStand_dim.y() - 2.*cm,
                                      jackStand_dim.z() - 2.*cm
                                      );

    G4SubtractionSolid* jackStandSolid = new G4SubtractionSolid("jackStandSolid",
                                                                jackStandOuter,
                                                                jackStandInner
                                                                );

    G4LogicalVolume* jackStandLogVol = new G4LogicalVolume(jackStandSolid,
                                                           mjack,
                                                           "jackStandLogVol"
                                                           );

    new G4PVPlacement(0,
                      G4ThreeVector(jackStand_pos.x(), jackStand_pos.y(), jackStand_pos.z()),
                      "jackStand",
                      jackStandLogVol,
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

    new G4PVPlacement(0,
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

    new G4PVPlacement(0,
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

    G4VisAttributes* detector_vis_att = new G4VisAttributes(G4Color(0., 0.8, 0.9, 1.));
    detector_vis_att->SetForceSolid(true);
    detector_vis_att->SetVisibility(true);
    logvol -> SetVisAttributes(detector_vis_att);

    G4VisAttributes* asic_vis_att = new G4VisAttributes(G4Color(0.7, 0., 0.7, 0.30));
    asic_vis_att->SetForceSolid(true);
    asic_vis_att->SetVisibility(true);
    asicLogVol -> SetVisAttributes(asic_vis_att);

    G4VisAttributes* motherb_vis_att = new G4VisAttributes(G4Color(0.7, 0., 0.7, 0.30));
    motherb_vis_att->SetForceSolid(true);
    motherb_vis_att->SetVisibility(true);
    motherbLogVol -> SetVisAttributes(motherb_vis_att);

    G4VisAttributes* dethousing_vis_att = new G4VisAttributes(G4Color(0.4, 0.4, 0.6, 0.10));
    dethousing_vis_att->SetForceSolid(true);
    dethousing_vis_att->SetVisibility(true);
    dethousingLogVol -> SetVisAttributes(dethousing_vis_att);

    // Stands and screws for Motherboard

    G4VisAttributes* motherStandScrew_vis_att = new G4VisAttributes(G4Color(0.2, 0.4, 0.6, 0.60));
    motherStandScrew_vis_att->SetForceSolid(true);
    motherStandScrew_vis_att->SetVisibility(true);
    motherStandScrewLogVol -> SetVisAttributes(motherStandScrew_vis_att);

    G4VisAttributes* motherStand_vis_att = new G4VisAttributes(G4Color(0.2, 0.35, 0.8, 0.40));
    motherStand_vis_att->SetForceSolid(true);
    motherStand_vis_att->SetVisibility(true);
    motherStandLogVol -> SetVisAttributes(motherStand_vis_att);

    // Insulation and wires from Motherboard

    G4VisAttributes* motherInsul12_vis_att = new G4VisAttributes(G4Color(0.2, 0.35, 0.35, 0.30));
    motherInsul12_vis_att->SetForceSolid(true);
    motherInsul12_vis_att->SetVisibility(true);
    motherInsul12LogVol -> SetVisAttributes(motherInsul12_vis_att);

    G4VisAttributes* motherInsul34_vis_att = new G4VisAttributes(G4Color(0.45, 0.35, 0.35, 0.30));
    motherInsul34_vis_att->SetForceSolid(true);
    motherInsul34_vis_att->SetVisibility(true);
    motherInsul34LogVol -> SetVisAttributes(motherInsul34_vis_att);

    G4VisAttributes* motherwire12_vis_att = new G4VisAttributes(G4Color(0.8, 0.35, 0., 0.60));
    motherwire12_vis_att->SetForceSolid(true);
    motherwire12_vis_att->SetVisibility(true);
    motherwire12LogVol -> SetVisAttributes(motherwire12_vis_att);

    G4VisAttributes* motherwire34_vis_att = new G4VisAttributes(G4Color(0.8, 0.35, 0., 0.60));
    motherwire34_vis_att->SetForceSolid(true);
    motherwire34_vis_att->SetVisibility(true);
    motherwire34LogVol -> SetVisAttributes(motherwire34_vis_att);

    G4VisAttributes* extinsul1_vis_att = new G4VisAttributes(G4Color(0.2, 0.35, 0.35, 0.30));
    extinsul1_vis_att->SetForceSolid(true);
    extinsul1_vis_att->SetVisibility(true);
    extinsul1LogVol -> SetVisAttributes(extinsul1_vis_att);

    G4VisAttributes* extinsul2_vis_att = new G4VisAttributes(G4Color(0.2, 0.35, 0.35, 0.30));
    extinsul2_vis_att->SetForceSolid(true);
    extinsul2_vis_att->SetVisibility(true);
    extinsul2LogVol -> SetVisAttributes(extinsul2_vis_att);

    G4VisAttributes* extinsul3_vis_att = new G4VisAttributes(G4Color(0.45, 0.35, 0.35, 0.30));
    extinsul3_vis_att->SetForceSolid(true);
    extinsul3_vis_att->SetVisibility(true);
    extinsul3LogVol -> SetVisAttributes(extinsul3_vis_att);

    G4VisAttributes* extinsul4_vis_att = new G4VisAttributes(G4Color(0.45, 0.35, 0.35, 0.30));
    extinsul4_vis_att->SetForceSolid(true);
    extinsul4_vis_att->SetVisibility(true);
    extinsul4LogVol -> SetVisAttributes(extinsul4_vis_att);

    G4VisAttributes* extwire1_vis_att = new G4VisAttributes(G4Color(0.8, 0.35, 0., 0.60));
    extwire1_vis_att->SetForceSolid(true);
    extwire1_vis_att->SetVisibility(true);
    extwire1LogVol -> SetVisAttributes(extwire1_vis_att);

    G4VisAttributes* extwire2_vis_att = new G4VisAttributes(G4Color(0.8, 0.35, 0., 0.60));
    extwire2_vis_att->SetForceSolid(true);
    extwire2_vis_att->SetVisibility(true);
    extwire2LogVol -> SetVisAttributes(extwire2_vis_att);

    G4VisAttributes* extwire3_vis_att = new G4VisAttributes(G4Color(0.8, 0.35, 0., 0.60));
    extwire3_vis_att->SetForceSolid(true);
    extwire3_vis_att->SetVisibility(true);
    extwire3LogVol -> SetVisAttributes(extwire3_vis_att);

    G4VisAttributes* extwire4_vis_att = new G4VisAttributes(G4Color(0.8, 0.35, 0., 0.60));
    extwire4_vis_att->SetForceSolid(true);
    extwire4_vis_att->SetVisibility(true);
    extwire4LogVol -> SetVisAttributes(extwire4_vis_att);

    // USB components

    G4VisAttributes* usbHoopTorusEast_vis_att = new G4VisAttributes(G4Color(0.4, 0.4, 0.4, 0.30));
    usbHoopTorusEast_vis_att->SetForceSolid(true);
    usbHoopTorusEast_vis_att->SetVisibility(true);
    usbHoopTorusEastLogVol -> SetVisAttributes(usbHoopTorusEast_vis_att);

    G4VisAttributes* usbHoopStraightNorth_vis_att = new G4VisAttributes(G4Color(0.4, 0.4, 0.4, 0.30));
    usbHoopStraightNorth_vis_att->SetForceSolid(true);
    usbHoopStraightNorth_vis_att->SetVisibility(true);
    usbHoopStraightNorthLogVol -> SetVisAttributes(usbHoopStraightNorth_vis_att);

    G4VisAttributes* usbHoopStraightSouth_vis_att = new G4VisAttributes(G4Color(0.4, 0.4, 0.4, 0.30));
    usbHoopStraightSouth_vis_att->SetForceSolid(true);
    usbHoopStraightSouth_vis_att->SetVisibility(true);
    usbHoopStraightSouthLogVol -> SetVisAttributes(usbHoopStraightSouth_vis_att);

    G4VisAttributes* usbHoopTorusWest_vis_att = new G4VisAttributes(G4Color(0.4, 0.4, 0.4, 0.30));
    usbHoopTorusWest_vis_att->SetForceSolid(true);
    usbHoopTorusWest_vis_att->SetVisibility(true);
    usbHoopTorusWestLogVol -> SetVisAttributes(usbHoopTorusWest_vis_att);

    G4VisAttributes* usbHoopTorusEastWire_vis_att = new G4VisAttributes(G4Color(0.8, 0.3, 0., 0.60));
    usbHoopTorusEastWire_vis_att->SetForceSolid(true);
    usbHoopTorusEastWire_vis_att->SetVisibility(true);
    usbHoopTorusEastWireLogVol -> SetVisAttributes(usbHoopTorusEastWire_vis_att);

    G4VisAttributes* usbHoopStraightNorthWire_vis_att = new G4VisAttributes(G4Color(0.8, 0.3, 0., 0.60));
    usbHoopStraightNorthWire_vis_att->SetForceSolid(true);
    usbHoopStraightNorthWire_vis_att->SetVisibility(true);
    usbHoopStraightNorthWireLogVol -> SetVisAttributes(usbHoopStraightNorthWire_vis_att);

    G4VisAttributes* usbHoopStraightSouthWire_vis_att = new G4VisAttributes(G4Color(0.8, 0.3, 0., 0.60));
    usbHoopStraightSouthWire_vis_att->SetForceSolid(true);
    usbHoopStraightSouthWire_vis_att->SetVisibility(true);
    usbHoopStraightSouthWireLogVol -> SetVisAttributes(usbHoopStraightSouthWire_vis_att);

    G4VisAttributes* usbHoopTorusWestWire_vis_att = new G4VisAttributes(G4Color(0.8, 0.3, 0., 0.60));
    usbHoopTorusWestWire_vis_att->SetForceSolid(true);
    usbHoopTorusWestWire_vis_att->SetVisibility(true);
    usbHoopTorusWestWireLogVol -> SetVisAttributes(usbHoopTorusWestWire_vis_att);

    G4VisAttributes* usbConn_vis_att = new G4VisAttributes(G4Color(0.8, 0.5, 0., 0.60));
    usbConn_vis_att->SetForceSolid(true);
    usbConn_vis_att->SetVisibility(true);
    usbConnLogVol -> SetVisAttributes(usbConn_vis_att);

    // Box and detailing

    G4VisAttributes* boxBot_vis_att = new G4VisAttributes(G4Color(0.6, 0.6, 0., 0.12));
    boxBot_vis_att->SetForceSolid(true);
    boxBot_vis_att->SetVisibility(true);
    boxLogBot -> SetVisAttributes(boxBot_vis_att);

    G4VisAttributes* boxTop_vis_att = new G4VisAttributes(G4Color(0.6, 0.6, 0., 0.12));
    boxTop_vis_att->SetForceSolid(true);
    boxTop_vis_att->SetVisibility(true);
    boxLogTop -> SetVisAttributes(boxTop_vis_att);

    G4VisAttributes* thread12_vis_att = new G4VisAttributes(G4Color(0.5, 0., 0., 0.50));
    thread12_vis_att->SetForceSolid(true);
    thread12_vis_att->SetVisibility(true);
    thread12LogVol -> SetVisAttributes(thread12_vis_att);

    G4VisAttributes* thread345_vis_att = new G4VisAttributes(G4Color(0.5, 0., 0., 0.30));
    thread345_vis_att->SetForceSolid(true);
    thread345_vis_att->SetVisibility(true);
    thread345LogVol -> SetVisAttributes(thread345_vis_att);

    G4VisAttributes* threadScrew_vis_att = new G4VisAttributes(G4Color(0.2, 0.4, 0.6, 0.60));
    threadScrew_vis_att->SetForceSolid(true);
    threadScrew_vis_att->SetVisibility(true);
    threadScrewLogVol -> SetVisAttributes(threadScrew_vis_att);

    G4VisAttributes* boxNub1_vis_att = new G4VisAttributes(G4Color(0.5, 0.5, 0., 0.50));
    boxNub1_vis_att->SetForceSolid(true);
    boxNub1_vis_att->SetVisibility(true);
    boxNub1LogVol -> SetVisAttributes(boxNub1_vis_att);

    G4VisAttributes* boxNub2_vis_att = new G4VisAttributes(G4Color(0.5, 0.5, 0., 0.50));
    boxNub2_vis_att->SetForceSolid(true);
    boxNub2_vis_att->SetVisibility(true);
    boxNub2LogVol -> SetVisAttributes(boxNub2_vis_att);

    // Outer geometry

    G4VisAttributes* jackStand_vis_att = new G4VisAttributes(G4Color(0.45, 0.45, 0.40, 0.25));
    jackStand_vis_att->SetForceSolid(true);
    jackStand_vis_att->SetVisibility(true);
    jackStandLogVol -> SetVisAttributes(jackStand_vis_att);

    G4VisAttributes* table_vis_att = new G4VisAttributes(G4Color(0.45, 0.55, 0., 0.22));
    table_vis_att->SetForceSolid(true);
    table_vis_att->SetVisibility(true);
    tableLogVol -> SetVisAttributes(table_vis_att);

    // G4VisAttributes* wall_vis_att = new G4VisAttributes(G4Color(50., 1., 1., 0.10));
    // wall_vis_att->SetForceSolid(true);
    // wall_vis_att->SetVisibility(true);
    // wallLogVol -> SetVisAttributes(wall_vis_att);

    // World

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
