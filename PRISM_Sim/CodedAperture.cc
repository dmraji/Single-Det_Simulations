// Geant4 header files
#ifdef G4MULTITHREADED
    #include "G4MTRunManager.hh"
    typedef G4MTRunManager TheRunManager;
#else
    #include "G4RunManager.hh"
    typedef G4RunManager TheRunManager;
#endif

#include "Randomize.hh"
#include "G4UImanager.hh"

#ifdef G4VIS_USE
    #include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
    #include "G4UIExecutive.hh"
#endif





// User Defined headers for classes
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"
typedef PhysicsList ThePhysicsList;

#include "SteppingAction.hh"
#include "TFile.h"
#include "TH3.h"


/*
#ifdef CUSTOM_PHYSICS_LIST
    #include "PhysicsList.hh"
    typedef PhysicsList ThePhysicsList;
#else
    #include "QGSP_BERT.hh"
    typedef QGSP_BERT ThePhysicsList;
#endif
*/
 
int main(int argc, char** argv)
{
    
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

	TheRunManager* runManager = new TheRunManager();

	runManager->SetUserInitialization(new DetectorConstruction);
    
	runManager->SetUserInitialization(new ThePhysicsList);

    runManager->SetUserInitialization(new ActionInitialization);
   
    
	runManager->Initialize();
    
    

#ifdef G4VIS_USE
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif

	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if(argc != 1) {
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	} else {
#ifdef G4UI_USE
		G4UIExecutive* ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
		UImanager->ApplyCommand("/control/execute macros/vis.mac");
#endif
		ui->SessionStart();
		delete ui;
#endif
	}

    
// Write response to file
TH3D * response = new TH3D("response", "response", 361, -180.5, 180.5, 181, -90.5, 90.5, 192, 0.5, 192.5);
response = SteppingAction::Instance()->GetTotalResponseHist();
TFile * ff = new TFile("output/totalresponse.root","RECREATE");
response->Write();
ff->Write();
ff->Close();
    
G4cout << "\n\nTotal number of PE absorptions = " << SteppingAction::Instance()->GetPENum() << "\n";
G4cout << "Total number of Compton scatters = " << SteppingAction::Instance()->GetComptonNum() << "\n\n";
    
    
    
    
#ifdef G4VIS_USE
	delete visManager;
#endif
	delete runManager;

	return 0;
}
