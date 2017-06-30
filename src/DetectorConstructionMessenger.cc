
#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithoutParameter.hh"

#include <istream>
#include <sstream>

using namespace std;

//==================================================================================================

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction * detector)
: G4UImessenger(), fDetectorConstruction(detector)
{

    fDirectory = new G4UIdirectory("/PRISM/");
    fDirectory->SetGuidance("Top directory for sim");

    fDirectory = new G4UIdirectory("/PRISM/geo/");
    fDirectory->SetGuidance("Manipulate geometry");

    fDetDimCmd = new G4UIcmdWith3Vector("/PRISM/geo/cubedim",this);
    fDetDimCmd->SetGuidance("Set the x,y,z lengths of the detector (in cm) (full lengths)");
    fDetDimCmd->SetParameterName("xdim","ydim","zdim", false);
    fDetDimCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//==================================================================================================

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
    delete fDirectory;
    delete fDetDimCmd;
}

//==================================================================================================

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{

    if (command == fDetDimCmd) {

        fDetectorConstruction->SetDetDim(fDetDimCmd->GetNew3VectorValue(newValue));
        fDetectorConstruction->UpdateGeometry();
    }

}

//==================================================================================================
