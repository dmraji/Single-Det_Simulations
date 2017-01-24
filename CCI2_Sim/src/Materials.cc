// Originally developed by Caleb Roecker at UCB (now at LANL)
//

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "G4OpticalSurface.hh"
#include "globals.hh"
#include "G4LogicalBorderSurface.hh"

#include "GeExampleMaterials.hh"
#include "GeExampleDetectorConstruction.hh"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;
GeExampleMaterials *GeExampleMaterials::RadDetMaterials = 0;

GeExampleMaterials::GeExampleMaterials(GeExampleDetectorConstruction* SimDet):GeExampleDet(SimDet)
{
	//Create the materials list, optical properties to be set later
	CreateMaterials();
	CreateVisualAttributes();

	//Create a point to this for the class
	RadDetMaterials = this;
}

GeExampleMaterials::~GeExampleMaterials()
{
	//delete variables/objects
	delete water;
	delete vacuum;
	delete air;
	delete lead;
	delete lithium;
	delete steel;
    delete germanium;
    delete silicon;
    delete aluminum;
    delete copper;
    delete g10;

	delete waterVis;
	delete vacuumVis;
	delete airVis;
	delete leadVis;
	delete steelVis;
    delete aluminumVis;
}

GeExampleMaterials *GeExampleMaterials::GetMaterials()
{
	//return pointer to this for classes outside
	return RadDetMaterials;
}

void GeExampleMaterials::CreateMaterials()
{
	//	Isotopes
	G4Isotope *H1 = new G4Isotope( "H1", 1, 1, 1.007825035*g/mole );
	G4Isotope *H2 = new G4Isotope( "H2", 1, 2, 2.014101779*g/mole );

	G4Isotope *C12 = new G4Isotope( "C12", 6, 12, 12.*g/mole );
	G4Isotope *C13 = new G4Isotope( "C13", 6, 13, 13.003354826*g/mole );

	G4Isotope *N14 = new G4Isotope( "N14", 7, 14, 14.003074*g/mole );
	G4Isotope *N15 = new G4Isotope( "N15", 7, 15, 15.000109*g/mole );

	G4Isotope *O16 = new G4Isotope( "O16", 8, 16, 15.99491463*g/mole );
	G4Isotope *O17 = new G4Isotope( "O17", 8, 17, 16.9991312*g/mole );
	G4Isotope *O18 = new G4Isotope( "O18", 8, 18, 17.9991603*g/mole );

    G4Isotope *Al27 = new G4Isotope( "Al27", 13, 27, 26.981538613*g/mole);

	G4Isotope *Si28 = new G4Isotope( "Si28", 14, 28, 27.9769271*g/mole );
	G4Isotope *Si29 = new G4Isotope( "Si29", 14, 29, 28.9764949*g/mole );
	G4Isotope *Si30 = new G4Isotope( "Si30", 14, 30, 29.9737707*g/mole );

	G4Isotope *Pb204 = new G4Isotope( "Pb204", 82, 204, 203.973020*g/mole );
	G4Isotope *Pb206 = new G4Isotope( "Pb206", 82, 206, 205.974440*g/mole );
	G4Isotope *Pb207 = new G4Isotope( "Pb207", 82, 207, 206.975872*g/mole );
	G4Isotope *Pb208 = new G4Isotope( "Pb208", 82, 208, 207.976627*g/mole );

	G4Isotope *Fe54 = new G4Isotope( "Fe54", 26, 54, 53.939613*g/mole );
	G4Isotope *Fe56 = new G4Isotope( "Fe56", 26, 56, 55.934939*g/mole );
	G4Isotope *Fe57 = new G4Isotope( "Fe57", 26, 57, 56.935396*g/mole );
	G4Isotope *Fe58 = new G4Isotope( "Fe58", 26, 58, 57.933277*g/mole );

    G4Isotope *Cu63 = new G4Isotope( "Cu63", 29, 63, 62.92959756*g/mole);
    G4Isotope *Cu65 = new G4Isotope( "Cu65", 29, 65, 64.92778957*g/mole);

    G4Isotope *Li6 = new G4Isotope( "Li6", 3, 6, 6.015122795*g/mole );

	G4Isotope *Mo92 = new G4Isotope( "Mo92", 42, 92, 91.906811*g/mole );
	G4Isotope *Mo94 = new G4Isotope( "Mo94", 42, 94, 93.9050883*g/mole );
	G4Isotope *Mo95 = new G4Isotope( "Mo95", 42, 95, 94.9058421*g/mole );
	G4Isotope *Mo96 = new G4Isotope( "Mo96", 42, 96, 95.9046795*g/mole );
	G4Isotope *Mo97 = new G4Isotope( "Mo97", 42, 97, 96.9060215*g/mole );
	G4Isotope *Mo98 = new G4Isotope( "Mo98", 42, 98, 97.9054082*g/mole );
	G4Isotope *Mo100 = new G4Isotope( "Mo100", 42, 100, 99.907477*g/mole );

	G4Isotope *Cr50 = new G4Isotope( "Cr50", 24, 50, 49.9460442*g/mole );
	G4Isotope *Cr52 = new G4Isotope( "Cr52", 24, 52, 51.9405075*g/mole );
	G4Isotope *Cr53 = new G4Isotope( "Cr53", 24, 53, 52.9406494*g/mole );
	G4Isotope *Cr54 = new G4Isotope( "Cr54", 24, 54, 53.9388804*g/mole );

    G4Isotope *Ge70 = new G4Isotope("Ge70",32,70,69.9242474*g/mole);
    G4Isotope *Ge72 = new G4Isotope("Ge72",32,72,71.9220758*g/mole);
    G4Isotope *Ge73 = new G4Isotope("Ge73",32,73,72.9234589*g/mole);
    G4Isotope *Ge74 = new G4Isotope("Ge74",32,74,73.9211778*g/mole);
    G4Isotope *Ge76 = new G4Isotope("Ge76",32,76,75.9214026*g/mole);

	//	Elements by standard isotopic percentages

	G4Element *natH = new G4Element( "Natural H", "natH", 2 );
	natH->AddIsotope( H1, 99.9885*perCent );
	natH->AddIsotope( H2, 0.0115*perCent );

	G4Element *natN = new G4Element( "Natural N", "natN", 2 );
	natN->AddIsotope( N14, 99.634*perCent );
	natN->AddIsotope( N15, 0.366*perCent );

	G4Element *natO = new G4Element( "Natural O", "natO", 3 );
	natO->AddIsotope( O16, 99.757*perCent );
	natO->AddIsotope( O17, 0.038*perCent );
	natO->AddIsotope( O18, 0.205*perCent );

	G4Element *natC = new G4Element( "Natural C", "natC", 2 );
	natC->AddIsotope( C12, 98.93*perCent );
	natC->AddIsotope( C13, 1.07*perCent );

    G4Element *natAl = new G4Element( "Natural Al", "natAl",1);
    natAl->AddIsotope(Al27, 100.0*perCent);

	G4Element *natSi = new G4Element( "Natural Si", "natSi", 3 );
	natSi->AddIsotope( Si28, 92.2297*perCent );
	natSi->AddIsotope( Si29, 4.6832*perCent );
	natSi->AddIsotope( Si30, 3.0872*perCent );

	G4Element *natPb = new G4Element( "Natural Pb", "natPb", 4 );
	natPb->AddIsotope( Pb204, 1.4*perCent );
	natPb->AddIsotope( Pb206, 24.1*perCent );
	natPb->AddIsotope( Pb207, 22.1*perCent );
	natPb->AddIsotope( Pb208, 52.4*perCent );

	G4Element *natFe = new G4Element( "Natural Fe", "natFe", 4 );
	natFe->AddIsotope( Fe54, 5.8*perCent );
	natFe->AddIsotope( Fe56, 91.76*perCent );
	natFe->AddIsotope( Fe57, 2.15*perCent );
	natFe->AddIsotope( Fe58, 0.29*perCent );

    G4Element *natCu = new G4Element( "Natural Cu", "natCu", 2);
    natCu->AddIsotope( Cu63, 69.1515*perCent);
    natCu->AddIsotope( Cu65, 30.8515*perCent);

	G4Element * natLi = new G4Element( "Natural Li", "natLi",1);
	natLi->AddIsotope( Li6,100*perCent);

	G4Element *natMo = new G4Element( "Natural Mo", "natMo", 7 );
	natMo->AddIsotope( Mo92, 14.84*perCent );
	natMo->AddIsotope( Mo94, 9.25*perCent );
	natMo->AddIsotope( Mo95, 15.92*perCent );
	natMo->AddIsotope( Mo96, 16.68*perCent );
	natMo->AddIsotope( Mo97, 9.55*perCent );
	natMo->AddIsotope( Mo98, 24.13*perCent );
	natMo->AddIsotope( Mo100, 9.63*perCent );

	G4Element *natCr = new G4Element( "Natural Cr", "natCr", 4 );
	natCr->AddIsotope( Cr50, 4.345*perCent );
	natCr->AddIsotope( Cr52, 83.789*perCent );
	natCr->AddIsotope( Cr53, 9.501*perCent );
	natCr->AddIsotope( Cr54, 2.365*perCent );

    G4Element *natGe = new G4Element("Natural Ge","natGe",5);
    natGe->AddIsotope(Ge70,20.3818*perCent);
    natGe->AddIsotope(Ge72,27.3126*perCent);
    natGe->AddIsotope(Ge73,7.768*perCent);
    natGe->AddIsotope(Ge74,36.7215*perCent);
    natGe->AddIsotope(Ge76,7.837*perCent);

	//	Materials
	air = new G4Material( "air",1.184*kg/m3 , 2);
	air->AddElement( natN, 80.0*perCent);
	air->AddElement( natO, 20.0*perCent);

	vacuum = new G4Material( "vacuum", 1., 1.008*g/mole, 1.e-25*g/cm3,kStateGas, 273*kelvin, 3.8e-18*pascal );

	lead = new G4Material( "lead", 11.340*g/cm3, 1 );
	lead->AddElement( natPb, 1 );

	lithium = new G4Material( "lithium", 0.534*g/cm3, 1 );
	lithium->AddElement( natLi, 1 );

	water = new G4Material( "water", 1*g/cm3, 2 );
	water->AddElement( natH, 2 );
	water->AddElement( natO, 1 );

	steel = new G4Material( "steel", 7.84*g/cm3, 4 );
	steel->AddElement( natFe, 84.*perCent );
	steel->AddElement( natC, 14.*perCent );
	steel->AddElement( natCr, 1.*perCent );
	steel->AddElement( natMo, 1.*perCent );

    germanium = new G4Material("germanium",5.323*g/cm3,1);
    germanium->AddElement(natGe,1.0);

    silicon = new G4Material("silicon",2.57*g/cm3,1);
    silicon->AddElement(natSi,1.0);

    aluminum = new G4Material("aluminum",2.70*g/cm3,2);
    aluminum->AddElement(natAl, 99.5*perCent);
    aluminum->AddElement(natSi, 0.5*perCent);

    copper = new G4Material("copper",8.96*g/cm3,1);
    copper->AddElement(natCu,1.0);

    g10 = new G4Material("G10",1.7*g/cm3, 4);
    g10->AddElement(natSi, 1);
    g10->AddElement(natO , 2);
    g10->AddElement(natC , 3);
    g10->AddElement(natH , 3);
}

void GeExampleMaterials::CreateVisualAttributes()
{
	//Define the visual attributes for everything in the volume
	waterVis = new G4VisAttributes();
	waterVis->SetColor( 0.0, 0.0, 1.0, 0.1 );
	waterVis->SetVisibility( true );

	vacuumVis = new G4VisAttributes();
	vacuumVis->SetVisibility( true );

	airVis = new G4VisAttributes();//red
	airVis->SetColor(1.0,0.0,0.0);
	airVis->SetVisibility( false );

	leadVis = new G4VisAttributes();
	leadVis->SetColor( 1., 0.4, 0.4 );
    //leadVis->SetColor( G4Color::Magenta() );
	//leadVis->SetForceSolid( true );
    //leadVis->SetForceWireframe( true );
	leadVis->SetForceWireframe( true );
	leadVis->SetVisibility( true );

	steelVis = new G4VisAttributes();//No clue
	steelVis->SetColor (1.0, 0.7, 0.4);
	steelVis->SetForceWireframe( true );
	steelVis->SetVisibility( true );

    aluminumVis = new G4VisAttributes();
    aluminumVis->SetColor(G4Color::Yellow());
    aluminumVis->SetForceSolid(true);
    aluminumVis->SetVisibility(true);

    copperVis = new G4VisAttributes();
    copperVis->SetColor(G4Color::Yellow());
    copperVis->SetForceSolid(true);
    copperVis->SetVisibility(true);

}
