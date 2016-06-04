
#include "PrimaryGeneratorActionMessenger.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"

#include <istream>
#include <sstream>


//==================================================================================================

PrimaryGeneratorActionMessenger::PrimaryGeneratorActionMessenger(PrimaryGeneratorAction * generator)
: G4UImessenger(), fPrimaryGeneratorAction(generator)
{
    
    fDirectory = new G4UIdirectory("/dir/");
    fDirectory->SetGuidance("UI commands specific to this example.");
    
    fThetaCmd = new G4UIcmdWithADouble("/dir/setTheta",this);
    fThetaCmd->SetGuidance("Set the theta of far field souce");
    fThetaCmd->SetParameterName("theta",false);
    fThetaCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fPhiCmd = new G4UIcmdWithADouble("/dir/setPhi",this);
    fPhiCmd->SetGuidance("Set the phi of far field souce");
    fPhiCmd->SetParameterName("phi",false);
    fPhiCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//==================================================================================================

PrimaryGeneratorActionMessenger::~PrimaryGeneratorActionMessenger()
{
    delete fThetaCmd;
    delete fPhiCmd;
    delete fDirectory;
}

//==================================================================================================

void PrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    
    if (command == fThetaCmd) {
        
        G4double theta = fThetaCmd->GetNewDoubleValue(newValue);
        fPrimaryGeneratorAction->SetTheta(theta);
        
    }
    
    else if (command == fPhiCmd){
        
        G4double phi = fPhiCmd->GetNewDoubleValue(newValue);
        fPrimaryGeneratorAction->SetPhi(phi);
        
    }
}

//==================================================================================================






