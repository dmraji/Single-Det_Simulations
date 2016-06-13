#ifndef actioninitialization_hh_
#define actioninitialization_hh_

#include "globals.hh"
#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization
{
public:
	// Constructor and Destructors
	ActionInitialization();
	// Virtual destructors are required by abstract classes 
	// so add it by default, just in case
	virtual ~ActionInitialization();

public:
	// Public functions
    virtual void BuildForMaster() const;
    virtual void Build() const;

private:


};



//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//


#endif
