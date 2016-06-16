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
    
	fgInstance = this;
}

//==================================================================================================

EventAction::~EventAction() {
    
	fgInstance = 0;
}

//==================================================================================================

void EventAction::BeginOfEventAction(const G4Event* /*evt*/) {}

//==================================================================================================

void EventAction::EndOfEventAction(const G4Event* evt) {

    G4cout << ">>> Event " << evt->GetEventID() << "\n";
    
    // Print out everything
    G4VHitsCollection* hc = evt->GetHCofThisEvent()->GetHC(0);
    G4cout << "    " << hc->GetSize() << " hits stored in this event\n";
    hc->PrintAllHits();
    
    /*
    // Print out select things from the HitsCollection (this also shows how we access information)
    HitsCollection * HC;
    HC = (HitsCollection*)(hc);
    if ( HC ) {
        int n_hit = HC->entries();
        for ( int i = 0 ; i < n_hit; i++){
            G4cout << (*HC)[i]->GetProcess() << "\n";
            // ...
        }
    }
    */

}

//==================================================================================================
