
#ifndef primarygeneratoractionmessenger_hh
#define primarygeneratoractionmessenger_hh 1

#include "globals.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

class PrimaryGeneratorActionMessenger : public G4UImessenger {
    
public:
    PrimaryGeneratorActionMessenger(PrimaryGeneratorAction* generator);
    ~PrimaryGeneratorActionMessenger();
    
    void SetNewValue(G4UIcommand * command,G4String newValue);

    
private:
    PrimaryGeneratorAction* fPrimaryGeneratorAction;
    
    G4UIdirectory* fDirDirectory;
    G4UIdirectory* fRunDirectory;
    
    G4UIcmdWithAnInteger*    fHPCmd;
    G4UIcmdWithAnInteger*    fHPNsideCmd;
    G4UIcmdWithAString*      fHPindexingCmd;
    G4UIcmdWithoutParameter* fSetUpHEALPixCmd;
    G4UIcmdWithAnInteger*    fNumPartCmd;
    G4UIcmdWithAnInteger*    fFinishHPCmd;
    G4UIcmdWithAnInteger*    fStartHPCmd;
    G4UIcmdWithABool*        fDOIsettingCmd;
    
};

#endif
