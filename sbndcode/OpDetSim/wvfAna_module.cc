////////////////////////////////////////////////////////////////////////
// Class:       wvfAna
// Module Type: analyzer
// File:        wvfAna_module.cc
//
// Analyzer to read optical waveforms
//
// Authors: L. Paulucci and F. Marinho
////////////////////////////////////////////////////////////////////////

#include <vector>
#include <cmath>
#include <memory>
#include <string>

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art_root_io/TFileService.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "canvas/Utilities/Exception.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "larcore/Geometry/Geometry.h"

#include "lardata/DetectorInfoServices/DetectorClocksService.h"
#include "lardata/DetectorInfoServices/DetectorClocksServiceStandard.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "lardata/DetectorInfoServices/LArPropertiesService.h"
#include "lardataobj/RawData/OpDetWaveform.h"
#include "sbndcode/Utilities/SignalShapingServiceSBND.h"
#include "lardataobj/Simulation/sim.h"
#include "lardataobj/Simulation/SimChannel.h"
#include "lardataobj/Simulation/SimPhotons.h"

#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"

namespace opdet{

  class wvfAna;

  class wvfAna : public art::EDAnalyzer {
  public:
    explicit wvfAna(fhicl::ParameterSet const & p);
    // The destructor generated by the compiler is fine for classes
    // without bare pointers or other resource use.

    // Plugins should not be copied or assigned.
    wvfAna(wvfAna const &) = delete;
    wvfAna(wvfAna &&) = delete;
    wvfAna & operator = (wvfAna const &) = delete;
    wvfAna & operator = (wvfAna &&) = delete;

    // Required functions.
    void analyze(art::Event const & e) override;

    //Selected optional functions
    void beginJob() override;
    void endJob() override;

  private:

    size_t fEvNumber;
    size_t fChNumber;
    double fSampling;
    double fStartTime;
    double fEndTime;
    //TTree *fWaveformTree;

    // Declare member data here.
    std::string fInputModuleName;
    std::stringstream histname;
  };


  wvfAna::wvfAna(fhicl::ParameterSet const & p)
    :
    EDAnalyzer(p)  // ,
    // More initializers here.
  {
    fInputModuleName = p.get< std::string >("InputModule" );

    auto const *timeService = lar::providerFrom< detinfo::DetectorClocksService >();
    fSampling = (timeService->OpticalClock().Frequency()); // MHz

  }

  void wvfAna::beginJob(){

  }

  void wvfAna::analyze(art::Event const & e)
  {
    // Implementation of required member function here.
    std::cout << "My module on event #" << e.id().event() << std::endl;

    art::ServiceHandle<art::TFileService> tfs;
    fEvNumber = e.id().event();

    art::Handle< std::vector< raw::OpDetWaveform > > waveHandle;
    e.getByLabel(fInputModuleName, waveHandle);

    if(!waveHandle.isValid()){
      std::cout << Form("Did not find any G4 photons from a producer: %s", "largeant") << std::endl;
    }

    std::cout << "Number of waveforms: " <<waveHandle->size() << std::endl;

    int hist_id = 0;
    for(auto const& wvf : (*waveHandle)){
      fChNumber = wvf.ChannelNumber();
      histname.str(std::string());
      histname << "event_" << fEvNumber
               << "_opchannel_" << fChNumber
               << "_" << hist_id;

      fStartTime = wvf.TimeStamp(); //in us
      fEndTime = double(wvf.size())/fSampling + fStartTime; //in us

      //Create a new histogram
      TH1D *wvfHist = tfs->make< TH1D >(histname.str().c_str(), TString::Format(";t - %f (#mus);",fStartTime), wvf.size(), fStartTime, fEndTime);
      for(unsigned int i=0; i<wvf.size();i++){
        wvfHist->SetBinContent(i+1,(double)wvf[i]);
      }
      hist_id++;
    }
  }

  void wvfAna::endJob(){
  }

  DEFINE_ART_MODULE(opdet::wvfAna)

}
