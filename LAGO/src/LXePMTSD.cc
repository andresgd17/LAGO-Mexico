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
// $Id: LXePMTSD.cc 73915 2013-09-17 07:32:26Z gcosmo $
//
/// \file optical/LXe/src/LXePMTSD.cc
/// \brief Implementation of the LXePMTSD class
//
//
#include "LXePMTSD.hh"
#include "LXePMTHit.hh"
#include "OpNoviceDetectorConstruction.hh"
#include "LXeUserTrackInformation.hh"
#include "OpNovicePrimaryGeneratorAction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXePMTSD::LXePMTSD(G4String name)
  : G4VSensitiveDetector(name),fPMTHitCollection(0),fPMTPositionsX(0)
  ,fPMTPositionsY(0),fPMTPositionsZ(0)
{
  collectionName.insert("pmtHitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXePMTSD::~LXePMTSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void LXePMTSD::SetPmtPositions(const std::vector<G4ThreeVector>& positions)
{
  for (G4int i=0; i<G4int(positions.size()); ++i) {
    if(fPMTPositionsX)fPMTPositionsX->push_back(positions[i].x());
    if(fPMTPositionsY)fPMTPositionsY->push_back(positions[i].y());
    if(fPMTPositionsZ)fPMTPositionsZ->push_back(positions[i].z());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePMTSD::Initialize(G4HCofThisEvent* hitsCE){
  fPMTHitCollection = new LXePMTHitsCollection
                      (SensitiveDetectorName,collectionName[0]);
  //Store collection with event and keep ID
  static G4int hitCID = -1;
  if(hitCID<0){
    hitCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitCID, fPMTHitCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool LXePMTSD::ProcessHits(G4Step* ,G4TouchableHistory* ){
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode

G4bool LXePMTSD::ProcessHits_constStep(const G4Step* aStep,
                                       G4TouchableHistory* ){
										   

  //need to know if this is an optical photon
  if(aStep->GetTrack()->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;
 
  //User replica number 1 since photocathode is a daughter volume
  //to the pmt which was replicated
  G4double t1= aStep->GetPostStepPoint()->GetGlobalTime();
  G4int pmtNumber=
    aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber();
  G4VPhysicalVolume* physVol=
    aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1);


  //How far is the PE from the PMT's center    
  G4int repNo =
    aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber();

  G4int tankNo =
    aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1)->GetCopyNo();

  double posX = 
    aStep->GetPostStepPoint()->GetPosition().x();
 
 double posY =
    aStep->GetPostStepPoint()->GetPosition().y();
//in cm
posX = posX/10.;
posY = posY/10.;

//move PE from tank 2 & 3
 if(tankNo==2){posX = posX - 2474.;}
 if(tankNo==3){posX = posX - 1756.;
               posY = posY + 2164.;}
 if(tankNo==4){posX = posX -1410.;
               posY = posY + 722.;

}

//get distance from PE's position to PMT's center

posX = abs(posX);
posY = abs(posY);

double radi;

if (tankNo==4)
{
    cout<<posX<<" " <<posY<<endl;
    radi = sqrt((posX-106.)*(posX-106.)+(posY-106.)*(posY-106.));
}
else {radi = sqrt((posX-203.)*(posX-203.)+(posY-203.)*(posY-203.));}

  ifstream inc("ya.dat",ios::in);
  ofstream dat("dato.dat",ios::app);

         if (inc.good())
        {  string sLine;
           getline(inc, sLine);
            dat<<sLine;}
  //G4cout<<tankNo<<"	"<<repNo<<"	"<<posX<<"	"<<posY<<G4endl;
  dat<<" "<<tankNo<<repNo<<" "<<radi<<G4endl;      
           
  //Find the correct hit collection
  G4int n=fPMTHitCollection->entries();
  LXePMTHit* hit=NULL;
  //ofstream cosa("time.txt",ios::app);
  //G4double test = t1/ns;
  //cosa<<tankNo<<" "<<repNo<<" "<<test<<G4endl;
  for(G4int i=0;i<n;i++){
    if((*fPMTHitCollection)[i]->GetPMTNumber()==pmtNumber){
      hit=(*fPMTHitCollection)[i];
      break;
    }
  }
 
  if(hit==NULL){//this pmt wasnt previously hit in this event
    hit = new LXePMTHit(); //so create new hit
    hit->SetPMTNumber(pmtNumber);
    hit->SetPMTPhysVol(physVol);
    hit->SetTime(t1);
    fPMTHitCollection->insert(hit);
    hit->SetPMTPos((*fPMTPositionsX)[pmtNumber],(*fPMTPositionsY)[pmtNumber],
                   (*fPMTPositionsZ)[pmtNumber]);
  }

  hit->IncPhotonCount(); //increment hit for the selected pmt
 
 /* if(!LXeDetectorConstruction::GetSphereOn()){
    hit->SetDrawit(true);
    //If the sphere is disabled then this hit is automaticaly drawn
  }
  else{//sphere enabled
    LXeUserTrackInformation* trackInfo=
      (LXeUserTrackInformation*)aStep->GetTrack()->GetUserInformation();
    if(trackInfo->GetTrackStatus()&hitSphere)
      //only draw this hit if the photon has hit the sphere first
      hit->SetDrawit(true);
  }*/

  return true;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePMTSD::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePMTSD::clear() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePMTSD::DrawAll() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePMTSD::PrintAll() {}
