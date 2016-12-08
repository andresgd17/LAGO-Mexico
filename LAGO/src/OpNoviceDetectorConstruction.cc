    //
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "OpNoviceDetectorConstruction.hh"
#include "LXePMTSD.hh"
#include "LXeScintSD.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSensitiveDetector.hh"

#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"
#include "G4Cons.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SDManager.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

//#include "G4VPrimitiveSensitivity.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::OpNoviceDetectorConstruction()
 : G4VUserDetectorConstruction()
{
  fExpHall_x = fExpHall_y =  100.0*m;
  fExpHall_z = 20.0*m;
  fTank_x    = fTank_y    = fTank_z    =  5.0*m;
  fBubble_x  = fBubble_y  = fBubble_z  =  0.5*m;

  scint_x = scint_y = 7.3*m;    //Diámetro
  scint_z = 1.0*m;             //Artura
  Scint_z = 4.2*m;
  outerRadius_pmtL = 10.3*cm;    //Radio de los fototubos laterales 8 pulgadas

//Desplazamiento de posición del tanque tank 2 respecto del tanque 1
  move_x2 = 24.74*m;	//Desplazamiento en x
  move_y2 = 0.0*m;  //Desplazamiento en y

//Desplazamiento de posición del tanque tank 3 respecto del tanque 1
  move_x3 = 17.56*m;	//Desplazamiento en x
  move_y3 = -21.64*m;  //Desplazamiento en y

//desplazamiento tanque central
  move_x4 = 14.10*m;
  move_y4 = -7.22*m;

  wtyvek=0.2*mm;   //grosor tyvek=200 micras
  d_mtl=0.635*cm; //width aluminum cylinder
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::~OpNoviceDetectorConstruction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* OpNoviceDetectorConstruction::Construct()
{

// ------------- Materials -------------

  G4double a, z, density;
  G4int nelements;


//Aluminum
//
G4Material* Al = new G4Material("Al",z=13.,a=26.98*g/mole,density=2.7*g/cm3);

//Vacuum
//
G4Material* Vacuum = new G4Material("Vacuum", z=1., a=1.01*g/mole, density=universe_mean_density, kStateGas, 0.1*kelvin, 1.e-19*pascal);

//Elements
//
  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
  G4Element* C = new G4Element("C", "C", z=6., a=12.01*g/mole);

// Air
//
  G4Material* air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  air->AddElement(N, 70.*perCent);
  air->AddElement(O, 30.*perCent);

// Water
//
  G4Element* H = new G4Element("Hydrogen", "H", z=1 , a=1.01*g/mole);

  G4Material* water = new G4Material("Water", density= 1.0*g/cm3, nelements=2);
  water->AddElement(H, 2);
  water->AddElement(O, 1);

//Tyvek
//
  G4Material* Tyvek = new G4Material("Tyvek",density=0.935*g/cm3,2);
  Tyvek->AddElement(C,2);
  Tyvek->AddElement(H,4);


//
// ------------ Generate & Add Material Properties Table ------------
//
  G4double photonEnergy[] =
            { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };

  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

//
// Water
//
  G4double refractiveIndex1[] =
            { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
              1.346,  1.3465, 1.347,  1.3475, 1.348,
              1.3485, 1.3492, 1.35,   1.3505, 1.351,
              1.3518, 1.3522, 1.3530, 1.3535, 1.354,
              1.3545, 1.355,  1.3555, 1.356,  1.3568,
              1.3572, 1.358,  1.3585, 1.359,  1.3595,
              1.36,   1.3608};

  assert(sizeof(refractiveIndex1) == sizeof(photonEnergy));

  G4double absorption[] =
           {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
           15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
           45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
           52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
           30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
           17.500*m, 14.500*m };

  assert(sizeof(absorption) == sizeof(photonEnergy));

  G4double scintilFast[] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };

  assert(sizeof(scintilFast) == sizeof(photonEnergy));

  G4double scintilSlow[] =
            { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
              7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
              3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
              4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
              7.00, 6.00, 5.00, 4.00 };

  assert(sizeof(scintilSlow) == sizeof(photonEnergy));

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  myMPT1->AddProperty("RINDEX",       photonEnergy, refractiveIndex1,nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("ABSLENGTH",    photonEnergy, absorption,     nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("FASTCOMPONENT",photonEnergy, scintilFast,     nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("SLOWCOMPONENT",photonEnergy, scintilSlow,     nEntries)
        ->SetSpline(true);

  myMPT1->AddConstProperty("SCINTILLATIONYIELD",50./MeV);
  myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT1->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT1->AddConstProperty("YIELDRATIO",0.8);

  G4double energy_water[] = {
     1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
     1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
     1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
     1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
     1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
     2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
     2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
     2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
     2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
     2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
     3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
     3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
     3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
     4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
     5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
  };

  const G4int numentries_water = sizeof(energy_water)/sizeof(G4double);

  //assume 100 times larger than the rayleigh scattering for now.
  G4double mie_water[] = {
     167024.4*m, 158726.7*m, 150742  *m,
     143062.5*m, 135680.2*m, 128587.4*m,
     121776.3*m, 115239.5*m, 108969.5*m,
     102958.8*m, 97200.35*m, 91686.86*m,
     86411.33*m, 81366.79*m, 76546.42*m,
     71943.46*m, 67551.29*m, 63363.36*m,
     59373.25*m, 55574.61*m, 51961.24*m,
     48527.00*m, 45265.87*m, 42171.94*m,
     39239.39*m, 36462.50*m, 33835.68*m,
     31353.41*m, 29010.30*m, 26801.03*m,
     24720.42*m, 22763.36*m, 20924.88*m,
     19200.07*m, 17584.16*m, 16072.45*m,
     14660.38*m, 13343.46*m, 12117.33*m,
     10977.70*m, 9920.416*m, 8941.407*m,
     8036.711*m, 7202.470*m, 6434.927*m,
     5730.429*m, 5085.425*m, 4496.467*m,
     3960.210*m, 3473.413*m, 3032.937*m,
     2635.746*m, 2278.907*m, 1959.588*m,
     1675.064*m, 1422.710*m, 1200.004*m,
     1004.528*m, 833.9666*m, 686.1063*m
  };

  assert(sizeof(mie_water) == sizeof(energy_water));

  // gforward, gbackward, forward backward ratio
  G4double mie_water_const[3]={0.99,0.99,0.8};

  myMPT1->AddProperty("MIEHG",energy_water,mie_water,numentries_water)
        ->SetSpline(true);
  myMPT1->AddConstProperty("MIEHG_FORWARD",mie_water_const[0]);
  myMPT1->AddConstProperty("MIEHG_BACKWARD",mie_water_const[1]);
  myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO",mie_water_const[2]);

  G4cout << "Water G4MaterialPropertiesTable" << G4endl;
  myMPT1->DumpTable();

  water->SetMaterialPropertiesTable(myMPT1);

  // Set the Birks Constant for the Water scintillator

  water->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

//
// Air
//
  G4double refractiveIndex2[] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };

  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndex2, nEntries);

  G4cout << "Air G4MaterialPropertiesTable" << G4endl;
  myMPT2->DumpTable();

  air->SetMaterialPropertiesTable(myMPT2);

//
// ------------- Volumes --------------

// The experimental Hall
//
  G4Box* expHall_box = new G4Box("World",fExpHall_x,fExpHall_y,fExpHall_z);

  G4LogicalVolume* expHall_log
    = new G4LogicalVolume(expHall_box,Vacuum,"World",0,0,0);

  G4VPhysicalVolume* expHall_phys
    = new G4PVPlacement(0,G4ThreeVector(),expHall_log,"World",0,false,0);

// The Water Tank
//
  G4Tubs* waterTank_box = new  G4Tubs("Tank",		//Nombre
					0., 		//radio interno
					scint_x/2.,	//radio externo
					scint_z/2.,	//mitad de longitud en z
					0.*deg, 	//comienzo en phi
                                        360.*deg);	//segmento del ángulo

  G4LogicalVolume* waterTank_log
    = new G4LogicalVolume(waterTank_box,water,"Tank",0,0,0);
//Tank 1
  G4VPhysicalVolume* waterTank_phys
    = new G4PVPlacement(0,G4ThreeVector(),waterTank_log,"Tank1",
                        expHall_log,false,1);
//Tank 2
  G4VPhysicalVolume* waterTank_phys2
    = new G4PVPlacement(0,G4ThreeVector(move_x2,move_y2,0.0*m),waterTank_log,"Tank2",
                        expHall_log,false,2);
//Tank 3
  G4VPhysicalVolume* waterTank_phys3
    = new G4PVPlacement(0,G4ThreeVector(move_x3,move_y3,0.0*m),waterTank_log,"Tank3",
                        expHall_log,false,3);
 //tanque central
  G4Tubs* waterTank_box_c = new  G4Tubs("Tank_c",		//Nombre
                                        0., 		//radio interno
                                        scint_x/2.,	//radio externo
                                        Scint_z/2.,	//mitad de longitud en z
                                        0.*deg, 	//comienzo en phi
                                        360.*deg);	//segmento del ángulo

  G4LogicalVolume* waterTank_log_c
    = new G4LogicalVolume(waterTank_box_c,water,"Tank_c",0,0,0);
  G4VPhysicalVolume* waterTank_phys4
    = new G4PVPlacement(0,G4ThreeVector(move_x4,move_y4,1.6*m),waterTank_log_c,"Tank4",
                        expHall_log,false,4);

//*********************************************************************************
//The Tyvek cylinder
//*********************************************************************************
G4Tubs* tivek_cylinder = new G4Tubs("tivek_cylinder",
                                  scint_x/2.,
                                  scint_x/2.+ wtyvek,
                                  scint_z/2.,
                                  0.*deg,
                                  360.*deg);//cilindro completo

G4LogicalVolume* tyvek_log = new G4LogicalVolume(tivek_cylinder,G4Material::GetMaterial("Tyvek"),"tivek_log",0,0,0);

//Tank 1
G4VPhysicalVolume* tyvek_phys = new G4PVPlacement(0,                                   //sin rotación
                                   G4ThreeVector(0.,0.,0.),//en (x, y, z)
                                   tyvek_log,                           //su volúmen lógico
                                   "tivek_cylinder1",                             //su nombre
                                   expHall_log,                         //su volúmen madre
                                   false,                               //sin operaciones booleanas
                                   1);                                  //número de copia
//Tank 2
G4VPhysicalVolume* tyvek_phys2 = new G4PVPlacement(0,                                   //sin rotación
                                   G4ThreeVector(move_x2,move_y2,0.0*m),//en (x, y, z)
                                   tyvek_log,                           //su volúmen lógico
                                   "tivek_cylinder2",                             //su nombre
                                   expHall_log,                         //su volúmen madre
                                   false,                               //sin operaciones booleanas
                                   2); 
//Tank 3
G4VPhysicalVolume* tyvek_phys3 = new G4PVPlacement(0,                                   //sin rotación
                                   G4ThreeVector(move_x3,move_y3,0.0*m),//en (x, y, z)
                                   tyvek_log,                           //su volúmen lógico
                                   "tivek_cylinder3",                             //su nombre
                                   expHall_log,                         //su volúmen madre
                                   false,                               //sin operaciones booleanas
                                   3); 
//tanque central
/*G4Tubs* tivek_cylinder_c = new G4Tubs("tivek_cylinder_c",
                                  scint_x/2.,
                                  scint_x/2.+ wtyvek,
                                  Scint_z/2.,
                                  0.*deg,
                                  360.*deg);//cilindro completo

G4LogicalVolume* tyvek_log_c = new G4LogicalVolume(tivek_cylinder_c,G4Material::GetMaterial("Tyvek"),"tivek_log",0,0,0);

G4VPhysicalVolume* tyvek_phys4 = new G4PVPlacement(0,                                   //sin rotación
                                   G4ThreeVector(move_x4,move_y4,1.6*m),//en (x, y, z)
                                   tyvek_log_c,                           //su volúmen lógico
                                   "tivek_cylinder4",                             //su nombre
                                   expHall_log,                         //su volúmen madre
                                   false,                               //sin operaciones booleanas
                                   4);
*/
//*********************************************************************************
//Tyvek top
//*********************************************************************************
G4Tubs* tyvektop = new G4Tubs("tyvektop",
			0,
                        scint_x/2.+wtyvek,
			wtyvek/2.,
			0.*deg,
			360.*deg);//tapa
G4LogicalVolume* tyvektop_log    = new G4LogicalVolume(tyvektop,G4Material::GetMaterial("Tyvek"),"tyvektop_log",0,0,0);

//Tank 1
G4VPhysicalVolume* tyvektop_phys =  new G4PVPlacement(0,
                                       G4ThreeVector(0,0,scint_z/2.+wtyvek/2.),
                                       tyvektop_log,
                                       "tyvektop1",
                                       expHall_log,
                                       false,
                                       1);
//Tank 2
G4VPhysicalVolume* tyvektop_phys2 =  new G4PVPlacement(0,
                                       G4ThreeVector(move_x2,move_y2,scint_z/2.+wtyvek/2.),
                                       tyvektop_log,
                                       "tyvektop2",
                                       expHall_log,
                                       false,
                                       2);
//Tank 3
G4VPhysicalVolume* tyvektop_phys3 =  new G4PVPlacement(0,
                                       G4ThreeVector(move_x3,move_y3,scint_z/2.+wtyvek/2.),
                                       tyvektop_log,
                                       "tyvektop3",
                                       expHall_log,
                                       false,
                                       3);

//tanque central
/*G4Tubs* tyvektop_c = new G4Tubs("tyvektop_c", 0, scint_x/2.+wtyvek, wtyvek/2., 0.*deg, 360.*deg);//tapa
G4LogicalVolume* tyvektop_log_c    = new G4LogicalVolume(tyvektop_c,G4Material::GetMaterial("Tyvek"),"tyvektop_log_c",0,0,0);
G4VPhysicalVolume* tyvektop_phys4 =  new G4PVPlacement(0,
                                       G4ThreeVector(move_x4,move_y4,Scint_z/2.+wtyvek/2.+1.6*m),
                                       tyvektop_log_c,
                                       "tyvektop4",
                                       expHall_log,
                                       false,
                                       4);
*/

//*********************************************************************************
//Tyvek bottom
//*********************************************************************************	
G4Tubs* tyvekbottom = new G4Tubs("tyvekbottom",
			0.,
                        scint_x/2.+wtyvek,
			wtyvek/2.,
			0.*deg,
			360.*deg);//base
G4LogicalVolume* tyvekbottom_log = new G4LogicalVolume(tyvekbottom,G4Material::GetMaterial("Tyvek"),"tyvekbottom_log",0,0,0);    
//Tank 1
G4VPhysicalVolume* tyvekbottom_phys = new G4PVPlacement(0,
                                         G4ThreeVector(0,0,-scint_z/2.-wtyvek/2.),
                                         tyvekbottom_log,
                                         "tyvekbottom1",
                                         expHall_log,
                                         false,
                                         1);
//Tank 2
G4VPhysicalVolume* tyvekbottom_phys2 = new G4PVPlacement(0,
                                         G4ThreeVector(move_x2,move_y2,-scint_z/2.-wtyvek/2.),
                                         tyvekbottom_log,
                                         "tyvekbottom2",
                                         expHall_log,
                                         false,
                                         2);
//Tank 3
G4VPhysicalVolume* tyvekbottom_phys3 = new G4PVPlacement(0,
                                         G4ThreeVector(move_x3,move_y3,-scint_z/2.-wtyvek/2.),
                                         tyvekbottom_log,
                                         "tyvekbottom3",
                                         expHall_log,
                                         false,
                                         3);
//tanque central
/*G4Tubs* tyvekbottom_c = new G4Tubs("tyvekbottom_c", 0., scint_x/2.+wtyvek, wtyvek/2., 0.*deg, 360.*deg);//base
G4LogicalVolume* tyvekbottom_log_c = new G4LogicalVolume(tyvekbottom_c,G4Material::GetMaterial("Tyvek"),"tyvekbottom_log_c",0,0,0);
G4VPhysicalVolume* tyvekbottom_phys4 = new G4PVPlacement(0,
                                         G4ThreeVector(move_x4,move_y4,-scint_z/2.-wtyvek/2.),
                                         tyvekbottom_log_c,
                                         "tyvekbottom1_c",
                                         expHall_log,
                                         false,
                                         4);
*/
//*********************************************************************************
//TyvekWall x1
//*********************************************************************************
G4Box* tyvekwallshortx1 = new G4Box("tyvekwallx", scint_x/2., wtyvek/2., scint_z/2.);

G4LogicalVolume* tyvekwallshortx1_log  = new G4LogicalVolume(tyvekwallshortx1, G4Material::GetMaterial("Tyvek"), "tyvekwallshortx1_log",0,0,0);

G4VPhysicalVolume* tyvekwallshortx1_phys = new G4PVPlacement(0,				//Rotación del mother frame
                                       G4ThreeVector(0, 0, 0),	//Posición en el frame rotado
                                       tyvekwallshortx1_log,		//Nombre del volúmen lógico hija
                                       "tyvekwallshortx1",		//Nombre
                                       waterTank_log,			//Nombre lógico del volúmen madre
                                       false,
                                       0);

//*********************************************************************************
//TyvekWallshort y1
//*********************************************************************************
G4Box* tyvekwallshorty1 = new G4Box("tyvekwally1", wtyvek/2., scint_x/4.-wtyvek/4, scint_z/2.);

G4LogicalVolume* tyvekwallshorty1_log  = new G4LogicalVolume(tyvekwallshorty1, G4Material::GetMaterial("Tyvek"), "tyvekwallshorty1_log",0,0,0);

//First y wall
//
G4VPhysicalVolume* tyvekwallshorty1_phys = new G4PVPlacement(0,				//Rotación del mother frame
                                       G4ThreeVector(0, -scint_y/4.-wtyvek/2, 0),	//Posición en el frame rotado
                                       tyvekwallshorty1_log,		//Nombre del volúmen lógico hija
                                       "tyvekwallshorty1",		//Nombre
                                       waterTank_log,			//Nombre lógico del volúmen madre
                                       false,
                                       0);


//Second y wall
//
G4VPhysicalVolume* tyvekwallshorty2_phys = new G4PVPlacement(0,				//Rotación del mother frame
                                       G4ThreeVector(0, scint_y/4.+wtyvek/2, 0),	//Posición en el frame rotado
                                       tyvekwallshorty1_log,		//Nombre del volúmen lógico hija
                                       "tyvekwallshorty2",		//Nombre
                                       waterTank_log,			//Nombre lógico del volúmen madre
                                       false,
                                       1);

// The Air Bubble
//
  G4Box* bubbleAir_box = new G4Box("Bubble",fBubble_x,fBubble_y,fBubble_z);

  G4LogicalVolume* bubbleAir_log
    = new G4LogicalVolume(bubbleAir_box,air,"Bubble",0,0,0);

//G4VPhysicalVolume* bubbleAir_phys =
//      new G4PVPlacement(0,G4ThreeVector(0,2.5*m,0),bubbleAir_log,"Bubble",
//                        waterTank_log,false,0);


//PMTs spere
//
G4double sphere_w = 0.5*mm;	//sphere_width
G4double cone_w = 0.0*mm;	//cone_width
G4double long_cone = 30*cm;//long cone
G4RotationMatrix* rm = new G4RotationMatrix();
rm->rotateY(180*deg);
G4double sshift=outerRadius_pmtL*0.5;



G4Sphere* photocathL = new G4Sphere("photocathL",outerRadius_pmtL-sphere_w,outerRadius_pmtL,0.*deg,360.*deg,0.*deg,60.*deg);

G4LogicalVolume* photocath_logL= new G4LogicalVolume(photocathL,
                                        G4Material::GetMaterial("Al"),
                                        "photocath_logL");
//*********************************************************************************
//PMTs first tank
//*********************************************************************************
G4VPhysicalVolume* photocatht1_phys = new G4PVPlacement(rm,G4ThreeVector(203.0*cm,203.0*cm,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht1",
                                                                              waterTank_log,false,10);

G4VPhysicalVolume* photocatht2_phys = new G4PVPlacement(rm,G4ThreeVector(-203.0*cm,203.0*cm,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht2",
                                                                              waterTank_log,false,11);

G4VPhysicalVolume* photocatht3_phys = new G4PVPlacement(rm,G4ThreeVector(203.0*cm,-203.0*cm,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht3",
                                                                              waterTank_log,false,12);

G4VPhysicalVolume* photocatht4_phys = new G4PVPlacement(rm,G4ThreeVector(-203.0*cm,-203.0*cm,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht4",
                                                                              waterTank_log,false,13);
//*********************************************************************************
//Aluminum cylinder
//*********************************************************************************
G4Tubs* AlCylinder = new G4Tubs("AlCylinder",
			scint_x/2.+wtyvek,
			scint_x/2.+wtyvek + d_mtl,
			scint_z/2.,
			0.*deg,
			360.*deg);//base
G4LogicalVolume* AlCylinder_log = new G4LogicalVolume(AlCylinder,G4Material::GetMaterial("Al"),"AlCylinder_log",0,0,0);   
//Tank 1
G4VPhysicalVolume* AlCylider_phys = new G4PVPlacement(0,
                                         G4ThreeVector(0,0,0),
                                         AlCylinder_log,
                                         "AlCylinder1",
                                         expHall_log,
                                         false,
                                         1);

//Tank 2
G4VPhysicalVolume* AlCylider_phys2 = new G4PVPlacement(0,
                                         G4ThreeVector(move_x2,move_y2,0),
                                         AlCylinder_log,
                                         "AlCylinder2",
                                         expHall_log,
                                         false,
                                         2);

//Tank 3
G4VPhysicalVolume* AlCylider_phys3 = new G4PVPlacement(0,
                                         G4ThreeVector(move_x3,move_y3,0),
                                         AlCylinder_log,
                                         "AlCylinder3",
                                         expHall_log,
                                         false,
                                         3);
//tanque central
G4Tubs* AlCylinder_c = new G4Tubs("AlCylinder_c",
                        scint_x/2.+wtyvek,
                        scint_x/2.+wtyvek + d_mtl,
                        Scint_z/2.,
                        0.*deg,
                        360.*deg);//base
G4LogicalVolume* AlCylinder_log_c = new G4LogicalVolume(AlCylinder_c,G4Material::GetMaterial("Al"),"AlCylinder_log_c",0,0,0);
G4VPhysicalVolume* AlCylider_phys4 = new G4PVPlacement(0,
                                         G4ThreeVector(move_x4,move_y4,1.6*m),
                                         AlCylinder_log_c,
                                         "AlCylinder4",
                                         expHall_log,
                                         false,
                                         4);

//PMTs en tanque central

G4VPhysicalVolume* photocatht1_phys_c = new G4PVPlacement(0,G4ThreeVector(106.0*cm,106.0*cm, - 1.98*m),
                                                                              photocath_logL,"photocatht1",
                                                                              waterTank_log_c,false,10);
G4VPhysicalVolume* photocatht2_phys_c = new G4PVPlacement(0,G4ThreeVector(-106.0*cm,106.0*cm, - 1.98*m),
                                                                              photocath_logL,"photocatht1",
                                                                              waterTank_log_c,false,11);
G4VPhysicalVolume* photocatht3_phys_c = new G4PVPlacement(0,G4ThreeVector(106.0*cm,-106.0*cm, - 1.98*m),
                                                                              photocath_logL,"photocatht1",
                                                                              waterTank_log_c,false,12);
G4VPhysicalVolume* photocatht4_phys_c = new G4PVPlacement(0,G4ThreeVector(-106.0*cm,-106.0*cm, - 1.98*m),
                                                                              photocath_logL,"photocatht1",
                                                                              waterTank_log_c,false,13);

//*********************************************************************************
/*
//*********************************************************************************
//PMTs second tank
//*********************************************************************************
G4VPhysicalVolume* photocatht21_phys = new G4PVPlacement(rm,G4ThreeVector(203.0*cm + move_x2,203.0*cm + move_y2,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht21",
                                                                              expHall_log,false,20);

G4VPhysicalVolume* photocatht22_phys = new G4PVPlacement(rm,G4ThreeVector(-203.0*cm + move_x2,203.0*cm + move_y2,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht22",
                                                                              expHall_log,false,21);

G4VPhysicalVolume* photocatht23_phys = new G4PVPlacement(rm,G4ThreeVector(203.0*cm + move_x2,-203.0*cm + move_y2,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht23",
                                                                              expHall_log,false,22);

G4VPhysicalVolume* photocatht24_phys = new G4PVPlacement(rm,G4ThreeVector(-203.0*cm + move_x2,-203.0*cm + move_y2,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht24",
                                                                              expHall_log,false,23);
//*********************************************************************************
//PMTs third tank
//*********************************************************************************


G4VPhysicalVolume* photocatht31_phys = new G4PVPlacement(rm,G4ThreeVector(203.0*cm + move_x3,203.0*cm + move_y3,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht31",
                                                                              expHall_log,false,30);

G4VPhysicalVolume* photocatht32_phys = new G4PVPlacement(rm,G4ThreeVector(-203.0*cm + move_x3,203.0*cm + move_y3,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht32",
                                                                              expHall_log,false,31);

G4VPhysicalVolume* photocatht33_phys = new G4PVPlacement(rm,G4ThreeVector(203.0*cm + move_x3,-203.0*cm + move_y3,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht33",
                                                                              expHall_log,false,32);

G4VPhysicalVolume* photocatht34_phys = new G4PVPlacement(rm,G4ThreeVector(-203.0*cm + move_x3,-203.0*cm + move_y3,scint_z/2.+sshift),
                                                                              photocath_logL,"photocatht34",
                                                                              expHall_log,false,33);*/


//******* PMT-Sensitive detector ********

/*
G4MultiFunctionalDetector* myScorer = new G4MultiFunctionalDetector("myPMTScorer");
G4SDManager::GetSDMpointer()->AddNewDetector(myScorer);
photocath_logL->SetSensitiveDetector(myScorer);
*/
/*
G4MultiFunctionalDetector* detector = new G4MultiFunctionalDetector("MyDetector");
G4SDManager::GetSDMpointer()->AddNewDetector(detector);
photocath_logL->SetSensitiveDetector(detector);
*/

/*G4VPrimitiveSensitivity* number = new G4PSNofCollision("NofCollision");
detector->Register(number);
*/

/*G4VSensitiveDetector* pSensitivePMT =  new MyDetector("/mydet");
G4SDManager* SDManager = G4SDManager::GetSDMpointer();
SDManager->AddNewDetector(pSensitivePMT);
photocath_logL->SetSensitiveDetector(pSensitivePMT);*/

  // PMT SD

  if (!fPmt_SD.Get()) {
    //Created here so it exists as pmts are being placed
    G4cout << "Construction /LXeDet/pmtSD" << G4endl;
    LXePMTSD* pmt_SD = new LXePMTSD("/LXeDet/pmtSD");
    fPmt_SD.Put(pmt_SD);
    pmt_SD->InitPMTs(16); //let pmtSD know # of pmts
    //pmt_SD->SetPmtPositions(fMainVolume->GetPmtPositions());
  }

    SetSensitiveDetector(photocath_logL, fPmt_SD.Get());



  // Scint SD

  if (!fScint_SD.Get()) {
    G4cout << "Construction /LXeDet/scintSD" << G4endl;
    LXeScintSD* scint_SD = new LXeScintSD("/LXeDet/scintSD");
    fScint_SD.Put(scint_SD);
  }
  SetSensitiveDetector(waterTank_log, fScint_SD.Get());

// ------------- Surfaces --------------
//

//
// Generate & Add Material Properties Table attached to the optical surfaces
//
  const G4int num = 2;
  G4double Ephoton[34] = {
4.960*eV,4.769*eV,4.428*eV,4.133*eV,
3.875*eV,3.647*eV,3.444*eV,3.351*eV,
3.263*eV,3.170*eV,3.100*eV,3.024*eV,
2.952*eV,2.883*eV,2.818*eV,2.755*eV,
2.695*eV,2.583*eV,2.530*eV,2.480*eV,
2.384*eV,2.296*eV,2.138*eV,2.066*eV,
2.00*eV,1.938*eV,1.879*eV,1.823*eV,
1.771*eV,1.722*eV,1.675*eV,1.631*eV,
1.590*eV,1.550*eV};
  //** Tyvek surface properties
  
  G4double TRef[34] ={
0.82,0.86,0.89,0.92,0.94,
0.95,0.95,0.95,0.96,0.96,
0.97,0.97,0.97,0.97,0.97,
0.97,0.97,0.97,0.97,0.97,
0.97,0.97,0.97,0.97,0.97,
0.97,0.97,0.97,0.97,0.97,
0.97,0.97,0.97,0.97}; 
//Tyvek
//
  G4MaterialPropertiesTable* optyvek = new G4MaterialPropertiesTable();
  optyvek->AddProperty("REFLECTIVITY", Ephoton, TRef,num);
  G4OpticalSurface* OpTyvekSurface =
  new G4OpticalSurface("TyvekSurface",unified,polished,dielectric_metal);
  OpTyvekSurface->SetMaterialPropertiesTable(optyvek);
  //**Create logical skin surfaces
  new G4LogicalSkinSurface("tyvekw_surface",tyvek_log,OpTyvekSurface);
  new G4LogicalSkinSurface("tyvekb_surface",tyvekbottom_log,OpTyvekSurface);
  new G4LogicalSkinSurface("tyvekt_surface",tyvektop_log,OpTyvekSurface);
  new G4LogicalSkinSurface("tyvekwlx_surface",tyvekwallshortx1_log, OpTyvekSurface);
  new G4LogicalSkinSurface("tyvekwlx_surface",tyvekwallshorty1_log, OpTyvekSurface);
//
//**Photocathode surface properties
//
  G4double photocath_EFF[num]={0.15,0.18}; //Enables 'detection' of photons
  G4double photocath_REFL[num]={0.,0.};
  G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
  photocath_mt->AddProperty("EFFICIENCY",Ephoton,photocath_EFF,num);
  photocath_mt->AddProperty("REFLECTIVITY",Ephoton,photocath_REFL,num);
  G4OpticalSurface* photocath_opsurf=
    new G4OpticalSurface("photocath_opsurf",glisur,polished,
                         dielectric_metal);
  photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

  new G4LogicalSkinSurface("photocath_surf",photocath_logL,photocath_opsurf);

//
// Water Tank
//
  G4OpticalSurface* opWaterSurface = new G4OpticalSurface("WaterSurface");
  opWaterSurface->SetType(dielectric_dielectric);
  opWaterSurface->SetFinish(ground);
  opWaterSurface->SetModel(unified);

  new G4LogicalBorderSurface("WaterSurface",
                                 waterTank_phys,expHall_phys,opWaterSurface);

// Air Bubble
//
  G4OpticalSurface* opAirSurface = new G4OpticalSurface("AirSurface");
  opAirSurface->SetType(dielectric_dielectric);
  opAirSurface->SetFinish(polished);
  opAirSurface->SetModel(glisur);

  G4LogicalSkinSurface* airSurface =
          new G4LogicalSkinSurface("AirSurface", bubbleAir_log, opAirSurface);

  G4OpticalSurface* opticalSurface = dynamic_cast <G4OpticalSurface*>
        (airSurface->GetSurface(bubbleAir_log)->GetSurfaceProperty());

  if (opticalSurface) opticalSurface->DumpInfo();

//
// Generate & Add Material Properties Table attached to the optical surfaces
//
  G4double ephoton[num] = {2.034*eV, 4.136*eV};

  //OpticalWaterSurface
  G4double refractiveIndex[num] = {1.35, 1.40};
  G4double specularLobe[num]    = {0.3, 0.3};
  G4double specularSpike[num]   = {0.2, 0.2};
  G4double backScatter[num]     = {0.2, 0.2};

  G4MaterialPropertiesTable* myST1 = new G4MaterialPropertiesTable();

  myST1->AddProperty("RINDEX",                ephoton, refractiveIndex, num);
  myST1->AddProperty("SPECULARLOBECONSTANT",  ephoton, specularLobe,    num);
  myST1->AddProperty("SPECULARSPIKECONSTANT", ephoton, specularSpike,   num);
  myST1->AddProperty("BACKSCATTERCONSTANT",   ephoton, backScatter,     num);

  G4cout << "Water Surface G4MaterialPropertiesTable" << G4endl;
  myST1->DumpTable();

  opWaterSurface->SetMaterialPropertiesTable(myST1);

  //OpticalAirSurface
  G4double reflectivity[num] = {0.3, 0.5};
  G4double efficiency[num]   = {0.8, 1.0};

  G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();

  myST2->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
  myST2->AddProperty("EFFICIENCY",   ephoton, efficiency,   num);

  G4cout << "Air Surface G4MaterialPropertiesTable" << G4endl;
  myST2->DumpTable();

  opAirSurface->SetMaterialPropertiesTable(myST2);

G4VisAttributes* water_va= new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)) ;  // cyan
waterTank_log -> SetVisAttributes(water_va);
water_va -> SetForceAuxEdgeVisible (true);//visualización del cilindro
water_va -> SetForceWireframe(true);
water_va -> SetForceSolid(false);
water_va -> SetVisibility(true);

//central
G4VisAttributes* water_va_c= new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)) ;  // cyan
waterTank_log_c -> SetVisAttributes(water_va_c);
water_va_c -> SetForceAuxEdgeVisible (true);//visualización del cilindro
water_va_c -> SetForceWireframe(true);
water_va_c -> SetForceSolid(false);
water_va_c -> SetVisibility(true);

//always return the physical World


G4VisAttributes* photocath_vaL= new G4VisAttributes(G4Colour::Yellow()) ;  // cyan
photocath_logL -> SetVisAttributes(photocath_vaL);
photocath_vaL-> SetForceSolid(true);
photocath_vaL-> SetVisibility(true);


G4VisAttributes* tyvek_blue= new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)) ;  // cyan
tyvek_log -> SetVisAttributes(tyvek_blue);
tyvek_blue-> SetForceSolid(true);
tyvek_blue-> SetVisibility(true);

//central
/*G4VisAttributes* tyvek_blue_c= new G4VisAttributes(G4Colour(0.0, 1.0, 1.0)) ;  // cyan
tyvek_log_c -> SetVisAttributes(tyvek_blue_c);
tyvek_blue_c-> SetForceSolid(true);
tyvek_blue_c-> SetVisibility(true);
*/


G4VisAttributes* tyvek_shortx1_gray= new G4VisAttributes(G4Colour::White()) ;  // cyan
tyvekwallshortx1_log -> SetVisAttributes(tyvek_shortx1_gray);
tyvek_shortx1_gray-> SetForceAuxEdgeVisible (true);//visualización de pared
tyvek_shortx1_gray-> SetForceWireframe(true);
tyvek_shortx1_gray-> SetForceSolid(true);
tyvek_shortx1_gray-> SetVisibility(true);

G4VisAttributes* tyvek_shorty1_gray= new G4VisAttributes(G4Colour::White()) ;  // cyan
tyvekwallshorty1_log -> SetVisAttributes(tyvek_shorty1_gray);
tyvek_shorty1_gray-> SetForceAuxEdgeVisible (true);//visualización de pared
tyvek_shorty1_gray-> SetForceWireframe(true);
tyvek_shorty1_gray-> SetForceSolid(true);
tyvek_shorty1_gray-> SetVisibility(true);


  return expHall_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
