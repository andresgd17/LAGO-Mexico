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

#ifndef OpNoviceDetectorConstruction_h
#define OpNoviceDetectorConstruction_h 1

#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4VUserDetectorConstruction.hh"

#include "LXePMTSD.hh"
#include "LXeScintSD.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class OpNoviceDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    OpNoviceDetectorConstruction();
    virtual ~OpNoviceDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

  private:
    G4double fExpHall_x;
    G4double fExpHall_y;
    G4double fExpHall_z;

    G4double fTank_x;
    G4double fTank_y;
    G4double fTank_z;

    G4double fBubble_x;
    G4double fBubble_y;
    G4double fBubble_z;

    G4double scint_x;
    G4double scint_y;
    G4double scint_z;
    G4double Scint_z;

    G4double outerRadius_pmtL;
    G4double outerRadius_pmtC;

    G4double move_x1;
    G4double move_y1;

    G4double move_x2;
    G4double move_y2;

    G4double move_x3;
    G4double move_y3;

    G4double move_x4;
    G4double move_y4;

    G4double wtyvek;
    G4double d_mtl;

    //Sensitive Detectors
    G4Cache<LXeScintSD*> fScint_SD;
    G4Cache<LXePMTSD*> fPmt_SD;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*OpNoviceDetectorConstruction_h*/
