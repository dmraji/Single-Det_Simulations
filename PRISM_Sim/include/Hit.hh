#ifndef Hit_h
#define Hit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

using namespace std;

class Hit : public G4VHit
{
public:
    Hit();
    Hit(const Hit&);
    virtual ~Hit();
    
    // operators
    const Hit& operator=(const Hit&);
    G4int operator==(const Hit&) const;
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
    // methods from base class
    virtual void Draw();
    virtual void Print();
    
    // Set methods
    void SetTrackID     (G4int track)          {fTrackID = track;}
    void SetEnergy      (G4double de)          {fEnergy = de;}
    void SetPos         (G4ThreeVector xyz)    {fPos = xyz;}
    void SetVol         (G4String volname)     {fVol = volname;}
    void SetProcess     (G4String procname)    {fProc = procname;}
    void SetPrevProcess (G4String prevprocname){fPrevProc = prevprocname;}
    void SetTheta       (G4double theta)       {fTheta = theta;}
    void SetPhi         (G4double phi)         {fPhi = phi;}
    void SetTime        (G4double time)        {fTime = time;}
    void SetDOI         (G4double doi)         {fDOI = doi;}
    
    // Get methods
    G4int GetTrackID() const               {return fTrackID;}
    G4double GetEnergy() const             {return fEnergy;}
    G4ThreeVector GetPos() const           {return fPos;}
    G4String GetVol() const                {return fVol;}
    G4String GetProcess() const            {return fProc;}
    G4String GetPrevProcess() const        {return fPrevProc;}
    G4double GetTheta() const              {return fTheta;}
    G4double GetPhi() const                {return fPhi;}
    G4double GetTime() const               {return fTime;}
    G4double GetDOI() const                {return fDOI;}
    
private:
    
    G4int         fTrackID;
    G4double      fEnergy, fTheta, fPhi, fTime, fDOI;
    G4ThreeVector fPos;
    G4String      fVol, fProc, fPrevProc;
    
};


typedef G4THitsCollection<Hit> HitsCollection;

extern G4ThreadLocal G4Allocator<Hit>* HitAllocator;

inline void* Hit::operator new(size_t)
{
    if(!HitAllocator)
        HitAllocator = new G4Allocator<Hit>;
    return (void *) HitAllocator->MallocSingle();
}


inline void Hit::operator delete(void *hit)
{
    HitAllocator->FreeSingle((Hit*) hit);
}


#endif
