#ifndef runaction_hh
#define runaction_hh 1

#include "globals.hh" 
//#include "g4root.hh"
//#include "g4csv.hh"
#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"
#include <vector>

class G4Run;

class RunAction : public G4UserRunAction
{
public:
	RunAction();
	~RunAction();

	static RunAction* Instance();


public:
	void BeginOfRunAction(const G4Run*);
	void EndOfRunAction(const G4Run*);
    
    
    
    
    // --------
    // Analysis
    
    // Depth of interaction
    std::vector<G4double> DOItuple;
    void FillDOItuple(G4double);
    void ClearDOItuple();
    std::vector<G4double> GetDOItuple();
    
    // Depth of interaction (binned)
    std::vector<G4int> DOIbintuple;
    void FillDOIbintuple(G4int);
    void ClearDOIbintuple();
    std::vector<G4int> GetDOIbintuple();
    
    // Detector ID
    std::vector<G4int> DetIDtuple;
    void FillDetIDtuple(G4int);
    void ClearDetIDtuple();
    std::vector<G4int> GetDetIDtuple();
    
    
    // Event Number
    std::vector<G4int> EvtNtuple;
    void FillEvtNtuple(G4int);
    void ClearEvtNtuple();
    std::vector<G4int> GetEvtNtuple();
    
    // Hit Number
    std::vector<G4int> HitNumtuple;
    void FillHitNumtuple(G4int);
    void ClearHitNumtuple();
    std::vector<G4int> GetHitNumtuple();
    
    // Track ID
    std::vector<G4int> TrackIDtuple;
    void FillTrackIDtuple(G4int);
    void ClearTrackIDtuple();
    std::vector<G4int> GetTrackIDtuple();
    
    // Local Time
    std::vector<G4double> Timetuple;
    void FillTimetuple(G4double);
    void ClearTimetuple();
    std::vector<G4double> GetTimetuple();
    
    // Energy
    std::vector<G4double> Energytuple;
    void FillEnergytuple(G4double);
    void ClearEnergytuple();
    std::vector<G4double> GetEnergytuple();
    
    // Process
    std::vector<G4String> Processtuple;
    void FillProcesstuple(G4String);
    void ClearProcesstuple();
    std::vector<G4String> GetProcesstuple();
    
    // Previous Process
    std::vector<G4String> PrevProcesstuple;
    void FillPrevProcesstuple(G4String);
    void ClearPrevProcesstuple();
    std::vector<G4String> GetPrevProcesstuple();
    
    // Position
    std::vector<G4ThreeVector> Positiontuple;
    void FillPositiontuple(G4ThreeVector);
    void ClearPositiontuple();
    std::vector<G4ThreeVector> GetPositiontuple();
    
    // Theta
    std::vector<G4double> Thetatuple;
    void FillThetatuple(G4double);
    void ClearThetatuple();
    std::vector<G4double> GetThetatuple();
    
    // Phi
    std::vector<G4double> Phituple;
    void FillPhituple(G4double);
    void ClearPhituple();
    std::vector<G4double> GetPhituple();
    
    // HP Index
    std::vector<G4int> HPindextuple;
    void FillHPindextuple(G4int);
    void ClearHPindextuple();
    std::vector<G4int> GetHPindextuple();
    
    
    // Print to file
    void PrintToTextFile();
    void PrintToBinaryFile();
    
    // Clear all tuples
    void ClearTuples();
    
    // -----

private:
	static RunAction* fgInstance;
    
};

#endif
