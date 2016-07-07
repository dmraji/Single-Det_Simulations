#ifndef runaction_hh
#define runaction_hh 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"
#include <vector>

using namespace std;

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
    
    
    // -----------------------------------------------
    // Analysis tuples
    
    // Depth of interaction
    vector<G4double> DOItuple;
    void FillDOItuple(G4double);
    void ClearDOItuple();
    vector<G4double> GetDOItuple();
    
    // Depth of interaction (binned)
    vector<G4int> DOIbintuple;
    void FillDOIbintuple(G4int);
    void ClearDOIbintuple();
    vector<G4int> GetDOIbintuple();
    
    // Detector ID
    vector<G4int> DetIDtuple;
    void FillDetIDtuple(G4int);
    void ClearDetIDtuple();
    vector<G4int> GetDetIDtuple();
    
    // Event Number
    vector<G4int> EvtNtuple;
    void FillEvtNtuple(G4int);
    void ClearEvtNtuple();
    vector<G4int> GetEvtNtuple();
    
    // Hit Number
    vector<G4int> HitNumtuple;
    void FillHitNumtuple(G4int);
    void ClearHitNumtuple();
    vector<G4int> GetHitNumtuple();
    
    // Track ID
    vector<G4int> TrackIDtuple;
    void FillTrackIDtuple(G4int);
    void ClearTrackIDtuple();
    vector<G4int> GetTrackIDtuple();
    
    // Local Time
    vector<G4double> Timetuple;
    void FillTimetuple(G4double);
    void ClearTimetuple();
    vector<G4double> GetTimetuple();
    
    // Energy
    vector<G4float> Energytuple;
    void FillEnergytuple(G4float);
    void ClearEnergytuple();
    vector<G4float> GetEnergytuple();
    
    // Process
    vector<G4String> Processtuple;
    void FillProcesstuple(G4String);
    void ClearProcesstuple();
    vector<G4String> GetProcesstuple();
    
    // Previous Process
    vector<G4String> PrevProcesstuple;
    void FillPrevProcesstuple(G4String);
    void ClearPrevProcesstuple();
    vector<G4String> GetPrevProcesstuple();
    
    // Position
    vector<G4ThreeVector> Positiontuple;
    void FillPositiontuple(G4ThreeVector);
    void ClearPositiontuple();
    vector<G4ThreeVector> GetPositiontuple();
    
    // Theta
    vector<G4double> Thetatuple;
    void FillThetatuple(G4double);
    void ClearThetatuple();
    vector<G4double> GetThetatuple();
    
    // Phi
    vector<G4double> Phituple;
    void FillPhituple(G4double);
    void ClearPhituple();
    vector<G4double> GetPhituple();
    
    // HP Index
    vector<G4int> HPindextuple;
    void FillHPindextuple(G4int);
    void ClearHPindextuple();
    vector<G4int> GetHPindextuple();
    
    // Clear all tuples
    void ClearTuples();
    
    // -----------------------------------------------

    // Print to file
    void PrintToTextFile();
    G4bool printtext;
    inline void SetPrintText(G4bool ww){printtext = ww;}
    inline G4bool GetPrintText(){return printtext;}
    
    void PrintToBinaryFile();
    G4bool printbin;
    inline void SetPrintBinary(G4bool qq){printbin = qq;}
    inline G4bool GetPrintBinary(){return printbin;}

    G4String outputfilename;
    inline void SetOutputFilename(G4String fn){outputfilename = fn;}
    inline G4String GetOutputFilename(){return outputfilename;}
    

private:
	static RunAction* fgInstance;
    
};

#endif
