#ifndef DAPHNEUtils_h
#define DAPHNEUtils_h

#include "detdataformats/daphne/DAPHNEFrame.hpp"
#include "detdataformats/daphne/DAPHNEStreamFrame.hpp"
#include "detdataformats/daphne/DAPHNEFrame2.hpp"
#include "detdataformats/daphne/DAPHNEStreamFrame2.hpp"
#include "dunecore/DuneObj/DUNEHDF5FileInfo2.h"
#include "dunecore/HDF5Utils/HDF5RawFile2Service.h"

#include "duneprototypes/Protodune/hd/ChannelMap/DAPHNEChannelMapService.h"

#include "TTree.h"

namespace daphne {
  using dunedaq::daqdataformats::SourceID;
  using dunedaq::daqdataformats::FragmentType;
  using dunedaq::daqdataformats::Fragment;
  using dunedaq::daqdataformats::FragmentHeader;

  using DAPHNEStreamFrame = dunedaq::fddetdataformats::DAPHNEStreamFrame;
  using DAPHNEFrame = dunedaq::fddetdataformats::DAPHNEFrame;
  using DAPHNEStreamFrame2 = dunedaq::fddetdataformats::Daphnestreamframe2;
  using DAPHNEFrame2 = dunedaq::fddetdataformats::Daphneframe2;
namespace utils {


  static const size_t FragmentHeaderSize = sizeof(FragmentHeader);

  template<typename T>
  void ProcessFrames(
    std::unique_ptr<Fragment> & frag,
    std::unordered_map<unsigned int, WaveformVector> & wf_map);

  template<typename T>
  void ProcessStreamFrames(
    std::unique_ptr<Fragment> & frag,
    std::unordered_map<unsigned int, WaveformVector> & wf_map);

  template <typename T>
  void ProcessStreamFrame(
      T * frame,
      std::unordered_map<unsigned int, WaveformVector> & wf_map);

  template <typename T>
  void ProcessFrame(
      T * frame,
      std::unordered_map<unsigned int, WaveformVector> & wf_map);

  raw::OpDetWaveform & MakeWaveform(
    unsigned int offline_chan,
    size_t n_adcs,
    raw::TimeStamp_t timestamp,
    std::unordered_map<unsigned int, std::vector<raw::OpDetWaveform>> & wf_map,
    bool is_stream = false) {

    //If needed, make a new element in the map
    if (wf_map.find(offline_chan) == wf_map.end()) {
      wf_map.emplace(offline_chan, std::vector<raw::OpDetWaveform>());
    }

    //If is_stream, we just want to change the waveform
    //If not, or if this is the first time hitting this channel,
    //add a new waveform to the vector
    if (wf_map.at(offline_chan).size() == 0 || !is_stream) {
      wf_map.at(offline_chan).emplace_back(
          raw::OpDetWaveform(timestamp, offline_chan));
    }


    auto & waveform = wf_map.at(offline_chan).back();
    //Reserve more adcs at once for efficiency
    waveform.reserve(waveform.size() + n_adcs);
    return waveform;

  }

  template <class T> size_t GetNFrames(size_t frag_size, size_t frag_header_size) {
    return (frag_size - frag_header_size)/sizeof(T);
  }


  //Determine if we're streaming, and pick the corresponding frame type
  //and processing method
  void UnpackFragment(
      std::unique_ptr<Fragment> & frag,
      int frame_version,
      /*bool is_stream,*/
      std::unordered_map<unsigned int, WaveformVector> & wf_map) {
  
    bool is_stream = (frag->get_fragment_type() != FragmentType::kDAPHNE);

    if (frame_version == 1 && !is_stream) {
      ProcessFrames<DAPHNEFrame>(frag, wf_map);
    }
    else if (frame_version == 1 && is_stream) {
      ProcessStreamFrames<DAPHNEStreamFrame>(frag, wf_map);
    }
    else if (frame_version == 2 && !is_stream) {
      ProcessFrames<DAPHNEFrame2>(frag, wf_map);
    }
    else if (frame_version == 2 && is_stream) {
      ProcessStreamFrames<DAPHNEStreamFrame2>(frag, wf_map);
    }
    else {
      throw cet::exception("DAPHNEUtils") <<
          "Somehow didn't pass frame version and stream check?? " <<
          frame_version << " " << is_stream;
    }
  }


  //Get number of non-streaming Frames then loop over them and process each one
  template<typename T>
  void ProcessFrames(
    std::unique_ptr<Fragment> & frag,
    std::unordered_map<unsigned int, WaveformVector> & wf_map) {
  
    auto frame_size = sizeof(T);
    auto n_frames = GetNFrames<T>(frag->get_size(), FragmentHeaderSize);
    for (size_t i = 0; i < n_frames; ++i) {
      auto frame
          = reinterpret_cast<T*>(
              static_cast<uint8_t*>(frag->get_data()) + i*frame_size);
      ProcessFrame<T>(frame, wf_map);
    }
  }
  
  //Get number of streaming Frames then loop over them and process each one
  template<typename T>
  void ProcessStreamFrames(
    std::unique_ptr<Fragment> & frag,
    std::unordered_map<unsigned int, WaveformVector> & wf_map) {
  
    auto frame_size = sizeof(T);
    auto n_frames = GetNFrames<T>(frag->get_size(), FragmentHeaderSize);
    for (size_t i = 0; i < n_frames; ++i) {
      auto frame
          = reinterpret_cast<T*>(
              static_cast<uint8_t*>(frag->get_data()) + i*frame_size);
      ProcessStreamFrame<T>(frame, wf_map);
    }
  }

  template <typename T>
  void ProcessStreamFrame(
      T * frame,
      std::unordered_map<unsigned int, WaveformVector> & wf_map) {
    art::ServiceHandle<dune::DAPHNEChannelMapService> channel_map;
    auto b_link = frame->daq_header.link_id;
    auto b_slot = frame->daq_header.slot_id;
  
  
    //Each streaming frame comes with data from 4 channels
    std::array<size_t, 4> frame_channels = {
      frame->header.channel_0,
      frame->header.channel_1,
      frame->header.channel_2,
      frame->header.channel_3};
    // Loop over channels
    for (size_t i = 0; i < frame->s_channels_per_frame; ++i) {
      auto offline_channel = -1;
  
      try {
        offline_channel = channel_map->GetOfflineChannel(
          b_slot, b_link, frame_channels[i]);
      }
      catch (const std::range_error & err) {
        std::cout << "WARNING: Could not find offline channel for " <<
                     b_slot << " " << b_link << " " << frame_channels[i] << std::endl;
      }
  
      //_Slot = b_slot;
      //_DaphneChannel = frame_channels[i];
      //_OfflineChannel = offline_channel;
      //_FrameTimestamp = frame->get_timestamp();
      //_TriggerSampleValue = 0;
      //_Threshold = 0;
      //_Baseline = 0;
  
  
      //Make output
      auto & waveform = MakeWaveform(
            offline_channel,
            frame->s_adcs_per_channel,
            frame->get_timestamp(),
            wf_map,
            true);
  
      // Loop over ADC values in the frame for channel i 
      for (size_t j = 0; j < static_cast<size_t>(frame->s_adcs_per_channel); ++j) {
        waveform.push_back(frame->get_adc(j, i));
       // _adc_value[j] = frame->get_adc(j, i);
      }
  
      //if(fExportWaveformTree) fWaveformTree->Fill();
  
    }
  }

  template <typename T>
  void ProcessFrame(
      T * frame,
      std::unordered_map<unsigned int, WaveformVector> & wf_map) {
  
    art::ServiceHandle<dune::DAPHNEChannelMapService> channel_map;
    int b_channel_0 = frame->get_channel();
    int b_link = frame->daq_header.link_id;
    int b_slot = frame->daq_header.slot_id;
    auto offline_channel=-1;
    try {
       offline_channel = channel_map->GetOfflineChannel(
          b_slot, b_link, b_channel_0);
    }
    catch (const std::range_error & err) {
      //Just throw a warning so users can check out the rest of the data
      //maybe we can configure this to crash for keepup reco
      std::cout << "WARNING: Could not find offline channel for " <<
                   b_slot << " " << b_link << " " << b_channel_0 << std::endl;
    }
    //_Slot = b_slot;
    //_DaphneChannel = b_channel_0;
    //_OfflineChannel = offline_channel;
    //_FrameTimestamp = frame->get_timestamp();
    //_TriggerSampleValue = frame->header.trigger_sample_value;
    //_Threshold = frame->header.threshold;
    //_Baseline = frame->header.baseline;
  
    //Make output waveform and fill
    auto & waveform = MakeWaveform(
        offline_channel,
        static_cast<size_t>(frame->s_num_adcs),
        frame->get_timestamp(),
        wf_map);
    for (size_t j = 0; j < static_cast<size_t>(frame->s_num_adcs); ++j) {
      waveform.push_back(frame->get_adc(j));
     //_adc_value[j]=frame->get_adc(j);
    }
    //if(fExportWaveformTree) fWaveformTree->Fill();
  
  }

  bool CheckSubdet(size_t geo_id, std::string subdet_label) {
    dunedaq::detdataformats::DetID::Subdetector det_idenum
        = static_cast<dunedaq::detdataformats::DetID::Subdetector>(
            0xffff & geo_id);

    //Check that it's photon detectors
    auto subdetector_string
        = dunedaq::detdataformats::DetID::subdetector_to_string(det_idenum);
    return (subdetector_string == subdet_label);

  }

  bool CheckFragSize(std::unique_ptr<Fragment> & frag) {
    // Large enough to have header
    return (frag->get_size() > FragmentHeaderSize);
  }

  bool CheckIsDetReadout(const SourceID & source_id) {
    return (source_id.subsystem == SourceID::Subsystem::kDetectorReadout);
  }


  /*class DAPHNETree {
   public:
    int fRun, fEvent, fTriggerNumber, fNFrames, fSlot, fCrate, fDaphneChannel,
        fOfflineChannel, fTriggerSampleValue, fThreshold, fBaseline;
    long fTimestamp, fWindowEnd, fWindowBegin, fFrameTimestamp;
    short fADCValue[1024];

    void SetTree(TTree * tree) {
      fTree = tree;
    }

   private:
    TTree * fTree;
  };*/
}
}

#endif
