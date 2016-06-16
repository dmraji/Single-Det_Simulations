
#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"

#include <istream>
#include <sstream>


//==================================================================================================

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction * detector)
: G4UImessenger(), fDetectorConstruction(detector)
{
    
    fDirectory = new G4UIdirectory("/PRISM_SIM/");
    fDirectory->SetGuidance("UI commands specific to this example.");
    
}

//==================================================================================================

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
    delete fDirectory;
}

//==================================================================================================
