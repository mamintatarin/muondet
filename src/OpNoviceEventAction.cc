#include "OpNoviceEventAction.hh"
#include "OpNoviceSteppingAction.hh"
#include "stdio.h"
#include "DataFileManager.hh"
#include "dataOpt.hh"
#include <G4SystemOfUnits.hh>

OpNoviceEventAction::OpNoviceEventAction(CLHEP::Hep3Vector pos,OpNoviceSteppingAction* stepact, G4double tempwidth, G4double tempheight)
        : G4UserEventAction()
{
    std::string filename = "Muons";
    foutMuons= DataFileManager::instance()->getDataFile<SensitiveDetectorParticleDataOpt>(filename);
    position = pos;
    tempStepping = stepact;
    width = tempwidth;
    height = tempheight;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceEventAction::~OpNoviceEventAction()
{
}
void OpNoviceEventAction::EndOfEventAction(const G4Event* Event)
{


    FillParticleDataOpt(data,position.getZ(),width,height,
                        tempStepping->results[0],tempStepping->results[1]);

    foutMuons->addData(data);


}

void OpNoviceEventAction::BeginOfEventAction(const G4Event* Event)
{
    tempStepping->results[0]=0.0;
    tempStepping->results[1]=0.0;
}