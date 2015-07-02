////////////////////////////////////////////////////////////////////////
// Class:       SamuelAnalyzer
// Module Type: analyzer
// File:        SamuelAnalyzer_module.cc
//
// Generated at Thu Jun 18 11:41:33 2015 by Samuel Santana using artmod
// from cetpkgsupport v1_08_06.
////////////////////////////////////////////////////////////////////////


/*#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Simulation/SimChannel.h"

//#include "Simulation/SimChannel.h"

#include "AnalysisBase/ParticleID.h"

// Larsoft includes

#include "SimulationBase/MCParticle.h"
#include "SimulationBase/MCTruth.h"
//#include "Simulation/LArG4Parameters.h" 

// C++ includes

#include <iostream> */

// LArSoft includes
#include "Simulation/SimChannel.h"
#include "Simulation/LArG4Parameters.h"
#include "RecoBase/Hit.h"
#include "RecoBase/Cluster.h"
#include "Geometry/Geometry.h"
#include "SimulationBase/MCParticle.h"
#include "SimulationBase/MCTruth.h"
#include "SimpleTypesAndConstants/geo_types.h"

// Framework includes
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Core/FindManyP.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "fhiclcpp/ParameterSet.h"
#include "cetlib/exception.h"
#include "MCCheater/BackTracker.h"
#include "Utilities/DetectorProperties.h"
#include "Utilities/LArProperties.h"
#include "Simulation/SimChannel.h"
#include "Simulation/sim.h"
#include "SimulationBase/MCTruth.h"
#include "Geometry/Geometry.h"
#include "AnalysisBase/ParticleID.h"
#include "SimpleTypesAndConstants/geo_types.h"
#include "SummaryData/POTSummary.h"

// ROOT includes
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TVector3.h"

// C++ Includes
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>

// Sowjanya's includes
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/View.h"
#include "art/Persistency/Common/Ptr.h"
#include "art/Persistency/Common/PtrVector.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "art/Framework/Core/FindMany.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geometry/Geometry.h"
#include "SimulationBase/MCTruth.h"
#include "SimulationBase/MCFlux.h"
#include "Simulation/SimChannel.h"
#include "Simulation/AuxDetSimChannel.h"
#include "AnalysisBase/Calorimetry.h"
#include "AnalysisBase/ParticleID.h"
#include "RawData/RawDigit.h"
#include "RawData/BeamInfo.h"
#include "Utilities/LArProperties.h"
#include "Utilities/AssociationUtil.h"
#include "Utilities/DetectorProperties.h"
#include "SummaryData/POTSummary.h"
#include "MCCheater/BackTracker.h"
#include "RecoBase/Track.h"
#include "RecoBase/Cluster.h"
#include "RecoBase/Hit.h"
#include "RecoBase/EndPoint2D.h"
#include "RecoBase/Vertex.h"
#include "SimpleTypesAndConstants/geo_types.h"
#include "RecoObjects/BezierTrack.h"

// nusoft includes
#include "SimulationBase/MCFlux.h"
#include "SimulationBase/MCNeutrino.h"
#include "SimulationBase/MCTruth.h"
#include "SimulationBase/GTruth.h"
#include "SimulationBase/MCParticle.h"
#include "SummaryData/POTSummary.h"


namespace lar1nd {
  class SamuelAnalyzer;
}

class lar1nd::SamuelAnalyzer : public art::EDAnalyzer {
public:

  explicit SamuelAnalyzer(fhicl::ParameterSet const & p);
  // The destructor generated by the compiler is fine for classes
  // without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  SamuelAnalyzer(SamuelAnalyzer const &) = delete;
  SamuelAnalyzer(SamuelAnalyzer &&) = delete;
  SamuelAnalyzer & operator = (SamuelAnalyzer const &) = delete;
  SamuelAnalyzer & operator = (SamuelAnalyzer &&) = delete;

  // Required functions.
  //void analyze(art::Event const & e) override;
  void analyze(art::Event const & e);

  // Selected optional functions.
 //oid beginJob() override;
  void beginJob();
 //oid reconfigure(fhicl::ParameterSet const & p) override;
void reconfigure(fhicl::ParameterSet const & p);

private:

  // Declare member data here.
  
  // The parameters from the .fcl file.
    std::string fSimulationProducerLabel; // The name of the producer that tracked simulated particles through the detector
    std::string fGenieModuleLabel;
  //std::string fLarg4ModuleLabel;
};


lar1nd::SamuelAnalyzer::SamuelAnalyzer(fhicl::ParameterSet const & p)
  :
  EDAnalyzer(p)  // ,
 // More initializers here.
{
 // Read in the parameters from the .fcl file.
   this->reconfigure(p);
}

void lar1nd::SamuelAnalyzer::analyze(art::Event const & e)
{
  // Implementation of required member function here.
   std::cout << std::endl;
   std::cout << "xxxxxxxxxxxxxxxxx Analyzer xxxxxxxxxxxxxxxxx"<< std::endl;
   int runNumber = e.run();
   int evtNumber = e.event();
   int SubRunNumber = e.subRun();
   std::cout<<"run:   "<<runNumber<<"   evt:     "<< evtNumber <<"   SubRunNumber:      "<< SubRunNumber << std::endl;
   std::cout << std::endl;
   
   art::Handle< std::vector<simb::MCTruth> > mclistGENIE;
   e.getByLabel(fGenieModuleLabel,mclistGENIE);

   art::Handle<std::vector<simb::MCParticle> > mcpHandle;
   e.getByLabel(fSimulationProducerLabel,mcpHandle);
  
        
    // Check is data product is found or not
    if(!mcpHandle.isValid())
    {
      std::cout << "Did not find MCParticle from label..." << std::endl << std::endl;
      return;
    }
      std::cout<<"Si paso, fSimulationProducerLabel:    "<<fSimulationProducerLabel<<std::endl;
      std::cout<<"size:   "<<mcpHandle->size()<<std::endl;

      
      std::vector<const simb::MCParticle* > particle;
     //sim::ParticleList plist;

     //std::cout<<"size:   "<<particle->size()<<std::endl;//this gives an error

     for(auto i = particle.begin(), iend = particle.end(); i != iend; ++i)
    {
      int pdg = (*i)->PdgCode();
      int trackId = (*i)->TrackId();
      std::cout<<"===================== entro al loop!! =============================="<<std::endl;
      std::cout<<pdg<<trackId<<std::endl;
    }

 // contains the gtruth object
   art::Ptr<simb::MCTruth> mc(mclistGENIE,0);

  // simb::MCNeutrino neutrino = mc -> GetNeutrino();

   art::Ptr<simb::MCParticle > pHandle(mcpHandle,0);
   
   //int isCC = -99;

   //std::cout<<"pHandle:   "<<pHandle->isCC << std::endl;

////Andrzej copying things
   
    // loop over all sim::SimChannels in the event and make sure there are no
    // sim::IDEs with trackID values that are not in the sim::ParticleList
    std::vector<const sim::SimChannel*> sccol;
    evt.getView(fG4ModuleLabel, sccol);

    double totalCharge=0.0;
    double totalEnergy=0.0;
    fnumChannels->Fill(sccol.size());
    for(size_t sc = 0; sc < sccol.size(); ++sc){
      double numIDEs=0.0;
      double scCharge=0.0;
      double scEnergy=0.0;
      const std::map<unsigned short, std::vector<sim::IDE> >& tdcidemap = sccol[sc]->TDCIDEMap();
      for(auto mapitr = tdcidemap.begin(); mapitr != tdcidemap.end(); mapitr++){
	const std::vector<sim::IDE> idevec = (*mapitr).second;
	numIDEs += idevec.size();
	for(size_t iv = 0; iv < idevec.size(); ++iv){
	  if(plist.find( idevec[iv].trackID ) == plist.end()
	     && idevec[iv].trackID != sim::NoParticleId) 
	  mf::LogWarning("LArG4Ana") << idevec[iv].trackID << " is not in particle list"; 
	  totalCharge +=idevec[iv].numElectrons;
	  scCharge += idevec[iv].numElectrons;
	  totalEnergy +=idevec[iv].energy;
	  scEnergy += idevec[iv].energy;
	}
      }
      fnumIDEs->Fill(sc,numIDEs);
      fChannelCharge->Fill(sc,scCharge);
      fChannelEnergy->Fill(sc,scEnergy);
    }
   
   
////end Andrzej copying things   
   
   
   
    art::Handle< std::vector<simb::GTruth> > mcgtruth;
    e.getByLabel(fGenieModuleLabel,mcgtruth);

 // Check is data product is found or not
    if(!mcgtruth.isValid())
    {
      std::cout << "Did not find mcgtruth from label..." << std::endl << std::endl;
      return;
    }
      std::cout<<"Si paso, fGenieModuleLabel:    "<<fGenieModuleLabel<<std::endl;
      std::cout<<"size:   "<<mcgtruth->size()<<std::endl;    

 // contains the gtruth object
    art::Ptr<simb::GTruth > gtruth(mcgtruth,0);

    std::cout << "The interaction info is: \n" 
              << "  gtruth->ftgtPDG................." << gtruth->ftgtPDG << "\n"
              << "  gtruth->ftgtZ..................." << gtruth->ftgtZ << "\n"
              << "  gtruth->ftgtA..................." << gtruth->ftgtA << "\n"
              << "  gtruth->fGint..................." << gtruth->fGint << "\n"
              << "  gtruth->fGscatter..............." << gtruth->fGscatter << "\n"
              << "  gtruth->fweight................." << gtruth->fweight << "\n"
    //          << "  neutrino.Mode()................." << neutrino.Mode() << "\n"
    //           << "  neutrino.InteractionType()......" << neutrino.InteractionType() << "\n"
    //           << "  neutrino.CCNC()................." << neutrino.CCNC() << "\n"
    //           << "  neutrino.Target()..............." << neutrino.Target() << "\n"
              << std::endl;


}

void lar1nd::SamuelAnalyzer::beginJob()
{
  // Implementation of optional member function here.
   std::cout << std::endl;
   std::cout << "xxxxxxxxxxxxxxx Empezando xxxxxxxxxxxxxxx"<< std::endl;
   std::cout << std::endl;
}

void lar1nd::SamuelAnalyzer::reconfigure(fhicl::ParameterSet const & p)
{
  // Implementation of optional member function here.

   std::cout << std::endl;
   std::cout << "xxxxxxxxxxxxxxxxx Reco xxxxxxxxxxxxxxxxx"<< std::endl;
   std::cout << std::endl;

   fSimulationProducerLabel = p.get< std::string >("SimulationLabel");
   fGenieModuleLabel        = p.get< std::string >("GenieGenModuleLabel");
// fLarg4ModuleLabel        = p.get< std::string >("LArG4ModuleLabel");
   
}

DEFINE_ART_MODULE(lar1nd::SamuelAnalyzer)
