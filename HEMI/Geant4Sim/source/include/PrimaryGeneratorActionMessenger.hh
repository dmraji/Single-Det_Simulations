
#ifndef primarygeneratoractionmessenger_hh
#define primarygeneratoractionmessenger_hh 1

#include "globals.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"


class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADouble;

//==================================================================================================

class PrimaryGeneratorActionMessenger : public G4UImessenger {
    
public:
    PrimaryGeneratorActionMessenger(PrimaryGeneratorAction* generator);
    ~PrimaryGeneratorActionMessenger();
    
    void SetNewValue(G4UIcommand * command,G4String newValue);

    
private:
    PrimaryGeneratorAction* fPrimaryGeneratorAction;
    
    G4UIdirectory* fDirectory;
    
    G4UIcmdWithADouble* fThetaCmd;
    G4UIcmdWithADouble* fPhiCmd;

    
};

//==================================================================================================


#endif
