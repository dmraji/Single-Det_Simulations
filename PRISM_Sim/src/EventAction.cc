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
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();
    
    if ( HC ) {
        int n_hit = HC->entries();
        //G4cout << "\n>>> Event " << evt->GetEventID() << " [" << n_hit << " hit(s)]\n";
        for (int i = 0 ; i < n_hit; i++){
            //G4cout << "\tHit number: " << i+1 << "\n";
            
            //Print out everything
                //(*HC)[i]->Print();
            
            
            // Bin the DOI values into 0.5mm wide bins
            G4double doi = (*HC)[i]->GetDOI();
            G4int doibin = 0;
            if      (doi <  0.5*CLHEP::mm)                       {doibin = 1; }   // These first two are the most common, put
            else if (doi >  9.5*CLHEP::mm)                       {doibin = 20;}   //   them at the beginning to avoid scanning all statements
            else if (doi >= 0.5*CLHEP::mm && doi < 1.0*CLHEP::mm){doibin = 2; }
            else if (doi >= 1.0*CLHEP::mm && doi < 1.5*CLHEP::mm){doibin = 3; }
            else if (doi >= 1.5*CLHEP::mm && doi < 2.0*CLHEP::mm){doibin = 4; }
            else if (doi >= 2.0*CLHEP::mm && doi < 2.5*CLHEP::mm){doibin = 5; }
            else if (doi >= 2.5*CLHEP::mm && doi < 3.0*CLHEP::mm){doibin = 6; }
            else if (doi >= 3.0*CLHEP::mm && doi < 3.5*CLHEP::mm){doibin = 7; }
            else if (doi >= 3.5*CLHEP::mm && doi < 4.0*CLHEP::mm){doibin = 8; }
            else if (doi >= 4.0*CLHEP::mm && doi < 4.5*CLHEP::mm){doibin = 9; }
            else if (doi >= 4.5*CLHEP::mm && doi < 5.0*CLHEP::mm){doibin = 10;}
            else if (doi >= 5.0*CLHEP::mm && doi < 5.5*CLHEP::mm){doibin = 11;}
            else if (doi >= 5.5*CLHEP::mm && doi < 6.0*CLHEP::mm){doibin = 12;}
            else if (doi >= 6.0*CLHEP::mm && doi < 6.5*CLHEP::mm){doibin = 13;}
            else if (doi >= 6.5*CLHEP::mm && doi < 7.0*CLHEP::mm){doibin = 14;}
            else if (doi >= 7.0*CLHEP::mm && doi < 7.5*CLHEP::mm){doibin = 15;}
            else if (doi >= 7.5*CLHEP::mm && doi < 8.0*CLHEP::mm){doibin = 16;}
            else if (doi >= 8.0*CLHEP::mm && doi < 8.5*CLHEP::mm){doibin = 17;}
            else if (doi >= 8.5*CLHEP::mm && doi < 9.0*CLHEP::mm){doibin = 18;}
            else if (doi >= 9.0*CLHEP::mm && doi < 9.5*CLHEP::mm){doibin = 19;}
            
            
            //
            // Fill analaysis ntuples
            //
            analysis->FillNtupleIColumn(0, evt->GetEventID());                    // Event number
            analysis->FillNtupleIColumn(1, i+1);                                  // Hit number
          //analysis->FillNtupleIColumn(2, (*HC)[i]->GetTrackID());               // Track ID
            analysis->FillNtupleDColumn(2, (*HC)[i]->GetTime()/CLHEP::ns);        // global time
            analysis->FillNtupleDColumn(3, (*HC)[i]->GetEnergy()/CLHEP::keV);     // energy
            analysis->FillNtupleIColumn(4, atoi((*HC)[i]->GetVol()));             // hit detector id
            analysis->FillNtupleSColumn(5, (*HC)[i]->GetProcess());               // interaction
            analysis->FillNtupleSColumn(6, (*HC)[i]->GetPrevProcess());           // previous interaction
          //analysis->FillNtupleIColumn(7, (*HC)[i]->GetDOI());                   // depth of interaction (raw value)
            analysis->FillNtupleIColumn(7, doibin);                               // depth of interaction (bin)
            analysis->FillNtupleDColumn(8, (*HC)[i]->GetPos().x());               // x position
            analysis->FillNtupleDColumn(9, (*HC)[i]->GetPos().y());               // y position
            analysis->FillNtupleDColumn(10,(*HC)[i]->GetPos().z());               // z position
            analysis->FillNtupleDColumn(11,(*HC)[i]->GetTheta());                 // y position
            analysis->FillNtupleDColumn(12,(*HC)[i]->GetPhi());                   // z position
            
            analysis->AddNtupleRow();
            
            }
        }
}

//==================================================================================================
