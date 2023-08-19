#ifndef OpNoviceEventAction_h
#define OpNoviceEventAction_h 1

#include "G4Event.hh"
#include <G4UserEventAction.hh>
#include "OpNoviceSteppingAction.hh"
#include "OpNoviceRunAction.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


class G4Event;

class OpNoviceEventAction: public G4UserEventAction
{
  public:
    OpNoviceEventAction(OpNoviceSteppingAction * stepact,G4double width, G4double height);
    virtual ~OpNoviceEventAction();
    OpNoviceSteppingAction * tempStepping;

    G4double width;
    G4double height;
  public:
    virtual void BeginOfEventAction(const G4Event* Event);
    virtual void EndOfEventAction(const G4Event* Event);
    SensitiveDetectorParticleDataOpt data;
    DataFile<SensitiveDetectorParticleDataOpt>* foutMuons;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*OpNoviceEventAction_h*/