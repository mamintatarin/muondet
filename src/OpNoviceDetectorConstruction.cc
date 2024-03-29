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
/// \file OpNovice/src/OpNoviceDetectorConstruction.cc
/// \brief Implementation of the OpNoviceDetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "OpNoviceDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include <Logger.hh>
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::OpNoviceDetectorConstruction(G4double tempwidth, G4double tempheight)
 : G4VUserDetectorConstruction()
{
  width=tempwidth;
  height=tempheight;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::~OpNoviceDetectorConstruction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* OpNoviceDetectorConstruction::Construct()
{


        // Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();
  

        G4bool isotopes = true;
  
        G4double ph_Energy[]    = { 1.0*eV , 7.07*eV};
   
        G4double RefractiveIndex_scint[] = {1.58,1.58};
        G4double Absorlen = 210*cm;
      
        G4double RefractiveIndex_meltmount[] = {1.5,1.5};

        G4Material* scintillator_mat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
        G4MaterialPropertiesTable* scintillator_prop_table = new G4MaterialPropertiesTable();


        G4Material* meltmount = new G4Material("meltmount", 1*g/cm3, 3);
        G4Element*  C = nist->FindOrBuildElement("C" , isotopes);
        G4Element* H = nist->FindOrBuildElement("H", isotopes);
        G4Element* O = nist->FindOrBuildElement("O", isotopes);
        meltmount->AddElement(C,1);
        meltmount->AddElement(H,1);
        meltmount->AddElement(O,1);



        G4MaterialPropertiesTable* meltmount_prop = new G4MaterialPropertiesTable();
        meltmount_prop ->AddProperty("RINDEX",ph_Energy,RefractiveIndex_meltmount,2);


        G4double distrEn[]={2.47*eV, 2.5*eV};
        G4double distrF[]={0.5,0.5};

        //https://arxiv.org/pdf/1804.08975.pdf
        //https://www.crystals.saint-gobain.com/radiation-detection-scintillators/plastic-scintillators/bc400-bc404-bc408-bc412-bc416
        //https://www.crystals.saint-gobain.com/radiation-detection-scintillators/plastic-scintillators
        //https://www.hep.phy.cam.ac.uk/~lester/teaching/SparkChamber/SGC_BC400_404_408_412_416_Data_Sheet.pdf
        G4double resolution_factor=1; //TODO
        scintillator_prop_table->AddConstProperty("SCINTILLATIONYIELD",8000./MeV);//BC-408?
        scintillator_prop_table->AddProperty("RINDEX", ph_Energy,RefractiveIndex_scint,2);
        scintillator_prop_table->AddConstProperty("ABSLENGTH",Absorlen);
        scintillator_prop_table->AddConstProperty("RESOLUTIONSCALE",resolution_factor); 
        /*Параметр, характеризующий статистический 
        разброс числа рожденных фотонов. На это
        число умножается вычисленная по закону 
        Гаусса статистическая ошибка: 
        S = ResolutionScale*sqrt(Edep*Yield).
        */
        scintillator_prop_table->AddConstProperty("YIELDRATIO",0.99);//???
        scintillator_prop_table->AddConstProperty("FASTTIMECONSTANT", 4*ns);
        scintillator_prop_table->AddConstProperty("SLOWTIMECONSTANT", 10*ns);
        scintillator_prop_table->AddProperty("FASTCOMPONENT",distrEn,distrF,2);
        scintillator_prop_table->AddProperty("SLOWCOMPONENT",distrEn,distrF,2);

        scintillator_mat->SetMaterialPropertiesTable(scintillator_prop_table);
        meltmount->SetMaterialPropertiesTable(meltmount_prop);


        // Option to switch on/off checking of volumes overlaps
        //
        G4bool checkOverlaps = true;
    
    
        

  
        G4double world_sizeXY = 250*cm;
        G4double world_sizeZ  = 250*cm;
        G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
        G4Material* det_mat = nist->FindOrBuildMaterial("G4_Si");
        G4MaterialPropertiesTable* GalacticSC = new G4MaterialPropertiesTable();
        G4MaterialPropertiesTable* detTable = new G4MaterialPropertiesTable();
        G4double refrWorld[]={1,1};
        G4double refrDet[]={3.88163,3.88163};
        GalacticSC->AddProperty("RINDEX",ph_Energy,refrWorld,2);
        detTable->AddProperty("RINDEX",ph_Energy,refrDet,2);
        world_mat ->SetMaterialPropertiesTable(GalacticSC);
        det_mat->SetMaterialPropertiesTable(detTable);






        //World
        G4Box* solidWorld =
        new G4Box("World",                       //its name
        world_sizeXY, world_sizeXY, world_sizeZ);     //its size
        
        G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld,          //its solid
                                world_mat,           //its material
                                "World");            //its name
                                        
        G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0,                     //no rotation
                        G4ThreeVector(),       //at (0,0,0)
                        logicWorld,            //its logical volume
                        "World",               //its name
                        0,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
                     

        //
        // Scintillator
        //
        G4double scintillator_sizeXY = this->width/2;
        G4double scintillator_sizeZ  = this->height/2;
        
        
        G4Box* solidScintillator =
        new G4Box("Scintillator",                       //its name
        scintillator_sizeXY, scintillator_sizeXY, scintillator_sizeZ);     //its size
        
        G4LogicalVolume* logicScintillator =
        new G4LogicalVolume(solidScintillator,          //its solid
                                scintillator_mat,           //its material
                                "Scintillator");            //its name
                                        
        G4VPhysicalVolume* Scintillator =
        new G4PVPlacement(0,                     //no rotation
                        G4ThreeVector(),       //at (0,0,0)
                        logicScintillator,            //its logical volume
                        "Scintillator",               //its name
                        logicWorld,                     //its mother  volume
                        false,                 //no boolean operation
                        0,                     //copy number
                        checkOverlaps);        //overlaps checking
 
        //side borders

        
        G4LogicalVolume* logicDegree1 =
                new G4LogicalVolume(solidScintillator,          //its solid
                                        world_mat,           //its material
                                        "Degree1");            //its name

        G4VPhysicalVolume* Degree1 =
                new G4PVPlacement(0,                     //no rotation
                                G4ThreeVector(scintillator_sizeXY*2,0.0*cm,0.0*cm),
                                logicDegree1,            //its logical volume
                                "Degree1",               //its name
                                logicWorld,                     //its mother  volume
                                false,                 //no boolean operation
                                0,                     //copy number
                                checkOverlaps);        //overlaps checking

        G4LogicalVolume* logicDegree2 =
                new G4LogicalVolume(solidScintillator,          //its solid
                                        world_mat,           //its material
                                        "Degree2");            //its name

        G4VPhysicalVolume* Degree2 =
                new G4PVPlacement(0,                     //no rotation
                                G4ThreeVector(-scintillator_sizeXY*2,0.0*cm,0.0*cm),
                                logicDegree2,            //its logical volume
                                "Degree2",               //its name
                                logicWorld,                     //its mother  volume
                                false,                 //no boolean operation
                                0,                     //copy number
                                checkOverlaps);        //overlaps checking
        G4LogicalVolume* logicDegree3 =
                new G4LogicalVolume(solidScintillator,          //its solid
                                        world_mat,           //its material
                                        "Degree3");            //its name

        G4VPhysicalVolume* Degree3 =
                new G4PVPlacement(0,                     //no rotation
                                G4ThreeVector(0*cm,scintillator_sizeXY*2,0.0*cm),
                                logicDegree3,            //its logical volume
                                "Degree3",               //its name
                                logicWorld,                     //its mother  volume
                                false,                 //no boolean operation
                                0,                     //copy number
                                checkOverlaps);        //overlaps checking

        G4LogicalVolume* logicDegree4 =
                new G4LogicalVolume(solidScintillator,          //its solid
                                        world_mat,           //its material
                                        "Degree4");            //its name

        G4VPhysicalVolume* Degree4 =
                new G4PVPlacement(0,                     //no rotation
                                G4ThreeVector(0*cm,-scintillator_sizeXY*2,0.0*cm),       //at (0,0,0)
                                logicDegree4,            //its logical volume
                                "Degree4",               //its name
                                logicWorld,                     //its mother  volume
                                false,                 //no boolean operation
                                0,                     //copy number
                                checkOverlaps);        //overlaps checking
      

        

        //downside detector
        G4double detector_z_size = 0.5*cm;
        G4double meltmount_z_size = 0.1*cm;
        

        



        G4Box* solidMeltmount =
                new G4Box("Meltmount",                       //its name
                        scintillator_sizeXY, scintillator_sizeXY, meltmount_z_size);     //its size
        G4LogicalVolume* logicMeltmount1 =
                new G4LogicalVolume(solidMeltmount,          //its solid
                                        meltmount,           //its material
                                        "Meltmount1");            //its name

        G4VPhysicalVolume* Meltmount1 =
                new G4PVPlacement(0,                     //no rotation
                                G4ThreeVector(0,0,-scintillator_sizeZ-meltmount_z_size),       //at (0,0,0)
                                logicMeltmount1,            //its logical volume
                                "Meltmount1",               //its name
                                logicWorld,                     //its mother  volume
                                false,                 //no boolean operation
                                0,                     //copy number
                                checkOverlaps);        //overlaps checking
        






        G4LogicalVolume* logicMeltmount2 =
                new G4LogicalVolume(solidMeltmount,          //its solid
                                        meltmount,           //its material
                                        "Meltmount2");            //its name

        G4VPhysicalVolume* Meltmount2 =
                new G4PVPlacement(0,                     //no rotation
                                G4ThreeVector(0,0,scintillator_sizeZ+meltmount_z_size),       //at (0,0,0)
                                logicMeltmount2,            //its logical volume
                                "Meltmount2",               //its name
                                logicWorld,                     //its mother  volume
                                false,                 //no boolean operation
                                0,                     //copy number
                                checkOverlaps);        //overlaps checking


        G4Box* solidDetector =
                new G4Box("Detector",                       //its name
                        scintillator_sizeXY, scintillator_sizeXY, detector_z_size);     //its size
        G4LogicalVolume* logicDetector1 =
                new G4LogicalVolume(solidDetector,          //its solid
                                        det_mat,           //its material
                                        "Detector1");            //its name

        G4VPhysicalVolume* Detector1 =
                new G4PVPlacement(0,                     //no rotation
                                G4ThreeVector(0,0,-scintillator_sizeZ-detector_z_size-meltmount_z_size*2),       //at (0,0,0)
                                logicDetector1,            //its logical volume
                                "Detector1",               //its name
                                logicWorld,                     //its mother  volume
                                false,                 //no boolean operation
                                0,                     //copy number
                                checkOverlaps);        //overlaps checking
        //upper detector
        G4LogicalVolume* logicDetector2 =
                new G4LogicalVolume(solidDetector,          //its solid
                                        det_mat,           //its material
                                        "Detector2");            //its name

        G4VPhysicalVolume* Detector2 =
                new G4PVPlacement(0,                     //no rotation
                                G4ThreeVector(0,0,scintillator_sizeZ+detector_z_size+meltmount_z_size*2),       //at (0,0,0)
                                logicDetector2,            //its logical volume
                                "Detector2",               //its name
                                logicWorld,                     //its mother  volume
                                false,                 //no boolean operation
                                0,                     //copy number
                                checkOverlaps);        //overlaps checking

        

   
        //surfaces
        
        G4OpticalSurface* OpSurface1 = new G4OpticalSurface("side1");
        G4LogicalBorderSurface* Surface1 = new
                G4LogicalBorderSurface("side1",Scintillator,Degree1,OpSurface1);

        G4OpticalSurface* OpSurface2 = new G4OpticalSurface("side2");
        G4LogicalBorderSurface* Surface2 = new
                G4LogicalBorderSurface("side2",Scintillator,Degree2,OpSurface2);

        G4OpticalSurface* OpSurface3 = new G4OpticalSurface("side3");
        G4LogicalBorderSurface* Surface3 = new
                G4LogicalBorderSurface("side3",Scintillator,Degree3,OpSurface3);

        G4OpticalSurface* OpSurface4 = new G4OpticalSurface("side4");
        G4LogicalBorderSurface* Surface4 = new
                G4LogicalBorderSurface("side4",Scintillator,Degree4,OpSurface4);


        //https://escholarship.org/content/qt82r9k9rn/qt82r9k9rn.pdf?t=li5j59
        G4OpticalSurfaceFinish finishSide=groundfrontpainted;
        G4OpticalSurfaceModel modelSide=unified;
        G4SurfaceType typeSide=dielectric_dielectric;
                

        Logger::instance()->print(("Surface type (side): "+std::to_string(typeSide)).c_str());
        Logger::instance()->print(("Surface model (side): "+std::to_string(modelSide)).c_str());
        Logger::instance()->print(("Surface finish (side): "+std::to_string(finishSide)).c_str());

        OpSurface1->SetType(typeSide);
        OpSurface1->SetFinish(finishSide);
        OpSurface1->SetModel(modelSide);
        OpSurface2->SetFinish(finishSide);
        OpSurface2->SetModel(modelSide);
        OpSurface2->SetType(typeSide);
        OpSurface3->SetFinish(finishSide);
        OpSurface3->SetModel(modelSide);
        OpSurface3->SetType(typeSide);
        OpSurface4->SetFinish(finishSide);
        OpSurface4->SetModel(modelSide);
        OpSurface4->SetType(typeSide);


  


        G4double reflectivitySideOp[]={0.95,0.95};
        

   
        G4MaterialPropertiesTable* OpSurfaceProperty1 = new G4MaterialPropertiesTable();
        OpSurfaceProperty1->AddProperty("REFLECTIVITY",ph_Energy,reflectivitySideOp,2);
        G4MaterialPropertiesTable* OpSurfaceProperty2 = new G4MaterialPropertiesTable();
        OpSurfaceProperty2->AddProperty("REFLECTIVITY",ph_Energy,reflectivitySideOp,2);
        G4MaterialPropertiesTable* OpSurfaceProperty3 = new G4MaterialPropertiesTable();
        OpSurfaceProperty3->AddProperty("REFLECTIVITY",ph_Energy,reflectivitySideOp,2);
        G4MaterialPropertiesTable* OpSurfaceProperty4 = new G4MaterialPropertiesTable();
        OpSurfaceProperty4->AddProperty("REFLECTIVITY",ph_Energy,reflectivitySideOp,2);


        OpSurface1->SetMaterialPropertiesTable(OpSurfaceProperty1);
        OpSurface2->SetMaterialPropertiesTable(OpSurfaceProperty2);
        OpSurface3->SetMaterialPropertiesTable(OpSurfaceProperty3);
        OpSurface4->SetMaterialPropertiesTable(OpSurfaceProperty4);
        
        G4OpticalSurface* OpSurface5 = new G4OpticalSurface("detector1");
        G4LogicalBorderSurface* Surface5 = new
                G4LogicalBorderSurface("detector1",Meltmount1,Detector1,OpSurface5);
        G4OpticalSurface* OpSurface6 = new G4OpticalSurface("detector2");
        G4LogicalBorderSurface* Surface6 = new
                G4LogicalBorderSurface("detector2",Meltmount2,Detector2,OpSurface6);
  
        G4OpticalSurfaceFinish finishDet = polished;
        G4OpticalSurfaceModel modelDet = unified;
        G4SurfaceType typeDet = dielectric_metal;
 
        OpSurface5->SetType(typeDet);
        OpSurface5->SetFinish(finishDet);
        OpSurface5->SetModel(modelDet);
        OpSurface6->SetType(typeDet);
        OpSurface6->SetFinish(finishDet);
        OpSurface6->SetModel(modelDet);
        // IT APPLIES REFLECTIVITY PROBABILLITY FIRST
        // THEN EFFICIENCY
        G4double reflectivityDet[]={0.1,0.1};
        G4double efficiencyDet[]={0.5,0.5};

 

        G4MaterialPropertiesTable* OpSurfaceProperty6 = new G4MaterialPropertiesTable();
        OpSurfaceProperty6->AddProperty("EFFICIENCY",ph_Energy,efficiencyDet,2);
        OpSurfaceProperty6->AddProperty("REFLECTIVITY",ph_Energy,reflectivityDet,2);
        G4MaterialPropertiesTable* OpSurfaceProperty5 = new G4MaterialPropertiesTable();
        OpSurfaceProperty5->AddProperty("EFFICIENCY",ph_Energy,efficiencyDet,2);
        OpSurfaceProperty5->AddProperty("REFLECTIVITY",ph_Energy,reflectivityDet,2);



        OpSurface6->SetMaterialPropertiesTable(OpSurfaceProperty6);
        OpSurface5->SetMaterialPropertiesTable(OpSurfaceProperty5);
        
        G4double reflectivitySlices[]={0.5,0.5};
        G4OpticalSurfaceFinish finishSlices=polished;
        G4OpticalSurfaceModel modelSlices=unified;
        G4SurfaceType typeSlices=dielectric_dielectric;
 
        

        G4OpticalSurface* OpSurface9 = new G4OpticalSurface("melt_scint1");
        G4LogicalBorderSurface* Surface9 = new
                G4LogicalBorderSurface("melt_scint1",Meltmount1,Scintillator,OpSurface9);
        G4OpticalSurface* OpSurface10 = new G4OpticalSurface("melt_scint2");
        G4LogicalBorderSurface* Surface10 = new
                G4LogicalBorderSurface("melt_scint2",Meltmount2,Scintillator,OpSurface10);
  
 
 

        OpSurface9->SetType(typeSlices);
        OpSurface9->SetFinish(finishSlices);
        OpSurface9->SetModel(modelSlices);
        OpSurface10->SetType(typeSlices);
        OpSurface10->SetFinish(finishSlices);
        OpSurface10->SetModel(modelSlices);
      

        G4MaterialPropertiesTable* OpSurfaceProperty9 = new G4MaterialPropertiesTable();
        OpSurfaceProperty9->AddProperty("REFLECTIVITY",ph_Energy,reflectivitySlices,2);
        G4MaterialPropertiesTable* OpSurfaceProperty10 = new G4MaterialPropertiesTable();
        OpSurfaceProperty10->AddProperty("REFLECTIVITY",ph_Energy,reflectivitySlices,2);
        OpSurface9->SetMaterialPropertiesTable(OpSurfaceProperty9);
        OpSurface10->SetMaterialPropertiesTable(OpSurfaceProperty10);


 





        return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
