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

#include "OpNovicePrimaryGeneratorAction.hh"
#include "OpNovicePrimaryGeneratorMessenger.hh"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "globals.hh"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string>
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNovicePrimaryGeneratorAction::OpNovicePrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), 
   fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
/*
  //create a messenger for this class
  fGunMessenger = new OpNovicePrimaryGeneratorMessenger(this);

  //default kinematic
  //
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("e+");

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleTime(0.0*ns);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0*m,0.0*m,2.6*m));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  fParticleGun->SetParticleEnergy(100.0*keV);*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNovicePrimaryGeneratorAction::~OpNovicePrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNovicePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{


   //G4double trad;
    //trad=0.0;
    G4double fTEnergy;
    G4int nl=0;
    G4int nl2=0;
    ifstream inc("ya.dat",ios::in);
    ofstream dat("dato.dat",ios::app);
    if ( !inc ){
    G4cout<<"Error al abrir archivo de datos"<<G4endl;
    /*  G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);
    */
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

    G4String particleName;
    fParticleGun->SetParticleDefinition(particleTable->
                        FindParticle(particleName="mu-"));
    fParticleGun->SetParticleEnergy(10000*MeV);
    fParticleGun->SetParticlePosition(G4ThreeVector(0.0*m,0.0*m,2.6*m));;// vector anterior:(1748.0*cm , 1115.0*cm, 2.51*m)
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
    fParticleGun->GeneratePrimaryVertex(anEvent);
    }
    else {  G4cout<<"Archivo de datos abierto exitosamente"<<G4endl;
      G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
         G4String particleName;

         while (inc>>fPartId>>fUx>>fUy>>fUz>>fX>>fY>>fTimeDelay>>fTEnergy)
        {   if ((nl2==0) && (fPartId == 142219)){
            fPrimEnergy = fUx;
            fangle = fUy; 
            fPhi = fUz;
            fxCore = fX;
            fyCore = fY;
            dat<<"142219"<<" "<<fPrimEnergy<<" "<<fangle<<" "<<fPhi<<" "<<fxCore<<" "<<fyCore<<" 0 0 0 0"<<endl;}

          //	if ((abs(fX-trad)<1.20)) G4cout<<setprecision(7)<<fX<<" "<<fY<<G4endl;
          nl2++;
          //if ((fX>-500.0) && (fX<500.0)){
          //    if ((fY>-500.0) && (fY<500.0)){

            nl++;
            //if (fX > 0) fX=fX-trad;
            //if (fX < 0) fX=fX+trad;
          if ( fPartId == 1)
            {
              particleName = "gamma";
            }
          else if ( fPartId == 2 )
            {                       //e+
              particleName = "e+";
            }
          else if ( fPartId == 3 )
            {                       //e-
              particleName = "e-";
            }
          else if ( fPartId == 5 )
            {                       //mu+
              particleName = "mu+";
            }
          else if ( fPartId == 6 )
            {                       //mu-
              particleName = "mu-";
            }
          else//do not simulate other particles
            continue;
          fZ=450;//posición inicial de la partícula en z


          G4double ttime;
          ttime = fTimeDelay;
          //	G4cout<<"Delay  "<<fTimeDelay<<"  ttime  "<<ttime<<G4endl;
          G4ThreeVector
          direction ( fUx*GeV, fUy*GeV, -fUz*GeV );
          G4ThreeVector
            position ( fX*cm, fY*cm, fZ*cm );
          //	G4cout<<"PartiName= "<<particleName<<"   "<<fUx<<G4endl;

          fParticleGun->SetParticleDefinition ( particleTable->FindParticle ( particleName ) );
                  fParticleGun->SetParticleMomentumDirection ( direction );
                  fParticleGun->SetParticlePosition ( position );
                  fParticleGun->SetParticleEnergy ( fTEnergy*GeV );
          fParticleGun->SetParticleTime ( ttime*ns );
          fParticleGun->GeneratePrimaryVertex(anEvent);
          //} //if Y
          //} //if X
        }  //while
         G4cout<<"Total de eventos "<<nl2<<G4endl;
         G4cout<<"Eventos dentro del Hall "<<nl<<G4endl;
         
         
    }


    //  fParticleGun->GeneratePrimaryVertex(anEvent);


dat.close();

  //fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNovicePrimaryGeneratorAction::SetOptPhotonPolar()
{
 G4double angle = G4UniformRand() * 360.0*deg;
 SetOptPhotonPolar(angle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNovicePrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
 if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton")
   {
     G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
               "the fParticleGun is not an opticalphoton" << G4endl;
     return;
   }

 G4ThreeVector normal (1., 0., 0.);
 G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
 G4ThreeVector product = normal.cross(kphoton);
 G4double modul2       = product*product;
 
 G4ThreeVector e_perpend (0., 0., 1.);
 if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
 G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
 
 G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
 fParticleGun->SetParticlePolarization(polar);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
