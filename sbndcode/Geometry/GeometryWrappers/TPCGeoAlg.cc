#include "TPCGeoAlg.h"

namespace sbnd{

// Constructor - get values from the geometry service
TPCGeoAlg::TPCGeoAlg(){

  fMinX = 99999;
  fMinY = 99999;
  fMinZ = 99999;
  fMaxX = -99999;
  fMaxY = -99999;
  fMaxZ = -99999;
  fCpaWidth = 0;

  fGeometryService = lar::providerFrom<geo::Geometry>();

  for(size_t cryo_i = 0; cryo_i < fGeometryService->Ncryostats(); cryo_i++){
    const geo::CryostatGeo& cryostat = fGeometryService->Cryostat(cryo_i);

    for (size_t tpc_i = 0; tpc_i < cryostat.NTPC(); tpc_i++)
    {
      const geo::TPCGeo& tpcg = cryostat.TPC(tpc_i);
      if (tpcg.MinX() < fMinX) fMinX = tpcg.MinX();
      if (tpcg.MaxX() > fMaxX) fMaxX = tpcg.MaxX();
      if (tpcg.MinY() < fMinY) fMinY = tpcg.MinY();
      if (tpcg.MaxY() > fMaxY) fMaxY = tpcg.MaxY();
      if (tpcg.MinZ() < fMinZ) fMinZ = tpcg.MinZ();
      if (tpcg.MaxZ() > fMaxZ) fMaxZ = tpcg.MaxZ();
      fCpaWidth = std::min(std::abs(tpcg.MinX()), std::abs(tpcg.MaxX()));
    }
  }
}


TPCGeoAlg::~TPCGeoAlg(){

}

// ----------------------------------------------------------------------------------
// Getters
double TPCGeoAlg::MinX() const{
  return fMinX;
}

double TPCGeoAlg::MinY() const{
  return fMinY;
}

double TPCGeoAlg::MinZ() const{
  return fMinZ;
}

double TPCGeoAlg::MaxX() const{
  return fMaxX;
}

double TPCGeoAlg::MaxY() const{
  return fMaxY;
}

double TPCGeoAlg::MaxZ() const{
  return fMaxZ;
}

double TPCGeoAlg::CpaWidth() const{
  return fCpaWidth;
}

// ----------------------------------------------------------------------------------
// Functions for applying fiducial volume cuts to total volume
bool TPCGeoAlg::InFiducial(geo::Point_t point, double fiducial){
  return InFiducial(point, fiducial, fiducial);
}

bool TPCGeoAlg::InFiducial(geo::Point_t point, double fiducial, double fiducialTop){
  return InFiducial(point, fiducial, fiducial, fiducial, fiducial, fiducialTop, fiducial);
}

bool TPCGeoAlg::InFiducial(geo::Point_t point, double minXCut, double minYCut, double minZCut, 
                           double maxXCut, double maxYCut, double maxZCut){
  
  double xmin = fMinX + minXCut;
  double xmax = fMaxX - maxXCut;
  double ymin = fMinY + minYCut;
  double ymax = fMaxY - maxYCut;
  double zmin = fMinZ + minZCut;
  double zmax = fMaxZ - maxZCut;

  double x = point.X();
  double y = point.Y();
  double z = point.Z();
  if(x>xmin && x<xmax && y>ymin && y<ymax && z>zmin && z<zmax) return true;

  return false;
}

// ----------------------------------------------------------------------------------
// Determine which TPC a collection of hits is detected in (-1 if multiple) 
int TPCGeoAlg::DetectedInTPC(std::vector<art::Ptr<recob::Hit>> hits){
  // Return tpc of hit collection or -1 if in multiple
  int tpc = hits[0]->WireID().TPC;
  for(size_t i = 0; i < hits.size(); i++){
    if((int)hits[i]->WireID().TPC != tpc) return -1;
  }
  return tpc;
}

// ----------------------------------------------------------------------------------
// Determine if a true particle is ever inside the TPC volume
bool TPCGeoAlg::InVolume(simb::MCParticle particle){
  for(size_t i = 0; i < particle.NumberTrajectoryPoints(); i++){
    double x = particle.Vx(i); 
    double y = particle.Vy(i);
    double z = particle.Vz(i);
    if(x > fMinX && y > fMinY && z > fMinZ && x < fMaxX && y < fMaxY && z < fMaxZ){
      return true;
    }
  }
  return false;
}

// ----------------------------------------------------------------------------------
// Determine if a true particle is contained inside the TPC volume
bool TPCGeoAlg::IsContained(simb::MCParticle particle){
  for(size_t i = 0; i < particle.NumberTrajectoryPoints(); i++){
    double x = particle.Vx(i); 
    double y = particle.Vy(i);
    double z = particle.Vz(i);
    if(x < fMinX || y < fMinY || z < fMinZ || x > fMaxX || y > fMaxY || z > fMaxZ){
      return false;
    }
  }
  return true;
}

// ----------------------------------------------------------------------------------
// Determine if a true particle enters the TPC volume
bool TPCGeoAlg::EntersVolume(simb::MCParticle particle){
  bool enters = false;
  bool startOutside = false;
  bool endOutside = false;
  for(size_t i = 0; i < particle.NumberTrajectoryPoints(); i++){
    double x = particle.Vx(i); 
    double y = particle.Vy(i);
    double z = particle.Vz(i);
    if(x > fMinX && y > fMinY && z > fMinZ && x < fMaxX && y < fMaxY && z < fMaxZ){
      enters = true;
    }
    else if(i == 0) startOutside = true;
    else if(i == particle.NumberTrajectoryPoints()-1) endOutside = true;
  }
  if(enters && (startOutside || endOutside)) return true;
  return false;
}

// ----------------------------------------------------------------------------------
// Determine if a true particle crosses the TPC volume
bool TPCGeoAlg::CrossesVolume(simb::MCParticle particle){
  bool enters = false;
  bool startOutside = false;
  bool endOutside = false;
  for(size_t i = 0; i < particle.NumberTrajectoryPoints(); i++){
    double x = particle.Vx(i); 
    double y = particle.Vy(i);
    double z = particle.Vz(i);
    if(x > fMinX && y > fMinY && z > fMinZ && x < fMaxX && y < fMaxY && z < fMaxZ){
      enters = true;
    }
    else if(i == 0) startOutside = true;
    else if(i == particle.NumberTrajectoryPoints()-1) endOutside = true;
  }
  if(startOutside && enters && endOutside) return true;
  return false;
}

}
