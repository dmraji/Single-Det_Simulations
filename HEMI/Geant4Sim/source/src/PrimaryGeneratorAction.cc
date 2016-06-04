#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"


#include "DetectorConstruction.hh"

//==================================================================================================

PrimaryGeneratorAction* PrimaryGeneratorAction::fgInstance = 0;

//==================================================================================================

PrimaryGeneratorAction* PrimaryGeneratorAction::Instance()
{
	return fgInstance;
    
}

//==================================================================================================

PrimaryGeneratorAction::PrimaryGeneratorAction()
:  gun(0), detector(0)
{
	fgInstance = this;
    G4int nparticles = 1;
    gun = new G4ParticleGun(nparticles);

    G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = ptable->FindParticle("geantino");
    if(!particle) G4Exception("PrimaryGeneratorAction", "null pointer to particle",
                              FatalException, "Find particle did not return a particle");
    // Set the particle gun default to a 1 MeV geantino at (0, 0, 0) heading in
    // direction of (0, 0, 1)
    gun->SetParticleDefinition(particle);
    gun->SetParticlePosition(G4ThreeVector(0.*CLHEP::cm, 0.*CLHEP::cm, 0.5*CLHEP::m));
    gun->SetParticleEnergy(1.*CLHEP::MeV);
    gun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    
    
    theta = 0.;
    phi = 0.;
    
    // Create a new messenger class
    primarygeneratoractionmessenger = new PrimaryGeneratorActionMessenger(this);
    

}

//==================================================================================================

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    // delete the particle gun
    delete gun;
    // set your pointer to the geometry class to zero
    detector = 0;

	fgInstance = 0;
    
    // Delete messenger class
    delete primarygeneratoractionmessenger;
}

//==================================================================================================

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // The Instance() member function is a special use of a static member function and
    // static member variable -- in essence, it works if there is ONLY ONE
    // instance of the class -- this is called a "singleton"
    if(!detector) detector = DetectorConstruction::Instance();
    // you can then use this detector pointer to get the world dimensions,
    // the target position/dimensions etc.
    

    // Cone emission
      //gun->SetParticleMomentumDirection(this->GetConeMomentumDirection());
    
    
    // Far field source at angle to origin 
        G4double z0 = 30;
        G4double x0 = z0*sin(theta*(CLHEP::pi/180.));
        G4double y0 = z0*sin(phi*(CLHEP::pi/180.));
    
        G4double x = (x0 + 15. * (G4UniformRand()-0.5))*CLHEP::cm;
        G4double y = (y0 + 15. * (G4UniformRand()-0.5))*CLHEP::cm;
        G4double z = z0*CLHEP::cm;
    
        G4double px0 = -x0;
        G4double py0 = -y0;
        G4double pz0 = -z0;
    
    
    gun->SetParticlePosition(G4ThreeVector(x,y,z));
    gun->SetParticleMomentumDirection(G4ThreeVector(px0,py0,pz0));
    
    // Use the particle gun to define a primary particle for the event
    gun->GeneratePrimaryVertex(anEvent);
}

//==================================================================================================


//==================================================================================================

void PrimaryGeneratorAction::SetTheta(G4double theta_){

    theta = theta_;
    //G4cout << "Theta reset to " << theta << "\n";

}
//==================================================================================================


//==================================================================================================

void PrimaryGeneratorAction::SetPhi(G4double phi_){

    phi = phi_;
    //G4cout << "Phi reset to " << phi << "\n";

}

//==================================================================================================





