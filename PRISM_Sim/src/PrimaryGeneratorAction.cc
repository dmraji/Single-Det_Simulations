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

PrimaryGeneratorAction* PrimaryGeneratorAction::Instance() {
    
	return fgInstance;
    
}

//==================================================================================================

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), HPindexing("Ring"), HP_Nside(8), gun(0), detector(0) {
    
	fgInstance = this;
    G4int nparticles = 1;
    gun = new G4ParticleGun(nparticles);

    G4ParticleTable* ptable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = ptable->FindParticle("gamma");
    if(!particle) G4Exception("PrimaryGeneratorAction", "null pointer to particle",
                              FatalException, "Find particle did not return a particle");
    
    gun->SetParticleDefinition(particle);
    gun->SetParticlePosition(G4ThreeVector(0.*CLHEP::cm, 0.*CLHEP::cm, 0.5*CLHEP::m));
    gun->SetParticleEnergy(60.*CLHEP::keV);
    gun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    
    theta = 0.;
    phi = 0.;
    
    // Create a new messenger class
    primarygeneratoractionmessenger = new PrimaryGeneratorActionMessenger(this);
    
    // Pull in HEALPix angles
    ReadInHEALPixAngles("HEALpix/n=8/HEALPix_PhiTheta_Ring.txt");
    
}

//==================================================================================================

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    
    // delete the particle gun
    delete gun;
    // set your pointer to the geometry class to zero
    detector = 0;

	fgInstance = 0;
    
    // Delete messenger class
    delete primarygeneratoractionmessenger;
}

//==================================================================================================

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    
    // The Instance() member function is a special use of a static member function and
    // static member variable -- in essence, it works if there is ONLY ONE
    // instance of the class -- this is called a "singleton"
    if(!detector) detector = DetectorConstruction::Instance();
    // you can then use this detector pointer to get the world dimensions,
    // the target position/dimensions etc.
    

    // Cone emission
      //gun->SetParticleMomentumDirection(this->GetConeMomentumDirection());
    
    
    // - -
    // Far field source at angle to origin
    // - -
    
    // Set the direction of the rays, given theta and phi (calculate position, take all negative values)
    G4double r = 15.*CLHEP::cm;
    G4ThreeVector dir;
    dir.setX(-r*cos(theta*(CLHEP::pi/180))*sin(phi*(CLHEP::pi/180)));
    dir.setY(-r*sin(theta*(CLHEP::pi/180))*sin(phi*(CLHEP::pi/180)));
    dir.setZ(-r*cos(phi  *(CLHEP::pi/180)));

    // Uniformly sample a disk that just covers the entire detector (start at z-axis (0,0,1) and then rotate)
    G4double rand_r = G4UniformRand();
    G4double rand_theta = 2. * CLHEP::pi * G4UniformRand();
    G4double sphererad = 8.*CLHEP::cm;
    G4ThreeVector disk_pos(0);
    disk_pos.setX(sphererad * sqrt(rand_r) * cos(rand_theta));
    disk_pos.setY(sphererad * sqrt(rand_r) * sin(rand_theta));
    disk_pos.setZ(r);
    
    // Get position by rotating the z-oriented disk
    G4ThreeVector pos = disk_pos.rotateY(phi*CLHEP::deg).rotateZ(theta*CLHEP::deg);

    // Set results to the gun
    gun->SetParticlePosition(pos);
    gun->SetParticleMomentumDirection(dir);
    
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

void PrimaryGeneratorAction::SetHP_index(G4int hpindex){
    
    HP_index = hpindex;
}

//==================================================================================================

void PrimaryGeneratorAction::ReadInHEALPixAngles(G4String filename){
    
    HPangles.clear();
    
    std::ifstream myfile(filename);
    std::string line;
    if (myfile.is_open()){
        Angles ang;
        while (getline(myfile,line)){
            std::istringstream(line) >> ang.phi >> ang.theta;
            HPangles.push_back(ang);
        }
        myfile.close();
    }
    
}

//==================================================================================================


