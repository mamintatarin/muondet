#ifndef OpNovicePrimaryGeneratorAction_h
#define OpNovicePrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include <CLHEP/Vector/ThreeVector.h>
#include <G4GeneralParticleSource.hh>

class G4ParticleGun;
class G4Event;
class OpNovicePrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class OpNovicePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    OpNovicePrimaryGeneratorAction();
    virtual ~OpNovicePrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);


  private:
    G4GeneralParticleSource* fParticleSource;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*OpNovicePrimaryGeneratorAction_h*/