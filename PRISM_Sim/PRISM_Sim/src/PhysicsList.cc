#include "PhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4ios.hh"

#include "G4UImanager.hh"
#include "globals.hh"

// gamma

#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh"
#include "G4LivermoreRayleighModel.hh"

// e-

#include "G4eMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

// e+

#include "G4eplusAnnihilation.hh"

// mu

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

// hadrons

#include "G4hMultipleScattering.hh"
#include "G4MscStepLimitType.hh"

#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4alphaIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

// msc models
//#include "G4UrbanMscModel93.hh"
#include "G4WentzelVIModel.hh"
#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4CoulombScattering.hh"

//

#include "G4LossTableManager.hh"
#include "G4EmProcessOptions.hh"

// particles

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"


using namespace CLHEP;

//==================================================================================================

PhysicsList* PhysicsList::fgInstance = 0;

//==================================================================================================

PhysicsList* PhysicsList::Instance(){

	return fgInstance;
}

//==================================================================================================

PhysicsList::PhysicsList(){

	fgInstance = this;

}

//==================================================================================================

PhysicsList::~PhysicsList(){

	fgInstance = 0;
}

//==================================================================================================

void PhysicsList::ConstructParticle(){


    // gamma
    G4Gamma::Gamma();

    // leptons
    G4Electron::Electron();
    G4Positron::Positron();
    G4MuonPlus::MuonPlus();
    G4MuonMinus::MuonMinus();

    // mesons
    G4PionPlus::PionPlusDefinition();
    G4PionMinus::PionMinusDefinition();
    G4KaonPlus::KaonPlusDefinition();
    G4KaonMinus::KaonMinusDefinition();

    // baryons
    G4Proton::Proton();
    G4AntiProton::AntiProton();

    // ions
    G4Deuteron::Deuteron();
    G4Triton::Triton();
    G4He3::He3();
    G4Alpha::Alpha();
    G4GenericIon::GenericIonDefinition();


}

//==================================================================================================

void PhysicsList::ConstructProcess(){

    AddTransportation();

    ConstructEM();

}

//==================================================================================================

void PhysicsList::ConstructEM(){

    theParticleIterator->reset();

    while( (*theParticleIterator)() ){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();

        if (particleName == "gamma") {
            pmanager->AddDiscreteProcess(new G4GammaConversion());
            pmanager->AddDiscreteProcess(new G4ComptonScattering());
            pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());
            pmanager->AddDiscreteProcess(new G4RayleighScattering());

        }

        else if (particleName == "e-") {
            pmanager->AddProcess(new G4eMultipleScattering());
            G4eIonisation* eIoni = new G4eIonisation();
            eIoni->SetStepFunction(0.2, 100*um);
            pmanager->AddProcess(eIoni,                -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung,-1, 3, 3);
        }

/*
        else if (particleName == "e+") {

            G4eMultipleScattering* msc = new G4eMultipleScattering();
            msc->SetStepLimitType(fUseDistanceToBoundary);
            pmanager->AddProcess(msc, -1, 1, 1);

            G4eIonisation* eIoni = new G4eIonisation();
            eIoni->SetStepFunction(0.2, 100*um);

            pmanager->AddProcess(eIoni,                 -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung, -1,-3, 3);
            pmanager->AddProcess(new G4eplusAnnihilation,0,-1, 4);

        }

        else if (particleName == "mu+" || particleName == "mu-") {

            G4MuMultipleScattering* msc = new G4MuMultipleScattering();
            msc->AddEmModel(0, new G4WentzelVIModel());
            pmanager->AddProcess(msc,                       -1, 1, 1);

            G4MuIonisation* muIoni = new G4MuIonisation();
            muIoni->SetStepFunction(0.2, 50*um);

            pmanager->AddProcess(muIoni,                    -1, 2, 2);
            pmanager->AddProcess(new G4MuBremsstrahlung,    -1,-3, 3);
            pmanager->AddProcess(new G4MuPairProduction,    -1,-4, 4);
            pmanager->AddDiscreteProcess(new G4CoulombScattering());

        }

        else if (particleName == "GenericIon") {

            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);

            G4ionIonisation* ionIoni = new G4ionIonisation();
            ionIoni->SetEmModel(new G4IonParametrisedLossModel());
            ionIoni->SetStepFunction(0.1, 10*um);
            pmanager->AddProcess(ionIoni,                   -1, 2, 2);
            pmanager->AddProcess(new G4NuclearStopping(),   -1, 3,-1);

        }

        else if (particleName == "alpha" || particleName == "He3" ) {

            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);

            G4ionIonisation* ionIoni = new G4ionIonisation();
            ionIoni->SetStepFunction(0.1, 20*um);
            pmanager->AddProcess(ionIoni,                   -1, 2, 2);
            pmanager->AddProcess(new G4NuclearStopping(),   -1, 3,-1);

        }

        else if (particleName == "pi+"   ||
                 particleName == "pi-"   ||
                 particleName == "kaon+" ||
                 particleName == "kaon-" ||
                 particleName == "proton" ) {

            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);

            G4hIonisation* hIoni = new G4hIonisation();
            hIoni->SetStepFunction(0.2, 50*um);

            pmanager->AddProcess(hIoni,                     -1, 2, 2);
            pmanager->AddProcess(new G4hBremsstrahlung,     -1,-3, 3);
            pmanager->AddProcess(new G4hPairProduction,     -1,-4, 4);

        }

        else if (particleName == "B+" ||
                 particleName == "B-" ||
                 particleName == "D+" ||
                 particleName == "D-" ||
                 particleName == "Ds+" ||
                 particleName == "Ds-" ||
                 particleName == "anti_He3" ||
                 particleName == "anti_alpha" ||
                 particleName == "anti_deuteron" ||
                 particleName == "anti_lambda_c+" ||
                 particleName == "anti_omega-" ||
                 particleName == "anti_proton" ||
                 particleName == "anti_sigma_c+" ||
                 particleName == "anti_sigma_c++" ||
                 particleName == "anti_sigma+" ||
                 particleName == "anti_sigma-" ||
                 particleName == "anti_triton" ||
                 particleName == "anti_xi_c+" ||
                 particleName == "anti_xi-" ||
                 particleName == "deuteron" ||
                 particleName == "lambda_c+" ||
                 particleName == "omega-" ||
                 particleName == "sigma_c+" ||
                 particleName == "sigma_c++" ||
                 particleName == "sigma+" ||
                 particleName == "sigma-" ||
                 particleName == "tau+" ||
                 particleName == "tau-" ||
                 particleName == "triton" ||
                 particleName == "xi_c+" ||
                 particleName == "xi-" ) {

            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
        }
    */
    }
}


//==================================================================================================

void PhysicsList::SetCuts(){}

//==================================================================================================
