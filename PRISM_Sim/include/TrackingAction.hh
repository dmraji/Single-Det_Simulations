#ifndef trackingaction_hh
#define trackingaction_hh 1

#include "globals.hh" 
#include "G4UserTrackingAction.hh"

class G4Track;



class TrackingAction : public G4UserTrackingAction
{
public:
	TrackingAction();
	~TrackingAction();

	static TrackingAction* Instance();


public:
	void PreUserTrackingAction(const G4Track*);
	void PostUserTrackingAction(const G4Track*);


private:
	static TrackingAction* fgInstance;


};

#endif
