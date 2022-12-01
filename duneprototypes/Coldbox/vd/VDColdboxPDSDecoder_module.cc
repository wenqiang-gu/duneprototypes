////////////////////////////////////////////////////////////////////////
// Class:       VDColdboxPDSDecoder
// Plugin Type: producer (Unknown Unknown)
// File:        VDColdboxPDSDecoder_module.cc
//
// Generated at Tue Nov  9 16:28:27 2021 by Jacob Calcutt using cetskelgen
// from  version .
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "lardataobj/RawData/OpDetWaveform.h"
#include "lardataobj/RecoBase/OpHit.h"
#include <hdf5.h>
#include "dunecore/DuneObj/DUNEHDF5FileInfo.h"
#include "dunecore/HDF5Utils/HDF5Utils.h"
#include "daqdataformats/v3_3_3/Fragment.hpp"
#include "detdataformats/ssp/SSPTypes.hpp"

#include <memory>

namespace dune {
  class VDColdboxPDSDecoder;
}


class dune::VDColdboxPDSDecoder : public art::EDProducer {
public:
  explicit VDColdboxPDSDecoder(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  VDColdboxPDSDecoder(VDColdboxPDSDecoder const&) = delete;
  VDColdboxPDSDecoder(VDColdboxPDSDecoder&&) = delete;
  VDColdboxPDSDecoder& operator=(VDColdboxPDSDecoder const&) = delete;
  VDColdboxPDSDecoder& operator=(VDColdboxPDSDecoder&&) = delete;

  // Required functions.
  void produce(art::Event& e) override;

private:

  std::pair<raw::OpDetWaveform, recob::OpHit> MakeWaveformAndHit(
      const dunedaq::detdataformats::ssp::EventHeader * event_header,
      size_t data_pos, uint8_t * data_ptr);

  hid_t fPrevStoredHandle = -1;
  hid_t fHDFFile = -1;
  std::string fOutputDataLabel;
  std::string fFileInfoLabel;
  bool fForceOpen;
  bool fDebug;

  // Declare member data here.

};


dune::VDColdboxPDSDecoder::VDColdboxPDSDecoder(fhicl::ParameterSet const& p)
  : EDProducer{p},
    fOutputDataLabel{p.get<std::string>("OutputDataLabel")},
    fFileInfoLabel{p.get<std::string>("FileInfoLabel", "daq")},
    fForceOpen(p.get<bool>("ForceOpen", false)),
    fDebug(p.get<bool>("Debug", false)) {
  produces<std::vector<raw::OpDetWaveform>>(fOutputDataLabel);
  produces<std::vector<recob::OpHit>>(fOutputDataLabel);
}

void dune::VDColdboxPDSDecoder::produce(art::Event& e) {

  using namespace dune::HDF5Utils;
  using namespace dunedaq::detdataformats::ssp;

  //To-do: put in sizes here?
  std::unique_ptr<std::vector<raw::OpDetWaveform>> output_wfs
      = std::make_unique<std::vector<raw::OpDetWaveform>>();
  std::unique_ptr<std::vector<recob::OpHit>> output_hits
      = std::make_unique<std::vector<recob::OpHit>>();


  auto infoHandle = e.getHandle<raw::DUNEHDF5FileInfo>(fFileInfoLabel);
  const std::string & group_name = infoHandle->GetEventGroupName();
  const std::string & file_name = infoHandle->GetFileName();
  hid_t file_id = infoHandle->GetHDF5FileHandle();
  
  //If the fcl file said to force open the file
  //(i.e. because one is just running DataPrep), then open
  //but only if we are on a new file -- identified by if the handle
  //stored in the event is different
  if (fForceOpen && (file_id != fPrevStoredHandle)) {
    std::cout << "Opening" << std::endl;
    fHDFFile = H5Fopen(file_name.data(), H5F_ACC_RDONLY, H5P_DEFAULT);
  }//If the handle is the same, fHDFFile won't change
  else if (!fForceOpen) {
    fHDFFile = file_id;
  }
  fPrevStoredHandle = file_id;

  hid_t PDS_group = getGroupFromPath(fHDFFile, group_name + "/PDS");
  std::deque<std::string> region_names = getMidLevelGroupNames(PDS_group);
  if (fDebug)
    std::cout << "Got " << region_names.size() << " regions" << std::endl;
  for (const auto & n : region_names) {
    hid_t region_group = getGroupFromPath(PDS_group, n);
    std::deque<std::string> element_names = getMidLevelGroupNames(region_group);
    if (fDebug)
      std::cout << "Got " << element_names.size() << " elements" << std::endl;
    for (const auto & element_name : element_names) {
      if (fDebug) std::cout << element_name << std::endl;

      hid_t dataset = H5Dopen(region_group, element_name.data(), H5P_DEFAULT);
      hsize_t ds_size = H5Dget_storage_size(dataset);
      if (fDebug) std::cout << "\tDataset size: " << ds_size << std::endl;

      std::vector<char> ds_data(ds_size);
      H5Dread(dataset, H5T_STD_I8LE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
              ds_data.data());
      H5Dclose(dataset);

      Fragment frag(&ds_data[0], Fragment::BufferAdoptionMode::kReadOnlyMode);
      if (fDebug) {
        std::cout << "\tMade fragment" << std::endl;
        std::cout << "\t" << frag.get_header() << std::endl;
      }

      
      uint8_t * data_ptr = reinterpret_cast<uint8_t*>(frag.get_data());

      size_t iP = 0;
      size_t data_pos = 0;
      while (data_pos < (frag.get_header().size - sizeof(FragmentHeader))) {
        EventHeader * event_header
            = reinterpret_cast<EventHeader*>(data_ptr + data_pos);
        if (fDebug) {
          std::cout << "\tEvent header " << event_header->length << std::endl;
          std::cout << "\t" << iP << std::endl;
        }

        auto wf_hit = MakeWaveformAndHit(event_header, data_pos, data_ptr);
        output_wfs->emplace_back(wf_hit.first);
        output_hits->emplace_back(wf_hit.second);

        //Iterate position in the data
        data_pos += event_header->length;
        ++iP;
      }
      if (fDebug) 
        std::cout << "Iterated through " << iP << " packets " << std::endl;
    }
    H5Gclose(region_group);
  }
  H5Gclose(PDS_group);

  e.put(std::move(output_wfs), fOutputDataLabel);
  e.put(std::move(output_hits), fOutputDataLabel);
}

std::pair<raw::OpDetWaveform, recob::OpHit>
    dune::VDColdboxPDSDecoder::MakeWaveformAndHit(
        const dunedaq::detdataformats::ssp::EventHeader * event_header,
        size_t data_pos, uint8_t * data_ptr) {

  using namespace dunedaq::detdataformats::ssp;
  size_t nADC = (event_header->length - sizeof(EventHeader))/2;
  if (fDebug) std::cout << "\tnADC: " << nADC << std::endl;
  unsigned long ts = 0;
  for (unsigned int iword = 0; iword <= 3; ++iword) {
    ts += ((unsigned long)(event_header->timestamp[iword])) << 16 * iword;
  }

  //Need time, channel
  raw::OpDetWaveform wf(ts, event_header->group2, nADC);

  //Iterate to the start of the adc data
  unsigned short * adc_ptr = reinterpret_cast<unsigned short *>(
      data_ptr + data_pos + sizeof(EventHeader));

  //size_t peak_tick = 0;
  //unsigned short max_adc = 0;

  for (size_t i = 0; i < nADC; ++i) {
    wf.push_back(*(adc_ptr + i));
    //max_adc = std::max(max_adc, *(adc_ptr + i));
    //if(max_adc == *(adc_ptr + i)) peak_tick = i;
  }
  //std::cout << "Max adc, peak tick: " << max_adc << ", " << peak_tick <<
  //             std::endl;

  return {wf, recob::OpHit()};
}

DEFINE_ART_MODULE(dune::VDColdboxPDSDecoder)
