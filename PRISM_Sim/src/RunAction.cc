#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"

using namespace CLHEP;
using namespace std;


//==================================================================================================

RunAction* RunAction::fgInstance = 0;

//==================================================================================================

RunAction* RunAction::Instance() {
    
	return fgInstance;
}

//==================================================================================================

RunAction::RunAction(): outputfilename("output") {
    
	fgInstance = this;
    
}

//==================================================================================================

RunAction::~RunAction() {
    
	fgInstance = 0;
}

//==================================================================================================

void RunAction::BeginOfRunAction(const G4Run* /*aRun*/) {
    
    /*
    G4int hp = PrimaryGeneratorAction::Instance()->GetHP_index();
    if (hp%300 == 0){G4cout << "[HEALPix index = " << hp << "]\n";}
    */
}

//==================================================================================================

void RunAction::EndOfRunAction(const G4Run* /*aRun*/) {
    
    // Dump data to file
    PrintToBinaryFile();
    PrintToTextFile();
    
    // Clear data tuples for next run
    ClearTuples();

}

//==================================================================================================
// Fill functions

void RunAction::FillDOItuple        (G4double doi)      {DOItuple.push_back(doi); }
void RunAction::FillDOIbintuple     (G4int doi)         {DOIbintuple.push_back(doi); }
void RunAction::FillDetIDtuple      (G4int detid)       {DetIDtuple.push_back(detid); }
void RunAction::FillEvtNtuple       (G4int evtN)        {EvtNtuple.push_back(evtN); }
void RunAction::FillHitNumtuple     (G4int hitnum)      {HitNumtuple.push_back(hitnum); }
void RunAction::FillTrackIDtuple    (G4int trackid)     {TrackIDtuple.push_back(trackid); }
void RunAction::FillTimetuple       (G4double time)     {Timetuple.push_back(time); }
void RunAction::FillEnergytuple     (G4float energy)    {Energytuple.push_back(energy); }
void RunAction::FillProcesstuple    (G4String proc)     {Processtuple.push_back(proc); }
void RunAction::FillPrevProcesstuple(G4String prevproc) {PrevProcesstuple.push_back(prevproc); }
void RunAction::FillPositiontuple   (G4ThreeVector pos) {Positiontuple.push_back(pos); }
void RunAction::FillThetatuple      (G4double theta)    {Thetatuple.push_back(theta); }
void RunAction::FillPhituple        (G4double phi)      {Phituple.push_back(phi); }
void RunAction::FillHPindextuple    (G4int hpindex)     {HPindextuple.push_back(hpindex); }

//==================================================================================================
// Clear functions

void RunAction::ClearDOItuple()         {DOItuple.clear(); }
void RunAction::ClearDOIbintuple()      {DOIbintuple.clear(); }
void RunAction::ClearDetIDtuple()       {DetIDtuple.clear(); }
void RunAction::ClearEvtNtuple()        {EvtNtuple.clear(); }
void RunAction::ClearHitNumtuple()      {HitNumtuple.clear(); }
void RunAction::ClearTrackIDtuple()     {TrackIDtuple.clear(); }
void RunAction::ClearTimetuple()        {Timetuple.clear(); }
void RunAction::ClearEnergytuple()      {Energytuple.clear(); }
void RunAction::ClearProcesstuple()     {Processtuple.clear(); }
void RunAction::ClearPrevProcesstuple() {PrevProcesstuple.clear(); }
void RunAction::ClearPositiontuple()    {Positiontuple.clear(); }
void RunAction::ClearThetatuple()       {Thetatuple.clear(); }
void RunAction::ClearPhituple()         {Phituple.clear(); }
void RunAction::ClearHPindextuple()     {HPindextuple.clear(); }

//==================================================================================================
// Get functions

vector<G4double>      RunAction::GetDOItuple()         {return DOItuple; }
vector<G4int>         RunAction::GetDOIbintuple()      {return DOIbintuple; }
vector<G4int>         RunAction::GetDetIDtuple()       {return DetIDtuple; }
vector<G4int>         RunAction::GetEvtNtuple()        {return EvtNtuple; }
vector<G4int>         RunAction::GetHitNumtuple()      {return HitNumtuple; }
vector<G4int>         RunAction::GetTrackIDtuple()     {return TrackIDtuple; }
vector<G4double>      RunAction::GetTimetuple()        {return Timetuple; }
vector<G4float>       RunAction::GetEnergytuple()      {return Energytuple; }
vector<G4String>      RunAction::GetProcesstuple()     {return Processtuple; }
vector<G4String>      RunAction::GetPrevProcesstuple() {return PrevProcesstuple; }
vector<G4ThreeVector> RunAction::GetPositiontuple()    {return Positiontuple; }
vector<G4double>      RunAction::GetThetatuple()       {return Thetatuple; }
vector<G4double>      RunAction::GetPhituple()         {return Phituple; }
vector<G4int>         RunAction::GetHPindextuple()     {return HPindextuple; }

//==================================================================================================

void RunAction::PrintToTextFile(){
    
    G4String filename = "output/";
    filename.append(GetOutputFilename());
    filename.append(".txt");
    
    ofstream myfile;
    myfile.open (filename, ios::out | ios::app);    // use append so we can write from mulitple runs
    
    for (int i = 0; i < int(EvtNtuple.size()); i++){
        myfile
        << EvtNtuple[i] << "\t"
        << HitNumtuple[i] << "\t"
        //<< TrackIDtuple[i] << "\t"
        //<< Timetuple[i] << "\t"
        << Energytuple[i] << "\t"
        << DetIDtuple[i] << "\t"
        << Processtuple[i] << "\t"
        //<< DOItuple[i] << "\t"
        << DOIbintuple[i] << "\t"
        //<< Positiontuple[i] << "\t"
        //<< Thetatuple[i] << "\t"
        //<< int(Phituple[i]) << "\t"
        << HPindextuple[i] << "\t"
        << "\n";
    }
    myfile.close();
}

//==================================================================================================   
 
void RunAction::PrintToBinaryFile(){
    
    G4String filename = "output/";
    filename.append(GetOutputFilename());
    filename.append(".bin");
    
    ofstream myfile;
    myfile.open (filename, ios::out | ios::app | ios::binary);   // use append so we can write from mulitple runs
    
    for (int i = 0; i < int(EvtNtuple.size()); i++){
        
        myfile.write(reinterpret_cast<char*>(&EvtNtuple[i]), 4);    // 4 Bytes (1 to 1e9)
        myfile.write(reinterpret_cast<char*>(&HitNumtuple[i]), 1);  // 1 Byte  (1 to 5)
        myfile.write(reinterpret_cast<char*>(&Energytuple[i]), 4);  // 4 Bytes (float)
        myfile.write(reinterpret_cast<char*>(&DetIDtuple[i]), 1);   // 1 Byte  (1 to 192)
        myfile.write(reinterpret_cast<char*>(&DOIbintuple[i]), 1);  // 1 Byte  (1 to 20)
        myfile.write(reinterpret_cast<char*>(&HPindextuple[i]), 2); // 2 Bytes (1 to 768 or 3072)
    
        //myfile.write(reinterpret_cast<char*>(&TrackIDtuple[i]), sizeof(TrackIDtuple[i]));
        //myfile.write(reinterpret_cast<char*>(&Timetuple[i]), sizeof(Timetuple[i]));
        //myfile.write(reinterpret_cast<char*>(&Processtuple[i]), sizeof(Processtuple[i]));
        //myfile.write(reinterpret_cast<char*>(&Positiontuple[i]), sizeof(Positiontuple[i]));
        //myfile.write(reinterpret_cast<char*>(&Thetatuple[i]), sizeof(Thetatuple[i]));
        //myfile.write(reinterpret_cast<char*>(&Phituple[i]), sizeof(Phituple[i]));
    
    }
    myfile.close();
}

//==================================================================================================

void RunAction::ClearTuples(){
    
    ClearDOItuple();
    ClearDOIbintuple();
    ClearDetIDtuple();
    ClearEvtNtuple();
    ClearHitNumtuple();
    ClearTrackIDtuple();
    ClearTimetuple();
    ClearEnergytuple();
    ClearProcesstuple();
    ClearPrevProcesstuple();
    ClearPositiontuple();
    ClearThetatuple();
    ClearPhituple();
    ClearHPindextuple();
    
}

//==================================================================================================
