//Programa que sólo agrega los datos al archivo dato.dat para evitar el geant4 de LAGO

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;




int main(){


    ifstream FILE("ya.dat",ios::in);
    ofstream dat("dato.dat",ios::app);

float area, pEnergy, angle, a, b, c, d, e;

area = 0;     
angle = 0;
pEnergy = 0;	


while (FILE>>area>>pEnergy>>angle>>a>>b>>c>>d>>e){
   dat<<area<<"	"<<pEnergy<<"	"<<angle<<"	"<<"0"<<endl;}

   dat.close();    
   FILE.close();    
        
    return 0;
}

