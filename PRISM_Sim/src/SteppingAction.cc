#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "RunAction.hh"
#include "G4Step.hh"


//==================================================================================================

SteppingAction* SteppingAction::fgInstance = 0;

//==================================================================================================

SteppingAction* SteppingAction::Instance() {
    
	return fgInstance;
}

//==================================================================================================

SteppingAction::SteppingAction() {
    
	fgInstance = this;
}

//==================================================================================================

SteppingAction::~SteppingAction() {
    
	fgInstance = 0;
}

//==================================================================================================

void SteppingAction::UserSteppingAction(const G4Step* step) {
    
    //G4Track* track = step->GetTrack();
    //track->SetTrackStatus(fStopAndKill);
    
    
    // If it hits detector and is a photoelectric absorption, tally
    if(atoi(step->GetPreStepPoint()->GetPhysicalVolume()->GetName()) && step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "phot"){
        pe_num++;
        totalresponse->Fill(PrimaryGeneratorAction::Instance()->GetTheta(), PrimaryGeneratorAction::Instance()->GetPhi(), atoi(step->GetPreStepPoint()->GetPhysicalVolume()->GetName()));
        
        //search track to see if it had a compton scatter first...

    }

    else if(atoi(step->GetPreStepPoint()->GetPhysicalVolume()->GetName()) && step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "compt"){
        compt_num++;
        //G4cout << "Comton scatter\n";
    }
    
    
    
    
}

//==================================================================================================




