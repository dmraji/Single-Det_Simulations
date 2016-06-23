#include "SensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "PrimaryGeneratorAction.hh"

//==================================================================================

SensitiveDetector::SensitiveDetector(const G4String& name,const G4String& hitsCollectionName)
: G4VSensitiveDetector(name),
  fHitsCollection(NULL)
{
    collectionName.insert(hitsCollectionName);
}

//==================================================================================

SensitiveDetector::~SensitiveDetector()
{}

//==================================================================================

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
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
    
    newHit->SetTrackID     (aStep->GetTrack()->GetTrackID());
    newHit->SetEnergy      (aStep->GetPreStepPoint()->GetTotalEnergy());
    newHit->SetPos         (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetVol         (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName());
    newHit->SetProcess     (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
    newHit->SetPrevProcess (aStep->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName());
    newHit->SetTheta       (PrimaryGeneratorAction::Instance()->GetTheta());
    newHit->SetPhi         (PrimaryGeneratorAction::Instance()->GetPhi());
    newHit->SetTime        (aStep->GetPreStepPoint()->GetLocalTime());
    //newHit->SetDOI         (aStep->GetPreStepPoint()->GetPosition());

    fHitsCollection->insert( newHit );
    
    //newHit->Print();
    
    return true;
}

//==================================================================================

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    /*
    if ( verboseLevel >= 1 ) {
        G4int nofHits = fHitsCollection->entries();
        G4cout << "\n-------->Hits Collection: in this event they are " << nofHits
        << " hits in the tracker chambers: " << G4endl;
        for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
    }
    */
}

//==================================================================================

