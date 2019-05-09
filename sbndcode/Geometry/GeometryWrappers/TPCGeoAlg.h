#ifndef TPCGEOALG_H_SEEN
#define TPCGEOALG_H_SEEN


///////////////////////////////////////////////
// TPCGeoAlg.h
//
// Wrapper for easy access to TPC geometry
// T Brooks (tbrooks@fnal.gov), May 2019
///////////////////////////////////////////////

// framework
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "fhiclcpp/ParameterSet.h" 
#include "art/Framework/Principal/Handle.h" 
#include "canvas/Persistency/Common/Ptr.h" 
#include "canvas/Persistency/Common/PtrVector.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h" 
#include "art_root_io/TFileService.h" 
#include "art_root_io/TFileDirectory.h" 
#include "messagefacility/MessageLogger/MessageLogger.h" 

// LArSoft
#include "larcore/Geometry/Geometry.h"
#include "larcorealg/Geometry/GeometryCore.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "lardataobj/RecoBase/Hit.h"

// c++
#include <vector>


namespace sbnd{

  class TPCGeoAlg {
  public:

    TPCGeoAlg();

    ~TPCGeoAlg();

    // Getters
    double MinX() const;
    double MinY() const;
    double MinZ() const;
    double MaxX() const;
    double MaxY() const;
    double MaxZ() const;
    double CpaWidth() const;

    // Functions for applying fiducial volume cuts to total volume
    bool InFiducial(geo::Point_t point, double fiducial);
    bool InFiducial(geo::Point_t point, double fiducial, double fiducialTop);
    bool InFiducial(geo::Point_t point, double minXCut, double minYCut, double minZCut, 
                    double maxXCut, double maxYCut, double maxZCut);
    
    // Determine which TPC a collection of hits is detected in (-1 if multiple)
    int DetectedInTPC(std::vector<art::Ptr<recob::Hit>> hits);

    // Determine if a true particle is ever inside the TPC volume
    bool InVolume(simb::MCParticle particle);
    // Determine if a true particle is contained inside the TPC volume
    bool IsContained(simb::MCParticle particle);
    // Determine if a true particle enters the TPC volume
    bool EntersVolume(simb::MCParticle particle);
    // Determine if a true particle crosses the TPC volume
    bool CrossesVolume(simb::MCParticle particle);

  private:

    double fMinX;
    double fMinY;
    double fMinZ;
    double fMaxX;
    double fMaxY;
    double fMaxZ;
    double fCpaWidth;

    geo::GeometryCore const* fGeometryService;

  };

}

#endif
