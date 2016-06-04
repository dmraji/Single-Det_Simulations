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

#ifdef CUSTOM_PHYSICS_LIST
    #include "PhysicsList.hh"
    typedef P1PhysicsList ThePhysicsList;
#else
    #include "QGSP_BERT.hh"
    typedef QGSP_BERT ThePhysicsList;
#endif

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
		UImanager->ApplyCommand("/control/execute ../macros/vis.mac");
#endif
		ui->SessionStart();
		delete ui;
#endif
	}

#ifdef G4VIS_USE
	delete visManager;
#endif
	delete runManager;

	return 0;
}
