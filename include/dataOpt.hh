/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   struct.hh
 * Author: mihail
 *
 * Created on 20 марта 2017 г., 0:05
 */

#ifndef STRUCTOPT_HH
#define STRUCTOPT_HH


#include <string>
#include "G4RunManager.hh"
#include "globals.hh"
#include <CLHEP/Vector/ThreeVector.h>
#include <stdio.h>

using namespace std;




struct SensitiveDetectorParticleDataOpt{
    double width;
    double height;
    double energy1;
    double energy2;
    double z;
};





void FillParticleDataOpt(SensitiveDetectorParticleDataOpt &data,  G4double width, G4double height,
                         G4double energy1,G4double energy2,G4double z);





#endif /* STRUCTOPT_HH */

