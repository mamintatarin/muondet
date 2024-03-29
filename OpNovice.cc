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
/// \file OpNovice/OpNovice.cc
/// \brief Main program of the OpNovice example
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// Description: Test of Continuous Process G4Cerenkov
//              and RestDiscrete Process G4Scintillation
//              -- Generation Cerenkov Photons --
//              -- Generation Scintillation Photons --
//              -- Transport of optical Photons --
// Version:     5.0
// Created:     1996-04-30
// Author:      Juliet Armstrong
// mail:        gum@triumf.ca
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <G4Types.hh>
#include "globals.hh"
#include <CLHEP/Vector/ThreeVector.h>
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"

#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "OpNoviceDetectorConstruction.hh"
#include "OpNoviceActionInitialization.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include <Logger.hh>
#include "G4SystemOfUnits.hh"
#include "iostream"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " OpNovice [-m macro ] [-u UIsession] [-t nThreads] [-r seed] "
           << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

    std::cout << "double:\t\t" << sizeof(double) << " bytes" << std::endl;
    std::cout << "int:\t\t" << sizeof(int) << " bytes" << std::endl;
    std::cout << "argc " << argc << " value" << std::endl;
  // Evaluate arguments
  //
 

  G4String macro;
  G4String session;
  G4double width = 5*cm;
  G4double height = 10*cm;


  G4long myseed = 345354;
  for ( G4int i=1; i<argc; i=i+2 ) {
     if      ( G4String(argv[i]) == "-m" ) macro   = argv[i+1];
     else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
     else if ( G4String(argv[i]) == "-r" ) myseed  = atoi(argv[i+1]);
     else if ( G4String(argv[i]) == "-width" ) width  = atof(argv[i+1])*cm;
     else if ( G4String(argv[i]) == "-height" ) height  = atof(argv[i+1])*cm;

    else {
      PrintUsage();
      return 1;
    }
  }
   
    Logger::instance()->print(("Width: "+std::to_string(width)).c_str());
    Logger::instance()->print(("Height: "+std::to_string(height)).c_str());
  // Instantiate G4UIExecutive if interactive mode
  G4UIExecutive* ui = nullptr;
  if ( macro.size() == 0 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  //

  G4RunManager * runManager = new G4RunManager;


  // Seed the random number generator manually
  G4Random::setTheSeed(myseed);

  // Set mandatory initialization classes
  //
  // Detector construction
  OpNoviceDetectorConstruction* detectorConstruction=new OpNoviceDetectorConstruction(width,height);
  runManager-> SetUserInitialization(detectorConstruction);
  // Physics list

  G4VModularPhysicsList* physicsList = new FTFP_BERT();
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();



  physicsList->RegisterPhysics(opticalPhysics);
  runManager-> SetUserInitialization(physicsList);

  // User action initialization
  OpNoviceActionInitialization* act_init = new OpNoviceActionInitialization(width,height);
 
  runManager->SetUserInitialization(act_init);


  // Initialize visualization
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if ( macro.size() ) {
     // Batch mode
     G4String command = "/control/execute ";
     UImanager->ApplyCommand(command+macro);
  }
  else // Define UI session for interactive mode
  {
     UImanager->ApplyCommand("/control/execute ../vis.mac");
     if (ui->IsGUI())
        UImanager->ApplyCommand("/control/execute ../gui.mac");
     ui->SessionStart();
     delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  delete visManager;
    delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
