
#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithoutParameter.hh"

#include <istream>
#include <sstream>


//==================================================================================================

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction * detector)
: G4UImessenger(), fDetectorConstruction(detector)
{
    
    fDirectory = new G4UIdirectory("/PRISM_SIM/");
    fDirectory->SetGuidance("Top directory for sim");
    
    fDirectory = new G4UIdirectory("/PRISM_SIM/geom/");
    fDirectory->SetGuidance("Manipulate geometry");
    
    fSetMaskCmd = new G4UIcmdWithAString("/PRISM_SIM/geom/setMask",this);
    fSetMaskCmd->SetGuidance("Set the mask with hexidecimal code");
    fSetMaskCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fRandMaskCmd = new G4UIcmdWithoutParameter("/PRISM_SIM/geom/randomMask",this);
    fRandMaskCmd->SetGuidance("Generate random mask and update geometry");
    fRandMaskCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fFullMaskCmd = new G4UIcmdWithoutParameter("/PRISM_SIM/geom/fullMask",this);
    fFullMaskCmd->SetGuidance("Fully populate the mask and update geometry");
    fFullMaskCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fDetDimCmd = new G4UIcmdWith3Vector("/PRISM_SIM/geom/cubedim",this);
    fDetDimCmd->SetGuidance("Set the x,y,z lengths of the detectors (in cm) (full lengths)");
    fDetDimCmd->SetParameterName("xdim","ydim","zdim", false);
    fDetDimCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
}

//==================================================================================================

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
    delete fDirectory;
    delete fSetMaskCmd;
    delete fRandMaskCmd;
    delete fDetDimCmd;
}

//==================================================================================================

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    
    if (command == fSetMaskCmd) {
        
        std::vector<int> maskarray = fDetectorConstruction->HexToBin(newValue);
        fDetectorConstruction->SetMask(maskarray);
        fDetectorConstruction->UpdateGeometry();
    }
    
    else if (command == fRandMaskCmd) {
        
        std::vector<int> maskarray = fDetectorConstruction->GetRandomMask();
        fDetectorConstruction->SetMask(maskarray);
        fDetectorConstruction->UpdateGeometry();
    }
    
    else if (command == fFullMaskCmd) {
        
        std::vector<int> maskarray(192, 1);
        fDetectorConstruction->SetMask(maskarray);
        fDetectorConstruction->UpdateGeometry();
    }
    
    else if (command == fDetDimCmd) {

        fDetectorConstruction->SetDetDim(fDetDimCmd->GetNew3VectorValue(newValue));
        fDetectorConstruction->CheckOverlapsOn();
        fDetectorConstruction->UpdateGeometry();
    }
    
    
}

//==================================================================================================
