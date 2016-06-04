#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"

//============================================================================//

ActionInitialization::ActionInitialization()

{

}

//============================================================================//

ActionInitialization::~ActionInitialization()
{  }

//============================================================================//

void ActionInitialization::BuildForMaster() const
{
    // all threads have same pointer to class instance
    SetUserAction(new RunAction);
}

//============================================================================//

void ActionInitialization::Build() const
{
    // for thread-local class instances
    SetUserAction(new PrimaryGeneratorAction);

    SetUserAction(new RunAction);

    SetUserAction(new EventAction);

    SetUserAction(new TrackingAction);

    SetUserAction(new SteppingAction);

}

//============================================================================//
