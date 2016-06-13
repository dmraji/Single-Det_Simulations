#include "PhysicsList.hh"



#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

// Hadrons
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4MesonConstructor.hh"

// Bosons
#include "G4BosonConstructor.hh"

// Leptons
#include "G4LeptonConstructor.hh"

// Other Particles
#include "G4ShortLivedConstructor.hh"

// Process options
#include "G4LossTableManager.hh"
#include "G4EmProcessOptions.hh"

// Physics List Helper
#include "G4PhysicsListHelper.hh"




//==================================================================================================

PhysicsList* PhysicsList::fgInstance = 0;

//==================================================================================================

PhysicsList* PhysicsList::Instance()
{
	return fgInstance;
}

//==================================================================================================

PhysicsList::PhysicsList()
{
	fgInstance = this;
}

//==================================================================================================

PhysicsList::~PhysicsList()
{
	fgInstance = 0;
}

//==================================================================================================

void PhysicsList::ConstructParticle()
{

}

//==================================================================================================

void PhysicsList::ConstructProcess()
{
    AddTransportation();
    
}

//==================================================================================================

void PhysicsList::SetCuts()
{

}

//==================================================================================================




