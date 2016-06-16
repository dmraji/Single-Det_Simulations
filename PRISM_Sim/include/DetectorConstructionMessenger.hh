
#ifndef detectorconstructionnmessenger_hh
#define detectorconstructionnmessenger_hh 1

#include "globals.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;

class DetectorConstructionMessenger : public G4UImessenger {
    
public:
    DetectorConstructionMessenger(DetectorConstruction* detector);
    ~DetectorConstructionMessenger();
    
    
private:
    DetectorConstruction* fDetectorConstruction;
    
    G4UIdirectory* fDirectory;
    
};

#endif
