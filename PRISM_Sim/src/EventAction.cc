#include "EventAction.hh"
#include "RunAction.hh"

#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4Event.hh"

#include "Hit.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "SensitiveDetector.hh"


//==================================================================================================

EventAction* EventAction::fgInstance = 0;

//==================================================================================================

EventAction* EventAction::Instance() {
    
	return fgInstance;
}

//==================================================================================================

EventAction::EventAction() {
    
    HCollID = -1;
	fgInstance = this;
}

//==================================================================================================

EventAction::~EventAction() {
    
	fgInstance = 0;
}

//==================================================================================================

void EventAction::BeginOfEventAction(const G4Event* /*evt*/) {

    G4SDManager * SDman = G4SDManager::GetSDMpointer();
    if(HCollID < 0){
        HCollID = SDman->GetCollectionID("fHitsCollection");
    }
    
}

//==================================================================================================

void EventAction::EndOfEventAction(const G4Event* evt) {
    
    // Get HitsCollection
    HitsCollection * HC = (HitsCollection*)(evt->GetHCofThisEvent()->GetHC(HCollID));

    // Get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4int tupleID;
    
    if ( HC ) {
        int n_hit = HC->entries();
        //G4cout << "\n>>> Event " << evt->GetEventID() << " [" << n_hit << " hit(s)]\n";
        for (int i = 0 ; i < n_hit; i++){
            //G4cout << "\tHit number: " << i+1 << "\n";
            
            //Print out everything
            //(*HC)[i]->Print();
            
            
            // Fill analaysismanager ntuples
            tupleID = 0;
            analysisManager->FillNtupleIColumn(tupleID,evt->GetEventID()); tupleID++;
            analysisManager->FillNtupleIColumn(tupleID,i+1); tupleID++;
            analysisManager->FillNtupleDColumn(tupleID,(*HC)[i]->GetTime()/CLHEP::ns); tupleID++;
            analysisManager->FillNtupleDColumn(tupleID,(*HC)[i]->GetEnergy()/CLHEP::keV); tupleID++;
            analysisManager->FillNtupleIColumn(tupleID,atoi((*HC)[i]->GetVol())); tupleID++;
            
            if      ((*HC)[i]->GetProcess() == "Transportation"){analysisManager->FillNtupleIColumn(tupleID,0);}
            else if ((*HC)[i]->GetProcess() == "phot"          ){analysisManager->FillNtupleIColumn(tupleID,1);}
            else if ((*HC)[i]->GetProcess() == "Rayl"          ){analysisManager->FillNtupleIColumn(tupleID,2);}
            else if ((*HC)[i]->GetProcess() == "compt"         ){analysisManager->FillNtupleIColumn(tupleID,3);} tupleID++;

            
            if      ((*HC)[i]->GetPrevProcess() == "Transportation"){analysisManager->FillNtupleIColumn(tupleID,0);}
            else if ((*HC)[i]->GetPrevProcess() == "phot"          ){analysisManager->FillNtupleIColumn(tupleID,1);}
            else if ((*HC)[i]->GetPrevProcess() == "Rayl"          ){analysisManager->FillNtupleIColumn(tupleID,2);}
            else if ((*HC)[i]->GetPrevProcess() == "compt"         ){analysisManager->FillNtupleIColumn(tupleID,3);} tupleID++;

            analysisManager->FillNtupleDColumn(tupleID,(*HC)[i]->GetDOI()/CLHEP::mm); tupleID++;
            //analysisManager->FillNtupleIColumn(tupleID,(*HC)[i]->GetTrackID()); tupleID++;
            analysisManager->AddNtupleRow();
            
            G4cout << "\t\tDepth of Interaction (DOI): "   << (*HC)[i]->GetDOI()/CLHEP::mm      << " mm\n";

            /*
             //Print out individually (also shows how to access individual hit information)
             G4cout
             << "\t\tTrackID: "                      << (*HC)[i]->GetTrackID()            << "\n"
             << "\t\tTime: "                         << (*HC)[i]->GetTime()/CLHEP::ns     << " ns\n"
             << "\t\tInitial Energy: "               << (*HC)[i]->GetEnergy()/CLHEP::keV  << " keV\n"
             << "\t\tPosition: "                     << (*HC)[i]->GetPos()/CLHEP::cm      << " cm\n"
             << "\t\tDetectorID: "                   << (*HC)[i]->GetVol()                << "\n"
             << "\t\tProcess: "                      << (*HC)[i]->GetProcess()            << "\n"
             << "\t\tPrevious Process: "             << (*HC)[i]->GetPrevProcess()        << "\n"
             << "\t\tDepth of Interaction (DOI): "   << (*HC)[i]->GetDOI()/CLHEP::mm      << " mm\n";
            */
            
            }
        }
}

//==================================================================================================
