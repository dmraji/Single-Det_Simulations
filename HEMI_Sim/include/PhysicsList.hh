#ifndef p1physicslist_hh
#define p1physicslist_hh 1



#include "globals.hh" 
#include "G4VUserPhysicsList.hh"




class PhysicsList : public G4VUserPhysicsList
{
public:
	PhysicsList();
	~PhysicsList();

	static PhysicsList* Instance();


public:
	void ConstructParticle();
	void ConstructProcess();
	void SetCuts();


private:
	static PhysicsList* fgInstance;




};

#endif
