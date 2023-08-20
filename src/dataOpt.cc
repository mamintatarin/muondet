#include <dataOpt.hh>
#include <stdio.h>
#include "G4SystemOfUnits.hh"


using namespace CLHEP;





void FillParticleDataOpt(SensitiveDetectorParticleDataOpt &data,  G4double width, G4double height,
                         G4double energy1,G4double energy2,G4double z) {

    data.width = width;
    data.height=height;
    data.energy1 = energy1;
    data.energy2 = energy2;
    data.z = z;
}








