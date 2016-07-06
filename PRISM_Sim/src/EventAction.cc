#include "EventAction.hh"
#include "RunAction.hh"

#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4Event.hh"

#include "Hit.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "SensitiveDetector.hh"

using namespace CLHEP;

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
    
    // Fill all the tuples for the event
    FillTuples(evt);
    
}

//==================================================================================================

void EventAction::FillTuples(const G4Event* evt_){
    
    // Get Runaction - where all ntuples are stored
    RunAction * runaction = RunAction::Instance();
    
    // Get HitsCollection
    HitsCollection * HC = (HitsCollection*)(evt_->GetHCofThisEvent()->GetHC(HCollID));
    
    
    if ( HC ) {
        int n_hit = HC->entries();
        for (int i = 0 ; i < n_hit; i++){
            
            //only get photoabsorptions
            if ((*HC)[i]->GetProcess() == "phot"){
    
            runaction->FillEvtNtuple       (evt_->GetEventID());
            runaction->FillHitNumtuple     (i+1);
            runaction->FillTrackIDtuple    ((*HC)[i]->GetTrackID());
            runaction->FillTimetuple       ((*HC)[i]->GetTime()/ns);
            runaction->FillEnergytuple     ((*HC)[i]->GetEnergy()/keV);
            runaction->FillDetIDtuple      (atoi((*HC)[i]->GetVol()));
            runaction->FillProcesstuple    ((*HC)[i]->GetProcess());
            runaction->FillDOItuple        ((*HC)[i]->GetDOI()/mm);
            runaction->FillPositiontuple   ((*HC)[i]->GetPos());
            runaction->FillThetatuple      ((*HC)[i]->GetTheta());
            runaction->FillPhituple        ((*HC)[i]->GetPhi());
            runaction->FillHPindextuple    ((*HC)[i]->GetHPindex());
            
            
            
            // Bin the DOI values into 0.5mm wide bins
            G4double doi = (*HC)[i]->GetDOI();
            G4int doibin = 0;
            if      (doi <  0.5*mm)                       {doibin = 1; }   // These first two are the most common, put
            else if (doi >  9.5*mm)                       {doibin = 20;}   //   them at the beginning to avoid scanning all statements
            else if (doi >= 0.5*mm && doi < 1.0*mm){doibin = 2; }
            else if (doi >= 1.0*mm && doi < 1.5*mm){doibin = 3; }
            else if (doi >= 1.5*mm && doi < 2.0*mm){doibin = 4; }
            else if (doi >= 2.0*mm && doi < 2.5*mm){doibin = 5; }
            else if (doi >= 2.5*mm && doi < 3.0*mm){doibin = 6; }
            else if (doi >= 3.0*mm && doi < 3.5*mm){doibin = 7; }
            else if (doi >= 3.5*mm && doi < 4.0*mm){doibin = 8; }
            else if (doi >= 4.0*mm && doi < 4.5*mm){doibin = 9; }
            else if (doi >= 4.5*mm && doi < 5.0*mm){doibin = 10;}
            else if (doi >= 5.0*mm && doi < 5.5*mm){doibin = 11;}
            else if (doi >= 5.5*mm && doi < 6.0*mm){doibin = 12;}
            else if (doi >= 6.0*mm && doi < 6.5*mm){doibin = 13;}
            else if (doi >= 6.5*mm && doi < 7.0*mm){doibin = 14;}
            else if (doi >= 7.0*mm && doi < 7.5*mm){doibin = 15;}
            else if (doi >= 7.5*mm && doi < 8.0*mm){doibin = 16;}
            else if (doi >= 8.0*mm && doi < 8.5*mm){doibin = 17;}
            else if (doi >= 8.5*mm && doi < 9.0*mm){doibin = 18;}
            else if (doi >= 9.0*mm && doi < 9.5*mm){doibin = 19;}
            
            runaction->FillDOIbintuple(doibin);
            }
            
            
        }
    }
    
}

//==================================================================================================

