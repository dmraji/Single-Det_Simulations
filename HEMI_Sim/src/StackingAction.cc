#include "StackingAction.hh"

#include "G4Track.hh"
#include "G4Electron.hh"

//==================================================================================

StackingAction::StackingAction(){}

//==================================================================================

StackingAction::~StackingAction(){}

//==================================================================================

G4ClassificationOfNewTrack

StackingAction::ClassifyNewTrack(const G4Track* track){
  
    //keep primary particle
    if (track->GetParentID() == 0){
        return fUrgent;
    }
  
  //kill secondary electrons
    if (track->GetDefinition() == G4Electron::Electron()){
        return fKill;
    }
    
    else {
      return fUrgent;
    }

}

//==================================================================================
