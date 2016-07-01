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


//==================================================================================================

DetectorConstruction* DetectorConstruction::fgInstance = 0;

//==================================================================================================

DetectorConstruction* DetectorConstruction::Instance() {
    
	return fgInstance;
}

//==================================================================================================

DetectorConstruction::DetectorConstruction()
: worldPhys(0), mworld(0), mdetector(0),
  world_dim(30*CLHEP::cm),                                                     // default world is a 50 cm radius sphere
  detector_dim(G4ThreeVector(0.5*CLHEP::cm, 0.5*CLHEP::cm, 0.5*CLHEP::cm)),    // default detector is 1 cc cube (use half sizes)
  detector_pos(G4ThreeVector(0.)),                                             // default position at 0, 0, 0
  _checkoverlaps(false)                                                        // by default, dont check overlaps while constructing
{
 
    // Get a random mask
    _mask = GetRandomMask();
    
    // Put it into hex form for storage later...
    G4String mask_hex = BinToHex(_mask);
    
    // Here is the method to put the hex string back into a binary array
        //std::vector<G4int> _mask_ = HexToBin(mask_hex);
    
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
    //std::string logname = "DetLog";
    for (int i = 0; i < 192; i++){
        //logname += std::to_string(i+1);
        logvols.push_back(new G4LogicalVolume(detectorSolid,    // target solid
                                              mdetector,        // target material
                                              "DetectorLog"     // name
                                              ));
    }
    
   
    // Pull in detector center verticies from file
    std::ifstream myfile("geo/ring/centervertices_Ring.txt");
    double x_, y_, z_;
    std::string line;
    if (myfile.is_open()){
        while (getline(myfile,line)){
            std::istringstream(line) >> x_ >> y_ >> z_;
            centers.push_back(G4ThreeVector(x_*CLHEP::mm, y_*CLHEP::mm ,z_*CLHEP::mm));
        }
        myfile.close();
    }
    
    // Pull in rotation matrices from file
    std::vector<G4RotationMatrix> rotationmat;
    std::ifstream myfile3("geo/ring/rotationmatrices_Ring.txt");
    double x1_,x2_,x3_,y1_,y2_,y3_,z1_,z2_,z3_;
    if (myfile3.is_open()){
        while (getline(myfile3,line)){
            std::istringstream(line) >> x1_ >> x2_ >> x3_ >> y1_ >> y2_ >> y3_ >> z1_ >> z2_ >> z3_;
            rotationmat.push_back(G4RotationMatrix(G4ThreeVector(x1_,x2_,x3_), G4ThreeVector(y1_,y2_,y3_), G4ThreeVector(z1_,z2_,z3_)));
        }
        myfile3.close();
    }
  
    
    
    // ---------------------------------------
    // Place masked detectors (192 slots)
    // ---------------------------------------
    
    // Loop through mask vector
    for (int i = 0; i < int(_mask.size()); i++){
        
        // Detctor ID are just numbers
        std::string detID = std::to_string(i+1);
        
        // if mask value is 1 then fill, if not leave empty
        if (_mask[i]){
        
            detector_pos = centers[i];
            G4Transform3D transform = G4Transform3D(rotationmat[i],detector_pos);
        
            G4PVPlacement *p = new G4PVPlacement(transform,               // the placement of the volume in center
                                                 detID,                   // name
                                                 logvols[i],              // the corresponding logical volume -- gives
                                                                          //   volume the material (and the dimensions
                                                                          //   via the solid assigned to the logical volume)
                                                 worldPhys,               // inside the world so the world is the mother
                                                                          //   physical volume,
                                                 false,                   // many = false (no copies)
                                                 0                        // the replica id number (only > 0 if copies exist)
                                                 );
            
            // Check for overlapping geometry
            if (_checkoverlaps){
                p->CheckOverlaps();
            }
        }
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
    
    G4VisAttributes* world_vis_att = new G4VisAttributes(G4Color(1.,0.,0.));
    world_vis_att->SetForceWireframe(true);
    world_vis_att->SetVisibility(true);
    worldLog->SetVisAttributes(world_vis_att);
    

    
    return worldPhys;
}

//==================================================================================================

void DetectorConstruction::ConstructSDandField(){
    
    // Sensitive detectors
    G4String SDname = "PRISM_SIM/SD";
    SensitiveDetector* SD = new SensitiveDetector(SDname,"fHitsCollection");
    SetSensitiveDetector("DetectorLog", SD, true);
    
}

//==================================================================================================


std::vector<G4int> DetectorConstruction::GetRandomMask(){
    
    std::vector<G4int> mask;
    G4double rand;
    
    // Get random number, convert to 1 or 0, fill mask array
    for (int q = 0; q < 192; q++){
        rand = G4UniformRand();
        if (rand > 0.5){mask.push_back(1);}
        else {mask.push_back(0);}
    }
    
    return mask;
}

//==================================================================================================

void DetectorConstruction::SetMask(std::vector<G4int> inputmask){
    
    _mask = inputmask;
}

//==================================================================================================

G4String DetectorConstruction::BinToHex(std::vector<G4int> mask){
    
    // Digit to hex table
    const char *hex_dig[] = {"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};
    
    G4String hex_;
    
    // Loop through mask arry, convert chucks of 4 into respective hex form, append to string
    for (int q = 0; q < int(mask.size()/4); q++){
        int idx = 8*mask[0+(q*4)] + 4*mask[1+(q*4)] + 2*mask[2+(q*4)] + mask[3+(q*4)];
        hex_.append(hex_dig[idx]);
    }

    return hex_;
}

//==================================================================================================

std::vector<G4int> DetectorConstruction::HexToBin(G4String hex_){
    
    std::vector<G4int> mask;

    // Loop through hex string and create a binary string with hex to binary table
    G4String mask_string = "";
    for (int i = 0; i < int(hex_.length()); ++i)
    {
        switch (hex_[i])
        {
            case '0': mask_string.append ("0000"); break;
            case '1': mask_string.append ("0001"); break;
            case '2': mask_string.append ("0010"); break;
            case '3': mask_string.append ("0011"); break;
            case '4': mask_string.append ("0100"); break;
            case '5': mask_string.append ("0101"); break;
            case '6': mask_string.append ("0110"); break;
            case '7': mask_string.append ("0111"); break;
            case '8': mask_string.append ("1000"); break;
            case '9': mask_string.append ("1001"); break;
            case 'a': mask_string.append ("1010"); break;
            case 'b': mask_string.append ("1011"); break;
            case 'c': mask_string.append ("1100"); break;
            case 'd': mask_string.append ("1101"); break;
            case 'e': mask_string.append ("1110"); break;
            case 'f': mask_string.append ("1111"); break;
            case 'A': mask_string.append ("1010"); break;
            case 'B': mask_string.append ("1011"); break;
            case 'C': mask_string.append ("1100"); break;
            case 'D': mask_string.append ("1101"); break;
            case 'E': mask_string.append ("1110"); break;
            case 'F': mask_string.append ("1111"); break;
        }
    }
    
    // Convert string into an int array
    for (int i = 0; i < 192; i++){mask.push_back((G4int)mask_string[i]-48);}
    
    return mask;
}
    
//==================================================================================================

void DetectorConstruction::UpdateGeometry(){

    // figure out a way to delete the old sensitive detectors before reconstructing geo
    //
    //
    
    //G4RunManager::GetRunManager()->DefineWorldVolume(ConstructWorld());
    G4RunManager::GetRunManager()->ReinitializeGeometry();
    
}

//==================================================================================================

void DetectorConstruction::SetDetDim(G4ThreeVector dim){
    
    detector_dim = G4ThreeVector((dim.x()/2.)*CLHEP::cm, (dim.y()/2.)*CLHEP::cm, (dim.z()/2.)*CLHEP::cm);
}

//==================================================================================================

void DetectorConstruction::CheckOverlapsOn(){
    
    _checkoverlaps = true;
}

//==================================================================================================

std::vector<G4ThreeVector> DetectorConstruction::GetDetCenters(){
    
    return centers;
}

//==================================================================================================



