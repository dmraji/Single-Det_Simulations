#include "TrackingAction.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"


//==================================================================================================

TrackingAction* TrackingAction::fgInstance = 0;

//==================================================================================================

TrackingAction* TrackingAction::Instance() {
    
	return fgInstance;
}

//==================================================================================================

TrackingAction::TrackingAction() {
    
	fgInstance = this;
}

//==================================================================================================

TrackingAction::~TrackingAction() {
    
	fgInstance = 0;
}

//==================================================================================================

void TrackingAction::PreUserTrackingAction(const G4Track* /*aTrack*/) {}

//==================================================================================================

void TrackingAction::PostUserTrackingAction(const G4Track* /*aTrack*/) {}

//==================================================================================================