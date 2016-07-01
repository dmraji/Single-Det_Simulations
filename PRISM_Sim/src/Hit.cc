#include "Hit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

using namespace std;

G4ThreadLocal G4Allocator<Hit>* HitAllocator = 0;

//==================================================================================================

Hit::Hit()
: G4VHit(),
  fTrackID(-1),
  fHP(0),
  fEnergy(0.),
  fTheta(0.),
  fPhi(0.),
  fDOI(0.),
  fPos(G4ThreeVector()),
  fVol(G4String()),
  fProc(G4String())
{}

//==================================================================================================

Hit::~Hit() {}

//==================================================================================================

Hit::Hit(const Hit& right): G4VHit(){
    
    fTrackID   = right.fTrackID;
    fEnergy    = right.fEnergy;
    fPos       = right.fPos;
    fVol       = right.fVol;
    fProc      = right.fProc;
    fDOI       = right.fDOI;
    fTheta     = right.fTheta;
    fPhi       = right.fPhi;
    fHP        = right.fHP;
}

//==================================================================================================

const Hit& Hit::operator=(const Hit& right){
    
    fTrackID   = right.fTrackID;
    fEnergy    = right.fEnergy;
    fPos       = right.fPos;
    fVol       = right.fVol;
    fProc      = right.fProc;
    fDOI       = right.fDOI;
    fTheta     = right.fTheta;
    fPhi       = right.fPhi;
    fHP        = right.fHP;
    
    return *this;
}

//==================================================================================================

G4int Hit::operator==(const Hit& right) const{
    
    return ( this == &right ) ? 1 : 0;
}

//==================================================================================================
