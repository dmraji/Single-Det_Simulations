#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "g4root.hh"
#include "G4Run.hh"



//==================================================================================================

RunAction* RunAction::fgInstance = 0;

//==================================================================================================

RunAction* RunAction::Instance() {
    
	return fgInstance;
}

//==================================================================================================

RunAction::RunAction() {
    
	fgInstance = this;
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFileName("Output");

/*
    // 1D histograms
    analysisManager->CreateH1("Chamber1","Drift Chamber 1 # Hits", 50, 0., 50); // h1 Id = 0
    analysisManager->CreateH1("Chamber2","Drift Chamber 2 # Hits", 50, 0., 50); // h1 Id = 1
    
    // 2D histograms
    analysisManager->CreateH2("Chamber1 XY","Drift Chamber 1 X vs Y",50, -1000., 1000, 50, -300., 300.); // h2 Id = 0
    analysisManager->CreateH2("Chamber2 XY","Drift Chamber 2 X vs Y",50, -1500., 1500, 50, -300., 300.); // h2 Id = 1
*/
    
    // ntuples
    analysisManager->CreateNtuple("Output", "Hits");
    analysisManager->CreateNtupleIColumn("EventNum");
    analysisManager->CreateNtupleIColumn("HitNum");
    analysisManager->CreateNtupleDColumn("Time");
    analysisManager->CreateNtupleDColumn("Energy");
    analysisManager->CreateNtupleIColumn("DetID");
    analysisManager->CreateNtupleIColumn("Process");                 // 0 = transportation, 1 = phot, 2 = rayl, 3 = compton
    analysisManager->CreateNtupleIColumn("Prev_Process");            // 0 = transportation, 1 = phot, 2 = rayl, 3 = compton
    analysisManager->CreateNtupleDColumn("DOI");
    //analysisManager->CreateNtupleDColumn(1,"Position");
    //analysisManager->CreateNtupleIColumn("TrackID");
    analysisManager->FinishNtuple();

    
}

//==================================================================================================

RunAction::~RunAction() {
    
	fgInstance = 0;
}

//==================================================================================================

void RunAction::BeginOfRunAction(const G4Run* /*aRun*/) {

    // Get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // Open an output file
    G4String fileName = "output/output";
    analysisManager->OpenFile(fileName);

}

//==================================================================================================

void RunAction::EndOfRunAction(const G4Run* /*aRun*/) {

    // Get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // Write to file
    analysisManager->Write();
    analysisManager->CloseFile();

    // can only create one file per run...
    // we want to store information from multiple runs....
    // hmmmmm
    
}

//==================================================================================================


