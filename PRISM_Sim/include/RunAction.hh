#ifndef runaction_hh
#define runaction_hh 1

#include "globals.hh" 
#include "g4root.hh"
#include "G4UserRunAction.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:
	RunAction();
	~RunAction();

	static RunAction* Instance();


public:
	void BeginOfRunAction(const G4Run*);
	void EndOfRunAction(const G4Run*);
    

private:
	static RunAction* fgInstance;
    
    std::vector<G4double> pos;

};

#endif
