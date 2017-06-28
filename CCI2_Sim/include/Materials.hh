// Originally developed by Caleb Roecker at UCB (now at LANL)
//

#ifndef __GeExampleMaterials_hh__
#define __GeExampleMaterials_hh_ 1

class G4Material;
class G4VisAttributes;
class G4OpticalSurface;
class GeExampleDetectorConstruction;
class G4LogicalBorderSurface;

class GeExampleMaterials
{

public:
	GeExampleMaterials(GeExampleDetectorConstruction *);
	~GeExampleMaterials();

    GeExampleMaterials * GetMaterials();

	inline G4Material * Water() { return water; };
	inline G4Material * Vacuum() { return vacuum; };
	inline G4Material * Air() { return air; };
	inline G4Material * Lead() { return lead; };
	inline G4Material * Steel() { return steel; };
	inline G4Material * Germanium() { return germanium; };
	inline G4Material * Lithium() { return lithium; };
	inline G4Material * Silicon() { return silicon; };
    inline G4Material * Aluminum() { return aluminum; };
    inline G4Material * Copper() { return copper; };
    inline G4Material *G10() { return g10; };

	inline G4VisAttributes * WaterVis() { return waterVis; };
	inline G4VisAttributes * VacuumVis() { return vacuumVis; };
	inline G4VisAttributes * AirVis() { return airVis; };
	inline G4VisAttributes * LeadVis() {return leadVis; };
	inline G4VisAttributes * SteelVis() {return steelVis; };
    inline G4VisAttributes * AluminumVis() {return aluminumVis; };
    inline G4VisAttributes * CopperVis() {return copperVis; };

private:
	void CreateMaterials();
	void CreateVisualAttributes();

private:
	static GeExampleMaterials *RadDetMaterials;

	GeExampleDetectorConstruction*	GeExampleDet;

	G4Material * water;
	G4Material * vacuum;
	G4Material * air;
	G4Material * rock;
	G4Material * lead;
	G4Material * lithium;
	G4Material * steel;
	G4Material * germanium;
    G4Material * silicon;
    G4Material * aluminum;
    G4Material * copper;
    G4Material * g10;

	G4VisAttributes * waterVis;
	G4VisAttributes * vacuumVis;
	G4VisAttributes * airVis;
	G4VisAttributes * leadVis;
	G4VisAttributes * steelVis;
    G4VisAttributes * aluminumVis;
    G4VisAttributes * copperVis;
};

#endif
