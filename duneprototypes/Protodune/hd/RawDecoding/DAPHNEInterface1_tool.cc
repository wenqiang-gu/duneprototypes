#include "DAPHNEInterfaceBase.h"
#include "dunecore/DuneObj/DUNEHDF5FileInfo2.h"
#include "dunecore/HDF5Utils/HDF5RawFile2Service.h"
#include "detdataformats/daphne/DAPHNEFrame.hpp"
#include "detdataformats/daphne/DAPHNEStreamFrame.hpp"
#include "DAPHNEUtils.h"

namespace daphne {

class DAPHNEInterface1 : public DAPHNEInterfaceBase {

 private:
  static const int fFrameVersion = 1;

 public:

  DAPHNEInterface1(fhicl::ParameterSet const& p) {};

  void Process(
      art::Event &evt,
      std::string inputlabel,
      std::string subdet_label,
      std::unordered_map<unsigned int, WaveformVector> & wf_map) override {

    //Get the HDF5 file to be opened
    auto infoHandle = evt.getHandle<raw::DUNEHDF5FileInfo2>(inputlabel);
    size_t   evtno = infoHandle->GetEvent();
    size_t   seqno = infoHandle->GetSequence();

    dunedaq::hdf5libs::HDF5RawDataFile::record_id_t record_id
        = std::make_pair(evtno, seqno);

    //Open the HDF5 file and get source ids
    art::ServiceHandle<dune::HDF5RawFile2Service> rawFileService;
    auto raw_file = rawFileService->GetPtr();
    auto source_ids = raw_file->get_source_ids(record_id);
    //Loop over source ids
    for (const auto & source_id : source_ids)  {
      // only want detector readout data (i.e. not trigger info)
      if (!utils::CheckIsDetReadout(source_id)) continue;

      //Loop over geo ids
      auto geo_ids = raw_file->get_geo_ids_for_source_id(record_id, source_id);
      for (const auto &geo_id : geo_ids) {
        //Check that it's photon detectors
        if (!utils::CheckSubdet(geo_id, subdet_label)) continue;

        //Get the fragment
        auto frag = raw_file->get_frag_ptr(record_id, geo_id);

        // Too small to even have a header
        if (!utils::CheckFragSize(frag)) continue;

        //Process it -- see DAPHNEUtils.h
        utils::UnpackFragment(frag, fFrameVersion, wf_map);
      }
    }
  };

};
}

DEFINE_ART_CLASS_TOOL(daphne::DAPHNEInterface1)
