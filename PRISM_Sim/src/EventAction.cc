#include "EventAction.hh"

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
    
    G4bool indiv = false;

    /*
    // Print out everything
    G4VHitsCollection* hc = evt->GetHCofThisEvent()->GetHC(0);
    G4cout << ">>> Event " << evt->GetEventID() << "\n";
    G4cout << "    " << hc->GetSize() << " hits stored in this event\n";
    hc->PrintAllHits();
    */

    
    // Print out from the HitsCollection
    HitsCollection * HC = (HitsCollection*)(evt->GetHCofThisEvent()->GetHC(HCollID));
    if ( HC ) {
        int n_hit = HC->entries();
        G4cout << "\n>>> Event " << evt->GetEventID() << " [" << n_hit << " hit(s)]\n";
        for (int i = 0 ; i < n_hit; i++){
            G4cout << "\tHit number: " << i+1 << "\n";
            
            //Print out everything
            if(!indiv){(*HC)[i]->Print();}
            
            //Print out individually (also shows how to access individual hit information)
            else {
                G4cout
                << "\t\tTrackID: "          << (*HC)[i]->GetTrackID()            << "\n"
                << "\t\tTime: "             << (*HC)[i]->GetTime()/CLHEP::ns     << " ns\n"
                << "\t\tInitial Energy: "   << (*HC)[i]->GetEnergy()/CLHEP::keV  << " keV\n"
                << "\t\tPosition: "         << (*HC)[i]->GetPos()/CLHEP::cm      << " cm\n"
                << "\t\tDetectorID: "       << (*HC)[i]->GetVol()                << "\n"
                << "\t\tProcess: "          << (*HC)[i]->GetProcess()            << "\n"
                << "\t\tPrevious Process: " << (*HC)[i]->GetPrevProcess()        << "\n";
            }
        }
    }
    

}

//==================================================================================================
