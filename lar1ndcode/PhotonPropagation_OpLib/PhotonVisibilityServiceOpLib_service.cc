////////////////////////////////////////////////////////////////////////
//
//  \file PhotonVisibilityServiceOpLib_service.cc
//
////////////////////////////////////////////////////////////////////////
//
//  Ben Jones, MIT 2012
//
//  This service reports the visibility of a particular point in
//  the detector to each OpDet.  This is used by the fast
//  optical simulation and by track-light association algorithms.
//
//  Visibility is defined as the fraction of isotropically produced
//  photons from a detector voxel which are expected to reach the 
//  OpDet in question.
//
//  This information is lookup up from a previousely generated
//  optical library file, whose path is specified to this service.
//
//  Note that it is important that the voxelization schemes match
//  between the library and the service instance for sensible results.
// 
//
// Framework includes

// LArSoft includes
#include "lar1ndcode/PhotonPropagation_OpLib/PhotonVisibilityServiceOpLib.h"
#include "lar1ndcode/PhotonPropagation_OpLib/PhotonLibrary.h"
#include "messagefacility/MessageLogger/MessageLogger.h" 
#include "Geometry/Geometry.h"
#include "Geometry/CryostatGeo.h"
#include "Geometry/OpDetGeo.h"
#include <string>
#include <iostream>
namespace phot{

  //--------------------------------------------------------------------
  PhotonVisibilityServiceOpLib::PhotonVisibilityServiceOpLib(fhicl::ParameterSet const& pset, art::ActivityRegistry &/*reg*/) :
    fCurrentVoxel(0),
    fCurrentValue(0.),
    fXmin(0.),
    fXmax(0.),
    fYmin(0.),
    fYmax(0.),
    fZmin(0.),
    fZmax(0.),
    fNx(0),
    fNy(0),
    fNz(0),
    fUseCryoBoundary(false),
    fLibraryBuildJob(false),
    fDoNotLoadLibrary(false),
    fParameterization(false),
    fStoreReflected(false),
    fTheLibrary(0)
  {
    this->reconfigure(pset);
    mf::LogInfo("PhotonVisibilityServiceOpLib")<<"PhotonVisbilityService initializing"<<std::endl;
        
    fNx        = pset.get< int          >("NX"       );
    fNy        = pset.get< int          >("NY"       );
    fNz        = pset.get< int          >("NZ"       );
    

  }


  //--------------------------------------------------------------------
  void PhotonVisibilityServiceOpLib::LoadLibrary() const
  {
    // Don't do anything if the library has already been loaded.

    if(fTheLibrary == 0) {
      fTheLibrary = new PhotonLibrary();

      art::ServiceHandle<geo::Geometry> geom;

      size_t NVoxels = GetVoxelDef().GetNVoxels();
      size_t NOpChannels = geom->NOpChannels();

    geo_file=std::string(geom->GDMLFile());


      if((!fLibraryBuildJob)&&(!fDoNotLoadLibrary)) {
	std::string LibraryFileWithPath;
	cet::search_path sp("FW_SEARCH_PATH");

	if( !sp.find_file(fLibraryFile, LibraryFileWithPath) )
	  throw cet::exception("PhotonVisibilityServiceOpLib") << "Unable to find photon library in "  << sp.to_string() << "\n";

	if(!fParameterization) {
	  mf::LogInfo("PhotonVisibilityServiceOpLib") << "PhotonVisibilityServiceOpLib Loading photon library from file "
						 << LibraryFileWithPath
						 << std::endl;
	  fTheLibrary->LoadLibraryFromFile(LibraryFileWithPath, NVoxels, NOpChannels,fStoreReflected);
	}
      }
      else {
	mf::LogInfo("PhotonVisibilityServiceOpLib") << " Vis service running library build job.  Please ensure " 
					       << " job contains LightSource, LArG4, SimPhotonCounter"<<std::endl;
					       	mf::LogInfo("PhotonVisibilityServiceOpLib")<<"extended library info "<<fExtendedLibraryInfo<<std::endl;

	 fTheLibrary->CreateEmptyLibrary(NVoxels, NOpChannels);
       	 mf::LogInfo("PhotonVisibilityServiceOpLib")<<"writing standard library -> extended library info val is "<<fExtendedLibraryInfo<<std::endl;
	 	
      }
    }
  }

  //--------------------------------------------------------------------
  void PhotonVisibilityServiceOpLib::StoreLibrary()
  {
    if(fTheLibrary == 0)
      LoadLibrary();

    if(fLibraryBuildJob )
      {
	mf::LogInfo("PhotonVisibilityServiceOpLib") << " Vis service "
					       << " Storing Library entries to file..." <<std::endl;
	
	if( fExtendedLibraryInfo==true) fTheLibrary->StoreLibraryToFile2(fLibraryFile,fNx,fNy,fNz,fNx*fNy*fNz,geo_file);
	else{
	fTheLibrary->StoreLibraryToFile(fLibraryFile,fStoreReflected);
       	 mf::LogInfo("PhotonVisibilityServiceOpLib")<<"writing standard library -> extended library info val is "<<fExtendedLibraryInfo<<std::endl;
	 	}
      }
  }
  

  //--------------------------------------------------------------------
  void PhotonVisibilityServiceOpLib::reconfigure(fhicl::ParameterSet const& p)
  {

    art::ServiceHandle<geo::Geometry> geom;
    	mf::LogInfo("PhotonVisibilityServiceOpLib") <<" reconfiguring PVS " <<std::endl;
	geo_file=geom->GDMLFile();
 mf::LogInfo("PhotonVisibilityServiceOpLib") << "gdml file path "<<geo_file<<std::endl;
    // Library details
    fLibraryBuildJob      = p.get< bool        >("LibraryBuildJob"     );
    fParameterization     = p.get< bool        >("LBNE10ktParameterization", false);
    fLibraryFile          = p.get< std::string >("LibraryFile"         );
    fDoNotLoadLibrary     = p.get< bool        >("DoNotLoadLibrary"    );
    fStoreReflected     = p.get< bool          >("StoreReflected"    );
    // Voxel parameters
    fUseCryoBoundary      = p.get< bool        >("UseCryoBoundary"     );
    fExtendedLibraryInfo     = p.get< bool        >("ExtendedLibraryInfo"    );
    
    if(fUseCryoBoundary)
      {
	double CryoBounds[6];
	geom->CryostatBoundaries(CryoBounds);
	fXmin = CryoBounds[0];
	fXmax = CryoBounds[1];
	fYmin = CryoBounds[2];
	fYmax = CryoBounds[3];
	fZmin = CryoBounds[4];
	fZmax = CryoBounds[5];
      }
    else
      {
	fXmin      = p.get< double >("XMin"     );
	fXmax      = p.get< double       >("XMax"     );
	fYmin      = p.get< double       >("YMin"     );
	fYmax      = p.get< double       >("YMax"     );
	fZmin      = p.get< double       >("ZMin"     );
	fZmax      = p.get< double       >("ZMax"     );
      }

    fNx        = p.get< int          >("NX"       );
    fNy        = p.get< int          >("NY"       );
    fNz        = p.get< int          >("NZ"       );
    
    fVoxelDef = sim::PhotonVoxelDef(fXmin, fXmax, fNx, fYmin, fYmax, fNy, fZmin, fZmax, fNz);

    return;
	
  }



  //------------------------------------------------------

  // Eventually we will calculate the light quenching factor here
  double PhotonVisibilityServiceOpLib::GetQuenchingFactor(double /* dQdx */)
  {
    // for now, no quenching
    return 1.0;

  }


  //------------------------------------------------------

  // Get a vector of the relative visibilities of each OpDet
  //  in the event to a point xyz

  const std::vector<float>* PhotonVisibilityServiceOpLib::GetAllVisibilities(double * xyz,bool wantReflected) const
  {
    int VoxID = fVoxelDef.GetVoxelID(xyz);
    return GetLibraryEntries(VoxID,wantReflected);
  }


  //------------------------------------------------------

  // Get distance to optical detector OpDet
  double PhotonVisibilityServiceOpLib::DistanceToOpDet( double* xyz, unsigned int OpChannel )
  {
    art::ServiceHandle<geo::Geometry> geom;
 
    // Find the right OpDet
    unsigned int c=0, o=0;
    geom->OpChannelToCryoOpDet(OpChannel, o, c);

    // Get its coordinates
    return geom->Cryostat(c).OpDet(o).DistanceToPoint(xyz);
      
  }


  //------------------------------------------------------


  // Get the solid angle reduction factor for planar optical detector OpDet
  double PhotonVisibilityServiceOpLib::SolidAngleFactor( double* xyz, unsigned int OpChannel )
  {
    art::ServiceHandle<geo::Geometry> geom;
   
    // Find the right OpDet
    unsigned int c=0, o=0;
    geom->OpChannelToCryoOpDet(OpChannel, o, c);
    
    double CosTheta = geom->Cryostat(c).OpDet(o).CosThetaFromNormal(xyz);

    return CosTheta;
  }

  //------------------------------------------------------

  float PhotonVisibilityServiceOpLib::GetVisibility(double * xyz, unsigned int OpChannel,bool wantReflected)
  {
    int VoxID = fVoxelDef.GetVoxelID(xyz);  
    return GetLibraryEntry(VoxID, OpChannel,wantReflected);
  }


  //------------------------------------------------------

  void PhotonVisibilityServiceOpLib::StoreLightProd(int VoxID, double N)
  {
    fCurrentVoxel = VoxID;
    fCurrentValue = N;
    mf::LogInfo("PhotonVisibilityServiceOpLib") << " PVS notes production of " << N << " photons at Vox " << VoxID<<std::endl; 
  }


  //------------------------------------------------------

  
  void PhotonVisibilityServiceOpLib::RetrieveLightProd(int& VoxID, double& N) const
  {
    N     = fCurrentValue;
    VoxID = fCurrentVoxel;
  }
  

  
  
  //------------------------------------------------------

  void PhotonVisibilityServiceOpLib::SetLibraryEntry(int VoxID, int OpChannel, float N, bool wantReflected)
  {

    if(fTheLibrary == 0)
      LoadLibrary();
   if(!wantReflected)
      fTheLibrary->SetCount(VoxID,OpChannel, N);
   else
      fTheLibrary->SetReflCount(VoxID,OpChannel, N); 
     
    mf::LogDebug("PhotonVisibilityServiceOpLib") << " PVS logging " << VoxID << " " << OpChannel<<std::endl;
  }

  //------------------------------------------------------

  

  const std::vector<float>* PhotonVisibilityServiceOpLib::GetLibraryEntries(int VoxID,bool wantReflected) const
  {
    if(fTheLibrary == 0)
      LoadLibrary();

    if(!wantReflected)
      return fTheLibrary->GetCounts(VoxID);
    else
      return fTheLibrary->GetReflCounts(VoxID);
  }

  //------------------------------------------------------

  float PhotonVisibilityServiceOpLib::GetLibraryEntry(int VoxID, int Channel, bool wantReflected ) const
  {
    if(fTheLibrary == 0)
      LoadLibrary();
  if(!wantReflected)
    return fTheLibrary->GetCount(VoxID, Channel);
  else
     return fTheLibrary->GetReflCount(VoxID, Channel); 
  }


} // namespace

namespace phot{
 
  DEFINE_ART_SERVICE(PhotonVisibilityServiceOpLib)

} // namespace phot