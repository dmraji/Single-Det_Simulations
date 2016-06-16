#include "Hit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<Hit>* HitAllocator=0;

//==================================================================================================

Hit::Hit()
: G4VHit(),
  fTrackID(-1),
  fE(0.),
  fPos(G4ThreeVector()),
  fVol(G4String()),
  fProc(G4String())
{}

//==================================================================================================

Hit::~Hit() {}

//==================================================================================================

Hit::Hit(const Hit& right): G4VHit(){
    
    fTrackID   = right.fTrackID;
    fE         = right.fE;
    fPos       = right.fPos;
    fVol       = right.fVol;
    fProc      = right.fProc;
}

//==================================================================================================

const Hit& Hit::operator=(const Hit& right){
    
    fTrackID   = right.fTrackID;
    fE         = right.fE;
    fPos       = right.fPos;
    fVol       = right.fVol;
    fProc      = right.fProc;
    
    return *this;
}

//==================================================================================================

G4int Hit::operator==(const Hit& right) const{
    
    return ( this == &right ) ? 1 : 0;
}

//==================================================================================================

void Hit::Draw(){
    
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager)
    {
        G4Circle circle(fPos);
        circle.SetScreenSize(4.);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1.,0.,0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}

//==================================================================================================

void Hit::Print(){
    
    G4cout
    << "    TrackID: " << fTrackID << "\n"
    << "    E: " << std::setw(2) << G4BestUnit(fE,"Energy") << "\n"
    << "    Position: " << std::setw(2) << G4BestUnit( fPos,"Length") << "\n"
    << "    DetectorID: " << fVol << "\n"
    << "    Process: " << fProc << "\n";
}

//================================================================================================

