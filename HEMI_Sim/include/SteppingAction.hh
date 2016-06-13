#ifndef steppingaction_hh
#define steppingaction_hh 1

#include "globals.hh" 
#include "G4UserSteppingAction.hh"

#include "TH3.h"

class G4Step;



class SteppingAction : public G4UserSteppingAction
{
public:
	SteppingAction();
	~SteppingAction();

	static SteppingAction* Instance();


public:
	void UserSteppingAction(const G4Step*);
    
    // define backplane matrix, intially all zeros
    int backplane_array[8][8] = {{0}};
    
    // Total response matrix for theta, phi, and detector ID
    TH3D * totalresponse = new TH3D("totalresponse", "totalresponse", 61, -30.5, 30.5, 61, -30.5, 30.5, 64, 0.5, 64.5);
    inline TH3D * GetTotalResponseHist(){return totalresponse;}

private:
	static SteppingAction* fgInstance;




};

#endif
