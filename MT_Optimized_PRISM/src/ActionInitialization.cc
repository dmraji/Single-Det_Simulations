#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "StackingAction.hh"

// ====================================================================================================

ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction){
}

// ====================================================================================================

ActionInitialization::~ActionInitialization(){}

// ====================================================================================================

void ActionInitialization::BuildForMaster() const{
    
  SetUserAction(new RunAction);
}

// ====================================================================================================

void ActionInitialization::Build() const{
    
  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction);
  SetUserAction(new EventAction);
  SetUserAction(new SteppingAction);
  SetUserAction(new StackingAction);

}

// ====================================================================================================
