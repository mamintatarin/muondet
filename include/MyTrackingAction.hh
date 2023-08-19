//---------------------------------------------------------------
// G4UserTrackingAction.hh
//
// Description:
// This class represents actions taken place by the user at
// the start/end point of processing one track.
//---------------------------------------------------------------
#include <G4TrackingManager.hh>
#include <dataOpt.hh>
#include <DataFile.hh>
class G4UserTrackingAction
{
   public:

     // Constructor & Destructor
     G4UserTrackingAction(){};
     virtual ~G4UserTrackingAction(){}

     // Member functions
     virtual void PreUserTrackingAction(const G4Track*){}
     virtual void PostUserTrackingAction(const G4Track*){}

   protected:

    G4TrackingManager* fpTrackingManager;
};