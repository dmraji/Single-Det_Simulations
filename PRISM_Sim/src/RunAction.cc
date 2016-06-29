#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"

// Analysis format header
#include "g4root.hh"
//#include "g4xml.hh"
//#include "g4csv.hh"



//==================================================================================================

RunAction* RunAction::fgInstance = 0;

//==================================================================================================

RunAction* RunAction::Instance() {
    
	return fgInstance;
}

//==================================================================================================

RunAction::RunAction() {
    
	fgInstance = this;
    
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();
    analysis->SetVerboseLevel(1);
    analysis->SetFileName("Output");

/*
    // 1D histograms
    analysis->CreateH1("Chamber1","Drift Chamber 1 # Hits", 50, 0., 50); // h1 Id = 0
    analysis->CreateH1("Chamber2","Drift Chamber 2 # Hits", 50, 0., 50); // h1 Id = 1
    
    // 2D histograms
    analysis->CreateH2("Chamber1 XY","Drift Chamber 1 X vs Y",50, -1000., 1000, 50, -300., 300.); // h2 Id = 0
    analysis->CreateH2("Chamber2 XY","Drift Chamber 2 X vs Y",50, -1500., 1500, 50, -300., 300.); // h2 Id = 1
*/
    
    // Create ntuples (switch to histograms??)
    analysis->CreateNtuple("dat", "dat");
    analysis->CreateNtupleIColumn("EvtN");
    analysis->CreateNtupleIColumn("HitNum");
    //analysis->CreateNtupleIColumn("TrackID");
    analysis->CreateNtupleDColumn("Time");
    analysis->CreateNtupleDColumn("Energy");
    analysis->CreateNtupleIColumn("DetID");
    analysis->CreateNtupleSColumn("Process");
    analysis->CreateNtupleSColumn("Prev_Process");
    analysis->CreateNtupleIColumn("DOI");
    analysis->CreateNtupleDColumn("Pos_X");
    analysis->CreateNtupleDColumn("Pos_Y");
    analysis->CreateNtupleDColumn("Pos_Z");
    analysis->CreateNtupleDColumn("Theta");
    analysis->CreateNtupleDColumn("Phi");
    analysis->FinishNtuple();


}

//==================================================================================================

RunAction::~RunAction() {
    
	fgInstance = 0;
}

//==================================================================================================

void RunAction::BeginOfRunAction(const G4Run* /*aRun*/) {

    // Get analysis manager
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();
    
    // Open an output file
    G4String fileName = "output/output";
    analysis->OpenFile(fileName);

}

//==================================================================================================

void RunAction::EndOfRunAction(const G4Run* /*aRun*/) {

    // Get analysis manager
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();

    // Write to file
    analysis->Write();
    analysis->CloseFile();

    // can only create one file per run...
    // we want to store information from multiple runs....
    // hmmmmm
    
}

//==================================================================================================


