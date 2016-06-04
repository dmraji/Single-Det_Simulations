#ifndef eventaction_hh
#define eventaction_hh 1

#include "globals.hh" 
#include "G4UserEventAction.hh"


class EventAction : public G4UserEventAction
{
public:
	EventAction();
	~EventAction();

	static EventAction* Instance();


public:
	void BeginOfEventAction(const G4Event*);
	void EndOfEventAction(const G4Event*);


private:
	static EventAction* fgInstance;


};

#endif
