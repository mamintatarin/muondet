#ifndef OpNoviceActionInitialization_h
#define OpNoviceActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include <G4INCLThreeVector.hh>
#include "globals.hh"
#include <CLHEP/Vector/ThreeVector.h>
class B4DetectorConstruction;

/// Action initialization class.
///

class OpNoviceActionInitialization : public G4VUserActionInitialization
{
  public:
    OpNoviceActionInitialization(CLHEP::Hep3Vector pos,G4double tempwidth,G4double tempheight);
    virtual ~OpNoviceActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
public:
    CLHEP::Hep3Vector position;
    G4double width;
    G4double height;
};

#endif