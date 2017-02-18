//
// ********************************************************************
// * HPGe-wh		                                              *
// *                                                                  *
// * 2017-02-16,by Zh                                                 *
// ********************************************************************

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4Tubs.hh"
#include "G4Hype.hh"
#include "G4Trd.hh"
#include "G4Orb.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
   
  // Option to switch on/off checking of volumes overlaps

  G4bool checkOverlaps = true;
     
  // Parameters Defination

  G4double world_sizeXY = 25 * cm;
  G4double world_sizeZ  = 25 * cm;
  G4double cryDia  = 56.1 * mm;
  G4double cryLen  = 67.6 * mm;
  G4double cryTorRad  = 8 * mm;
  G4double holeDia  = 10.3 * mm;
  G4double holeDep  = 61 * mm;
  G4double holeOrbRad  = 5 * mm;
  G4double LideadTh  = 0.7 * mm;
  G4double BdeadTh  = 0.3 / 1000 * mm;
  G4double Al_Mylar  = 0.03 * mm;
  G4double AlShieK  = 0.8 * mm;
  G4double AlShieL  = 1 * mm;
  G4double space4  = 4 * mm;
  G4double space3  = 3 * mm;
  G4double cupLen  = 105 * mm;
  G4double DiaK  = cryDia + 3 * mm;
  G4double DiaL  = cryDia + 10 * mm;
  G4double TopBeTh  = 0.5 * mm;
  G4double TopAlTh  = 1.5 * mm;
  
  // Materials Defination
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* Al = nist->FindOrBuildMaterial("G4_Al");
  G4Material* HPGeMat = nist->FindOrBuildMaterial("G4_Ge");
  G4Material* Li = nist->FindOrBuildMaterial("G4_Li");
  G4Material* B = nist->FindOrBuildMaterial("G4_B");
  G4Material* Be = nist->FindOrBuildMaterial("G4_Be");
  G4Material* Mylar = nist->FindOrBuildMaterial("G4_Mylar");
  
  // World
  G4Box* solidWorld =    
    new G4Box("World",				//its name
		0.5 * world_sizeXY,
		0.5 * world_sizeXY,
		0.5 * world_sizeZ);		//its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  // HPGe
  G4ThreeVector posHPGe = G4ThreeVector(0, 0, cryTorRad);

  G4Torus* solidHPGeTorus = 
  	new G4Torus("HPGeTorus",			//name
  			0,				//Rmin
  			cryTorRad,			//Rmax
  			cryDia / 2 - cryTorRad,		//Rtor
  			0,				//Sphi
  			360 *degree);			//Dphi

  G4Tubs* solidHPGeTub1 = 
  	new G4Tubs("HPGeTub1",
  			0,
  			cryDia / 2 - cryTorRad,
  			cryTorRad,
  			0,
  			360 * degree);

  G4Tubs* solidHPGeTub2 = 
  	new G4Tubs("HPGeTub2",
  			0,
  			cryDia / 2,
  			(cryLen - cryTorRad) / 2,
  			0,
  			360 * degree);

  G4UnionSolid* solidHPGeShape1 =
  	new G4UnionSolid("HPGeShape1",
  			solidHPGeTorus,
  			solidHPGeTub1);

  G4ThreeVector zTransTub2(0, 0, (cryLen - cryTorRad) / 2);
  G4UnionSolid* solidHPGeShape2 =
  	new G4UnionSolid("HPGe",
  			solidHPGeShape1,
  			solidHPGeTub2,
  			0,
  			zTransTub2);

  G4Tubs* solidCoolingTub =
	new G4Tubs("CoolingTub",
			0,
			holeDia / 2,
			(holeDep - holeOrbRad) / 2,
			0,
			360 * degree);
  
  G4Orb* solidCoolingOrb =
	new G4Orb("CoolingOrb",
			holeOrbRad);

  G4ThreeVector zTransCoolingTub(0, 0, (holeDep - holeOrbRad) / 2 );

  G4UnionSolid* solidCoolingHole =
	new G4UnionSolid("CoolingHole",
			solidCoolingOrb,
			solidCoolingTub,
			0,
			zTransCoolingTub);

  G4ThreeVector zTransCoolingHole(0, 0, (cryLen  - holeDep - cryTorRad + holeOrbRad) );

  G4SubtractionSolid* solidHPGe =
	new G4SubtractionSolid("HPGe",
				solidHPGeShape2,
				solidCoolingHole,
				0,
				zTransCoolingHole);

  G4LogicalVolume* logicalHPGe = 
	new G4LogicalVolume(solidHPGe,			// solid
				HPGeMat,		// material
				"logicHPGe");		// name

  new G4PVPlacement(0,
  			posHPGe,
  			logicalHPGe,
  			"HPGe",
  			logicWorld,
  			false,
  			0,
  			checkOverlaps);

  // Li dead layer
  G4ThreeVector posLideadlayer = G4ThreeVector(0, 0, (cryLen - holeDep + holeOrbRad));
  
  G4Sphere* solidLideadlayerSp =
	new G4Sphere("LideadlayerSp",			// name
			holeOrbRad - LideadTh,		// Rmin
			holeOrbRad,			// Rmax
			0 * degree,			// Sphi
			360 * degree,			// Dphi
			90 * degree,			// Sthe
			270 * degree);			// Dthe

  G4Tubs* solidLideadlayerTub =
	new G4Tubs("solidLideadlayerTub",
			holeOrbRad - LideadTh,
			holeOrbRad * mm,
			(holeDep - holeOrbRad) / 2,
			0,
			360 * degree);

  G4ThreeVector zTransLideadlayerTub = G4ThreeVector(0, 0, (holeDep - holeOrbRad) / 2);
  G4UnionSolid* solidLideadlayer =
	new G4UnionSolid("Lideadlayer",
			solidLideadlayerSp,
			solidLideadlayerTub,
			0,
			zTransLideadlayerTub);

  G4LogicalVolume* logicLideadlayer =
	new G4LogicalVolume(solidLideadlayer,
			Li,
			"logicLideadlayer");

  new G4PVPlacement(0,
			posLideadlayer,
			logicLideadlayer,
			"Lideadlayer",
			logicWorld,
			false,
			0,
			checkOverlaps);
  //B dead layer
  G4ThreeVector posBdeadlayer = G4ThreeVector(0, 0, (cryTorRad + (cryLen - cryTorRad) / 2));
  G4Tubs* solidBdeadlayer =
	new G4Tubs("solidBdeadlayer",
			cryDia / 2,
			cryDia / 2 + BdeadTh,
			(cryLen - cryTorRad) / 2,
			0,
			360 * degree);

  G4LogicalVolume* logicBdeadlayer =
	new G4LogicalVolume(solidBdeadlayer,
			B,
			"logicBdeadlayer");

  new G4PVPlacement(0,
			posBdeadlayer,
			logicBdeadlayer,
			"Bdeadlayer",
			logicWorld,
			false,
			0,
			checkOverlaps);
  
  // Al Shield K
  G4ThreeVector posAlShieK = G4ThreeVector(0, 0, (cupLen - space3) / 2);
  
  G4Tubs* solidAlShieK = 
  	new G4Tubs("solidAlShieK",
  			DiaK / 2,
  			DiaK / 2 + AlShieK,
  			(cupLen - space3) / 2,
  			0,
  			360 * degree);
  
  
  G4LogicalVolume* logicAlShieK =
	new G4LogicalVolume(solidAlShieK,
			Al,
			"logicAlShieK");
  
  new G4PVPlacement(0,
			posAlShieK,
			logicAlShieK,
			"AlShieK",
			logicWorld,
			false,
			0,
			checkOverlaps);

  // Al and Mylar
  
  G4ThreeVector posAlMy_Al = G4ThreeVector(0, 0, - Al_Mylar / 2);
  G4ThreeVector posAlMy_My = G4ThreeVector(0, 0, - Al_Mylar * 3 / 2);
  
  G4Tubs* solidAlMy_Al = 
  	new G4Tubs("solidAlMy_Al",
  			0,
  			DiaK / 2 + AlShieK,
  			Al_Mylar / 2,
  			0,
  			360 * degree);
  
  G4Tubs* solidAlMy_My = 
  	new G4Tubs("solidAlMy_My",
  			0,
  			DiaK / 2 + AlShieK,
  			Al_Mylar / 2,
  			0,
  			360 * degree);
  			
  G4LogicalVolume* logicAlMy_Al =
	new G4LogicalVolume(solidAlMy_Al,
			Al,
			"logicAlMy_Al");

  G4LogicalVolume* logicAlMy_My =
	new G4LogicalVolume(solidAlMy_My,
			Mylar,
			"logicAlMy_My");
  
  new G4PVPlacement(0,
			posAlMy_Al,
			logicAlMy_Al,
			"AlMy_Al",
			logicWorld,
			false,
			0,
			checkOverlaps);
  
  new G4PVPlacement(0,
			posAlMy_My,
			logicAlMy_My,
			"AlMy_My",
			logicWorld,
			false,
			0,
			checkOverlaps);
  
  // Button Al of K
  G4ThreeVector posBuk = G4ThreeVector(0, 0, cupLen - space3 / 2);
  
  G4Tubs* solidBuk = 
  	new G4Tubs("solidBuk",
  			holeDia / 2,
  			DiaK / 2 + AlShieK,
  			space3 / 2,
  			0,
  			360 * degree);
  
  
  G4LogicalVolume* logicBuK =
	new G4LogicalVolume(solidBuk,
			Al,
			"logicBuk");
  
  new G4PVPlacement(0,
			posBuk,
			logicBuK,
			"Buk",
			logicWorld,
			false,
			0,
			checkOverlaps);
  
  // Al Shield L
  G4ThreeVector posAlShieL = G4ThreeVector(0, 0, cupLen / 2);
  
  G4Tubs* solidAlShieL = 
  	new G4Tubs("solidAlShieL",
  			DiaL / 2,
  			DiaL / 2 + AlShieL,
  			cupLen / 2 + space4,
  			0,
  			360 * degree);
  
  
  G4LogicalVolume* logicAlShieL =
	new G4LogicalVolume(solidAlShieL,
			Al,
			"logicAlShieL");
  
  new G4PVPlacement(0,
			posAlShieL,
			logicAlShieL,
			"AlShieL",
			logicWorld,
			false,
			0,
			checkOverlaps);

  // Top Be I
  G4ThreeVector posTopBe = G4ThreeVector(0, 0, - space4 - TopBeTh / 2);
  
  G4Tubs* solidTopBe = 
  	new G4Tubs("solidTopBe",
  			0,
  			DiaK / 2,
  			TopBeTh / 2,
  			0,
  			360 * degree);
  
  
  G4LogicalVolume* logicTopBe =
	new G4LogicalVolume(solidTopBe,
			Be,
			"logicTopBe");
  
  new G4PVPlacement(0,
			posTopBe,
			logicTopBe,
			"TopBe",
			logicWorld,
			false,
			0,
			checkOverlaps);

  // Top Al I
  G4ThreeVector posTopAl = G4ThreeVector(0, 0, - space4 - TopAlTh / 2);
  
  G4Tubs* solidTopAl = 
  	new G4Tubs("solidTopAl",
  			DiaK / 2,
  			DiaL / 2 + AlShieL,
  			TopAlTh / 2,
  			0,
  			360 * degree);
  
  
  G4LogicalVolume* logicTopAl =
	new G4LogicalVolume(solidTopAl,
			Al,
			"logicTopAl");
  
  new G4PVPlacement(0,
			posTopAl,
			logicTopAl,
			"TopAl",
			logicWorld,
			false,
			0,
			checkOverlaps);
  
  // Set HPGe as scoring volume
  fScoringVolume = logicalHPGe;

  //always return the physical World
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
