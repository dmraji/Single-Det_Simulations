#include "Hit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<Hit>* HitAllocator = 0;

//==================================================================================================

Hit::Hit()
: G4VHit(),
  fTrackID(-1),
  fEnergy(0.),
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
    //fDOI       = right.fDOI;
}

//==================================================================================================

const Hit& Hit::operator=(const Hit& right){
    
    fTrackID   = right.fTrackID;
    fEnergy    = right.fEnergy;
    fPos       = right.fPos;
    fVol       = right.fVol;
    fProc      = right.fProc;
    //fDOI       = right.fDOI;
    
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
    << "\t\tTrackID: "                    << fTrackID           << "\n"
    << "\t\tTime: "                       << fTime/CLHEP::ns    << " ns\n"
    << "\t\tInitial Energy: "             << fEnergy/CLHEP::keV << " keV\n"
    << "\t\tPosition: "                   << fPos/CLHEP::cm     << " cm\n"
    << "\t\tDetectorID: "                 << fVol               << "\n"
    << "\t\tProcess: "                    << fProc              << "\n"
    << "\t\tPrevious Process: "           << fPrevProc          << "\n";
    //<< "\t\tDepth of Interaction (DOI): " << fDOI               << "\n";

}

//================================================================================================

