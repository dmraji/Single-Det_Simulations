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
    
    // Total response matrix for theta, phi, and detector ID
    TH3D * totalresponse = new TH3D("totalresponse", "totalresponse", 361, -180.5, 180.5, 181, -90.5, 90.5, 192, 0.5, 192.5);
    inline TH3D * GetTotalResponseHist(){return totalresponse;}

    int pe_num;
    int compt_num;
    inline int GetPENum(){return pe_num;}
    inline int GetComptonNum(){return compt_num;}


private:
	static SteppingAction* fgInstance;


};

#endif
