#include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"



//==================================================================================================

RunAction* RunAction::fgInstance = 0;

//==================================================================================================

RunAction* RunAction::Instance() {
    
	return fgInstance;
}

//==================================================================================================

RunAction::RunAction(): outputfilename("output") {
    
	fgInstance = this;

    /*
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();
    analysis->SetVerboseLevel(1);
    analysis->SetFileName("Output");

    // Create ntuples
    analysis->CreateNtuple("dat", "dat");
    analysis->CreateNtupleIColumn("EvtN");
    analysis->CreateNtupleIColumn("HitNum");
    //analysis->CreateNtupleIColumn("TrackID");
    analysis->CreateNtupleDColumn("Time");
    analysis->CreateNtupleDColumn("Energy");
    analysis->CreateNtupleIColumn("DetID");
    analysis->CreateNtupleSColumn("Process");
    analysis->CreateNtupleSColumn("Prev_Process");
    analysis->CreateNtupleIColumn("DOI");
    analysis->CreateNtupleDColumn("Pos_X");
    analysis->CreateNtupleDColumn("Pos_Y");
    analysis->CreateNtupleDColumn("Pos_Z");
    analysis->CreateNtupleDColumn("Theta");
    analysis->CreateNtupleDColumn("Phi");
    analysis->FinishNtuple();


    // Create 1D histograms
    analysis->CreateH1("DOI","Depth of Interaction", 20, 0., 10.*CLHEP::mm); // h1 Id = 0
    analysis->SetH1XAxisTitle(0, "DOI (mm)");
    analysis->SetH1YAxisTitle(0, "Counts");
    
    analysis->CreateH1("DetID","Detector ID", 192, 0.5, 192.5); // h1 Id = 1
    analysis->SetH1XAxisTitle(1, "Detector ID");
    analysis->SetH1YAxisTitle(1, "Counts");
    
    analysis->CreateH1("Energy","Incident Energy", 8000, 0., 3000.*CLHEP::keV); // h1 Id = 2
    analysis->SetH1XAxisTitle(2, "Energy (keV)");
    analysis->SetH1YAxisTitle(2, "Counts");
    */
    
}

//==================================================================================================

RunAction::~RunAction() {
    
	fgInstance = 0;
}

//==================================================================================================

void RunAction::BeginOfRunAction(const G4Run* /*aRun*/) {
    /*
    // Get analysis manager
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();
    
    
    // Open an output file
    G4String fileName = "output/output";
    analysis->OpenFile(fileName);
    */
    
    /*
    G4int hp = PrimaryGeneratorAction::Instance()->GetHP_index();
    if (hp%300 == 0){G4cout << "[HEALPix index = " << hp << "]\n";}
    */
}

//==================================================================================================

void RunAction::EndOfRunAction(const G4Run* /*aRun*/) {
    
    /*
    // Get analysis manager
    G4AnalysisManager* analysis = G4AnalysisManager::Instance();
    
    // Write to file
    G4cout << "Writing to file...\n";
    analysis->Write();
    analysis->CloseFile();
    */

    PrintToBinaryFile();
    ClearTuples();

}

//==================================================================================================


void RunAction::FillDOItuple(G4double doi) { DOItuple.push_back(doi); }

void RunAction::FillDOIbintuple(G4int doi) { DOIbintuple.push_back(doi); }

void RunAction::FillDetIDtuple(G4int detid) { DetIDtuple.push_back(detid); }

void RunAction::FillEvtNtuple(G4int evtN) { EvtNtuple.push_back(evtN); }

void RunAction::FillHitNumtuple(G4int hitnum) { HitNumtuple.push_back(hitnum); }

void RunAction::FillTrackIDtuple(G4int trackid) { TrackIDtuple.push_back(trackid); }

void RunAction::FillTimetuple(G4double time) { Timetuple.push_back(time); }

void RunAction::FillEnergytuple(G4double energy) { Energytuple.push_back(energy); }

void RunAction::FillProcesstuple(G4String proc) { Processtuple.push_back(proc); }

void RunAction::FillPrevProcesstuple(G4String prevproc) { PrevProcesstuple.push_back(prevproc); }

void RunAction::FillPositiontuple(G4ThreeVector pos) { Positiontuple.push_back(pos); }

void RunAction::FillThetatuple(G4double theta) { Thetatuple.push_back(theta); }

void RunAction::FillPhituple(G4double phi) { Phituple.push_back(phi); }

void RunAction::FillHPindextuple(G4int hpindex) { HPindextuple.push_back(hpindex); }

//==================================================================================================

void RunAction::ClearDOItuple() { DOItuple.clear(); }

void RunAction::ClearDOIbintuple() { DOIbintuple.clear(); }

void RunAction::ClearDetIDtuple() { DetIDtuple.clear(); }

void RunAction::ClearEvtNtuple() { EvtNtuple.clear(); }

void RunAction::ClearHitNumtuple() { HitNumtuple.clear(); }

void RunAction::ClearTrackIDtuple() { TrackIDtuple.clear(); }

void RunAction::ClearTimetuple() { Timetuple.clear(); }

void RunAction::ClearEnergytuple() { Energytuple.clear(); }

void RunAction::ClearProcesstuple() { Processtuple.clear(); }

void RunAction::ClearPrevProcesstuple() { PrevProcesstuple.clear(); }

void RunAction::ClearPositiontuple() { Positiontuple.clear(); }

void RunAction::ClearThetatuple() { Thetatuple.clear(); }

void RunAction::ClearPhituple() { Phituple.clear(); }

void RunAction::ClearHPindextuple() { HPindextuple.clear(); }

//==================================================================================================

std::vector<G4double> RunAction::GetDOItuple() { return DOItuple; }

std::vector<G4int> RunAction::GetDOIbintuple() { return DOIbintuple; }

std::vector<G4int> RunAction::GetDetIDtuple() { return DetIDtuple; }

std::vector<G4int> RunAction::GetEvtNtuple() { return EvtNtuple; }

std::vector<G4int> RunAction::GetHitNumtuple() { return HitNumtuple; }

std::vector<G4int> RunAction::GetTrackIDtuple() { return TrackIDtuple; }

std::vector<G4double> RunAction::GetTimetuple() { return Timetuple; }

std::vector<G4double> RunAction::GetEnergytuple() { return Energytuple; }

std::vector<G4String> RunAction::GetProcesstuple() { return Processtuple; }

std::vector<G4String> RunAction::GetPrevProcesstuple() { return PrevProcesstuple; }

std::vector<G4ThreeVector> RunAction::GetPositiontuple() { return Positiontuple; }

std::vector<G4double> RunAction::GetThetatuple() { return Thetatuple; }

std::vector<G4double> RunAction::GetPhituple() { return Phituple; }

std::vector<G4int> RunAction::GetHPindextuple() { return HPindextuple; }

//==================================================================================================

void RunAction::PrintToTextFile(){
    
    G4String filename = "output/";
    filename.append(GetOutputFilename());
    filename.append(".txt");
    
    //G4cout << "\n\nWriting data to " << filename << "...\n\n";

    ofstream myfile;
    myfile.open (filename, ios::out | ios::app);
    
    for (int i = 0; i < int(EvtNtuple.size()); i++){
        myfile
        << int(EvtNtuple[i]) << "\t"
        << int(HitNumtuple[i]) << "\t"
        //<< TrackIDtuple[i] << "\t"
        //<< Timetuple[i] << "\t"
        << int(Energytuple[i]) << "\t"
        << int(DetIDtuple[i]) << "\t"
        //<< Processtuple[i] << "\t"
        //<< PrevProcesstuple[i] << "\t"
        //<< DOItuple[i] << "\t"
        << int(DOIbintuple[i]) << "\t"
        //<< Positiontuple[i] << "\t"
        //<< Thetatuple[i] << "\t"
        //<< int(Phituple[i]) << "\t"
        << int(HPindextuple[i]) << "\t"
        << "\n";
    }
    myfile.close();
}

//==================================================================================================   
 
void RunAction::PrintToBinaryFile(){
    
    G4String filename = "output/";
    filename.append(GetOutputFilename());
    filename.append(".bin");
    
    //G4cout << "\n\nWriting data to " << filename << "...\n\n";
    
    ofstream myfile2;
    myfile2.open (filename, ios::out | ios::app | ios::binary);
    
    G4int energybin;
    
    for (int i = 0; i < int(EvtNtuple.size()); i++){
        
        energybin = int(Energytuple[i]);
        
        myfile2.write(reinterpret_cast<char*>(&EvtNtuple[i]), 4);
        myfile2.write(reinterpret_cast<char*>( &HitNumtuple[i]), 1);
        //myfile2.write(reinterpret_cast<char*>(&TrackIDtuple[i]), sizeof(TrackIDtuple[i]));
        //myfile2.write(reinterpret_cast<char*>(&Timetuple[i]), sizeof(Timetuple[i]));
        myfile2.write(reinterpret_cast<char*>(&energybin), 2);
        myfile2.write(reinterpret_cast<char*>(&DetIDtuple[i]), 1);
        //myfile2.write(reinterpret_cast<char*>(&Processtuple[i]), sizeof(Processtuple[i]));
        //myfile2.write(reinterpret_cast<char*>(&PrevProcesstuple[i]), sizeof(PrevProcesstuple[i]));
        myfile2.write(reinterpret_cast<char*>(&DOIbintuple[i]), 1);
        //myfile2.write(reinterpret_cast<char*>(&Positiontuple[i]), sizeof(Positiontuple[i]));
        //myfile2.write(reinterpret_cast<char*>(&Thetatuple[i]), sizeof(Thetatuple[i]));
        //myfile2.write(reinterpret_cast<char*>(&Phituple[i]), sizeof(Phituple[i]));
        myfile2.write(reinterpret_cast<char*>(&HPindextuple[i]), 2);
    }
    myfile2.close();
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
