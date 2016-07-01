
#include "PrimaryGeneratorActionMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

#include <istream>
#include <sstream>


//==================================================================================================

PrimaryGeneratorActionMessenger::PrimaryGeneratorActionMessenger(PrimaryGeneratorAction * generator)
: G4UImessenger(), fPrimaryGeneratorAction(generator)
{
    
    fDirectory = new G4UIdirectory("/PRISM_SIM/direction/");
    fDirectory->SetGuidance("Direction of incoming rays");
    
    fThetaCmd = new G4UIcmdWithADouble("/PRISM_SIM/direction/setTheta",this);
    fThetaCmd->SetGuidance("Set the theta of far field souce");
    fThetaCmd->SetParameterName("theta",false);
    fThetaCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fPhiCmd = new G4UIcmdWithADouble("/PRISM_SIM/direction/setPhi",this);
    fPhiCmd->SetGuidance("Set the phi of far field souce");
    fPhiCmd->SetParameterName("phi",false);
    fPhiCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fHPCmd = new G4UIcmdWithAnInteger("/PRISM_SIM/direction/setHPindex",this);
    fHPCmd->SetGuidance("Set the HEALPix index far field souce");
    fHPCmd->SetParameterName("HPindex",false);
    fHPCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fHPNsideCmd = new G4UIcmdWithAnInteger("/PRISM_SIM/direction/setHPNside",this);
    fHPNsideCmd->SetGuidance("Set nside for HEALPix (8 or 16)");
    fHPNsideCmd->SetParameterName("HPNside",false);
    fHPNsideCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fHPindexingCmd = new G4UIcmdWithAString("/PRISM_SIM/direction/setHPindexscheme",this);
    fHPindexingCmd->SetGuidance("Set indexing scheme for HEALPix (nested or ring)");
    fHPindexingCmd->SetParameterName("HPNside",false);
    fHPindexingCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fSetUpHEALPixCmd = new G4UIcmdWithoutParameter("/PRISM_SIM/direction/SetUpHEALPix",this);
    fSetUpHEALPixCmd->SetGuidance("Set up HEALPix (command to follow the Nside and IndexingScheme commands)");
    fSetUpHEALPixCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//==================================================================================================

PrimaryGeneratorActionMessenger::~PrimaryGeneratorActionMessenger()
{
    delete fThetaCmd;
    delete fPhiCmd;
    delete fHPCmd;
    delete fHPNsideCmd;
    delete fHPindexingCmd;
    delete fSetUpHEALPixCmd;
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
    
    else if (command == fHPCmd){
                
        G4int HPindex = fHPCmd->GetNewIntValue(newValue);
        
        std::vector<struct Angles> HPangles = fPrimaryGeneratorAction->GetHPangles();

        fPrimaryGeneratorAction->SetTheta(HPangles[HPindex].theta*(180./CLHEP::pi));
        fPrimaryGeneratorAction->SetPhi(HPangles[HPindex].phi*(180./CLHEP::pi));
        
        G4int nside = fPrimaryGeneratorAction->GetHPNside();
        G4int maxindex;
        if (nside == 8){maxindex = 768;}
        else if (nside == 16){maxindex = 3072;}
        else {maxindex = 0;}
        
        if (HPindex <= maxindex){
            fPrimaryGeneratorAction->SetHP_index(HPindex);
        }
        else { G4cerr << "\n\nThat HEALPix index is out of bounds for the current HPNside value... HPindex will be set to 0\n\n";}
                              
    }
    
    else if (command == fHPNsideCmd){
        
        G4int HPNsideindex = fHPCmd->GetNewIntValue(newValue);
        
        if (HPNsideindex == 8 || HPNsideindex == 16){
            fPrimaryGeneratorAction->SetHPNside(HPNsideindex);
        }
        
        else {G4cerr << "\n\nI do not recognize your command: /setHPNside " << HPNsideindex << "\nStopping...\n\n";}

    }
    
    else if (command == fHPindexingCmd){
        
        
        G4String HPindexing = newValue;
        
        if  (HPindexing == "nested" || HPindexing == "Nested" || HPindexing == "NESTED" ||
             HPindexing == "ring"   || HPindexing == "Ring"   || HPindexing == "RING"){
            
            fPrimaryGeneratorAction->SetHPindexing(HPindexing);
        }
        
        else {G4cerr << "\n\nI do not recognize your command: '/setHPindexscheme " << HPindexing << "'\nStopping...\n\n";}

    }
    
    else if (command == fSetUpHEALPixCmd){
        
        G4String filename;
        
        G4String indexing = fPrimaryGeneratorAction->GetHPindexing();
        G4int nside = fPrimaryGeneratorAction->GetHPNside();
        
        if (nside == 8 && (indexing == "nested" || indexing == "Nested" || indexing == "NESTED")){filename = "HEALpix/n=8/HEALPix_PhiTheta_Nested.txt";}
        if (nside == 8 && (indexing == "ring" || indexing == "Ring" || indexing == "RING")){filename = "HEALpix/n=8/HEALPix_PhiTheta_Ring.txt";}
        if (nside == 16 && (indexing == "nested" || indexing == "Nested" || indexing == "NESTED")){filename = "HEALpix/n=16/HEALPix_PhiTheta_Nested.txt";}
        if (nside == 16 && (indexing == "ring" || indexing == "Ring" || indexing == "RING")){filename = "HEALpix/n=16/HEALPix_PhiTheta_Ring.txt";}

        fPrimaryGeneratorAction->ReadInHEALPixAngles(filename);
        
    }
    
}

//==================================================================================================






