#include <dataOpt.hh>
#include <stdio.h>
#include "G4SystemOfUnits.hh"


using namespace CLHEP;





void FillParticleDataOpt(SensitiveDetectorParticleDataOpt &data, G4double z,
                         G4double energy1,G4double energy2) {
    data.z=z;
    data.energy1 = energy1;
    data.energy2 = energy2;
}








