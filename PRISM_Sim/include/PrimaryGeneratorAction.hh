#ifndef primarygeneratoraction_hh
#define primarygeneratoraction_hh 1

#include "globals.hh" 
#include "G4VUserPrimaryGeneratorAction.hh"
#include "PrimaryGeneratorActionMessenger.hh"

#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"

class G4Event;
class G4ParticleGun;
class DetectorConstruction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	PrimaryGeneratorAction();
	~PrimaryGeneratorAction();

	static PrimaryGeneratorAction* Instance();
    
    PrimaryGeneratorActionMessenger * primarygeneratoractionmessenger;
    
    
public:
	void GeneratePrimaries(G4Event*);
    
    void SetTheta(G4double);
    void SetPhi(G4double);
    
    inline G4double GetTheta(){return theta;}
    inline G4double GetPhi(){return phi;}
    

protected:
    inline G4ThreeVector GetIsotropicMomentumDirection() const;
    inline G4ThreeVector GetHalfIsotropicMomentumDirection() const;
    inline G4ThreeVector GetConeMomentumDirection() const;
    
private:
	static PrimaryGeneratorAction* fgInstance;
    
    G4double theta;
    G4double phi;

    G4ParticleGun* gun;
    DetectorConstruction* detector;

};

//----------------------------------------------------------------------------//
inline G4ThreeVector PrimaryGeneratorAction::GetIsotropicMomentumDirection() const {
    
    G4double u = 2*G4UniformRand()-1;
    G4double v = 2.*CLHEP::pi*G4UniformRand();
    return G4ThreeVector(sqrt(1-u*u)*cos(v),
                         sqrt(1-u*u)*sin(v),
                         u);
}
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
inline G4ThreeVector PrimaryGeneratorAction::GetHalfIsotropicMomentumDirection() const {

    G4double u = G4UniformRand()-1;
    G4double v = 2.*CLHEP::pi*G4UniformRand();
    return G4ThreeVector(sqrt(1-u*u)*cos(v),
                         sqrt(1-u*u)*sin(v),
                         u);
}
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
inline G4ThreeVector PrimaryGeneratorAction::GetConeMomentumDirection() const {
    
    // open a cone of degree theta
    G4double _theta = 10.;
    
    G4double u = (-cos(_theta*CLHEP::pi/180)+1)*G4UniformRand()-1;
    G4double v = 2.*CLHEP::pi*G4UniformRand();
    return G4ThreeVector(sqrt(1-u*u)*cos(v),
                         sqrt(1-u*u)*sin(v),
                         u);
}
//----------------------------------------------------------------------------//


#endif
