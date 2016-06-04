#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"

#include "G4RunManager.hh"
#include "g4root.hh"
#include "G4Run.hh"

#include "TROOT.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"


//==================================================================================================

RunAction* RunAction::fgInstance = 0;

//==================================================================================================

RunAction* RunAction::Instance() {
    
	return fgInstance;
}

//==================================================================================================

RunAction::RunAction() {
    
	fgInstance = this;
    
    /*
    // Create analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFirstHistoId(1);
    
    G4String fileName = "Results";
    analysisManager->OpenFile(fileName);
    */
}

//==================================================================================================

RunAction::~RunAction() {
    
	fgInstance = 0;
    
    delete G4AnalysisManager::Instance();
}

//==================================================================================================

void RunAction::BeginOfRunAction(const G4Run* /*aRun*/) {
    
    /*
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    int theta = PrimaryGeneratorAction::Instance()->GetTheta();
    int phi   = PrimaryGeneratorAction::Instance()->GetPhi();

    string name1 = "Mask_" + to_string(theta) + "_" + to_string(phi);
    analysisManager->CreateH2(name1, name1, 8, -4., 4., 8, -4., 4.);
    
    string name2 = "Backplane_" + to_string(theta) + "_" + to_string(phi);
    analysisManager->CreateH2(name2, name2, 8, -4., 4., 8, -4., 4.);
    */
    
}

//==================================================================================================

void RunAction::EndOfRunAction(const G4Run* /*aRun*/) {
    
    /*
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // Normalize hit histograms
    G4double norm = 1./analysisManager->GetH2(RunNumber)->get_entries();
    analysisManager->GetH2(RunNumber)->multiply(norm);
    norm = 1./analysisManager->GetH2(RunNumber+1)->get_entries();
    analysisManager->GetH2(RunNumber+1)->multiply(norm);
    
    */

    TH3D * individualresponse = new TH3D("individualresponse", "individualresponse", 61, -30.5, 30.5, 61, -30.5, 30.5, 64, 0.5, 64.5);
    individualresponse = SteppingAction::Instance()->GetIndividualResponseHist();
    TFile * f = new TFile("individualresponse.root","RECREATE");
    individualresponse->Write();
    f->Write();
    f->Close();
    //delete individualresponse;
    //SteppingAction::Instance()->ClearIndividualResponseHist();
    
    
    if (PrimaryGeneratorAction::Instance()->GetTheta() == 30 && PrimaryGeneratorAction::Instance()->GetPhi() == 30){
        //analysisManager->Write();
        //analysisManager->CloseFile();
        
        TH3D * response = new TH3D("response", "response", 61, -30.5, 30.5, 61, -30.5, 30.5, 64, 0.5, 64.5);
        response = SteppingAction::Instance()->GetTotalResponseHist();
        TFile * ff = new TFile("totalresponse.root","RECREATE");
        response->Write();
        ff->Write();
        ff->Close();
    }
    
}

//==================================================================================================




