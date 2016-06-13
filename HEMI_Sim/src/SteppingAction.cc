#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "RunAction.hh"
#include "G4Step.hh"




//==================================================================================================

SteppingAction* SteppingAction::fgInstance = 0;

//==================================================================================================

SteppingAction* SteppingAction::Instance()
{
	return fgInstance;
}

//==================================================================================================

SteppingAction::SteppingAction()
{
	fgInstance = this;
}

//==================================================================================================

SteppingAction::~SteppingAction()
{
	fgInstance = 0;
}

//==================================================================================================

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    
    
    G4Track* track = step->GetTrack();
    
    // Fill histograms
    //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Mask"){
        
        //analysisManager->FillH2(runnumber, step->GetPreStepPoint()->GetPhysicalVolume()->GetObjectTranslation().x()/CLHEP::cm, step->GetPreStepPoint()->GetPhysicalVolume()->GetObjectTranslation().y()/CLHEP::cm);
        
        //totalresponse->Fill(PrimaryGeneratorAction::Instance()->GetTheta(), PrimaryGeneratorAction::Instance()->GetPhi(), step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo());

        track->SetTrackStatus(fStopAndKill);

    }
  
    if (step->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Backplane"){
        
        //analysisManager->FillH2(runnumber+1, step->GetPreStepPoint()->GetPhysicalVolume()->GetObjectTranslation().x()/CLHEP::cm, step->GetPreStepPoint()->GetPhysicalVolume()->GetObjectTranslation().y()/CLHEP::cm);
        
        totalresponse->Fill(PrimaryGeneratorAction::Instance()->GetTheta(), PrimaryGeneratorAction::Instance()->GetPhi(), step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo());
        //individualresponse->Fill(PrimaryGeneratorAction::Instance()->GetTheta(), PrimaryGeneratorAction::Instance()->GetPhi(), step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo());

        track->SetTrackStatus(fStopAndKill);

    }
    
    
    
    
}

//==================================================================================================




