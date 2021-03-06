#include "SensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "Hit.hh"

using namespace CLHEP;

//==================================================================================

SensitiveDetector::SensitiveDetector(const G4String& name,const G4String& hitsCollectionName)
: G4VSensitiveDetector(name),
  fHitsCollection(NULL)
{
    collectionName.insert(hitsCollectionName);
}

//==================================================================================

SensitiveDetector::~SensitiveDetector(){}

//==================================================================================

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE){

    static int HCID = -1;

    // Create hits collection
    fHitsCollection= new HitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hce
    if(HCID < 0){HCID = GetCollectionID(0);}         //G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection(HCID, fHitsCollection);

}

//==================================================================================

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*){

    class Hit* newHit = new class Hit();

    // Track ID
    newHit->SetTrackID(aStep->GetTrack()->GetTrackID());

    // Deposited energy (assume all from gamma interaction - neglect electrons)
    //newHit->SetEnergy(G4float(aStep->GetPreStepPoint()->GetTotalEnergy()));
    newHit->SetEnergy(fabs(aStep->GetDeltaEnergy()));

    // Detector ID
    newHit->SetVol(atoi(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()));

    // Interaction Process
    G4String proc = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    if (proc == "phot"){
        newHit->SetProcess(1);
        // cout << "\nhey1\n";
    }
    else if (proc == "compt"){
        newHit->SetProcess(2);
        // cout << "\nhey2\n";
    }
    else if (proc == "Rayl"){
        newHit->SetProcess(3);
        // cout << "\nhey3\n";
    }
    else {
        newHit->SetProcess(4);
        // cout << "\nhey1\n";
    }

    // HEALPix index
    newHit->SetHPindex(PrimaryGeneratorAction::Instance()->GetHP_index());

    // Angles
    newHit->SetPhi(PrimaryGeneratorAction::Instance()->GetPhi());
    newHit->SetTheta(PrimaryGeneratorAction::Instance()->GetTheta());

    // Global time
    newHit->SetTime(aStep->GetPreStepPoint()->GetGlobalTime());

    // Depth of interaction
    //G4ThreeVector detcent = DetectorConstruction::Instance()->GetDetCenters()[atoi(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName()) + 1];
    G4ThreeVector detcent = G4ThreeVector(0.*cm, 0.*cm, 0.*cm);
    G4ThreeVector intpos = aStep->GetPostStepPoint()->GetPosition();
    G4double doi = intpos.z() + .5*cm;
    // /detcent.mag() + 0.500*cm;
    if      (doi < 0.) {newHit->SetDOI(0. *mm);}
    else if (doi > 10.){newHit->SetDOI(10.*mm);}
    else {newHit->SetDOI(doi);}

    // Add the newHit to the HitCollection
    fHitsCollection->insert( newHit );

    return true;
}

//==================================================================================

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*){}

//==================================================================================
