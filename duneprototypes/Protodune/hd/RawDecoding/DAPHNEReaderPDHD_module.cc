////////////////////////////////////////////////////////////////////////
// Class:       DAPHNEReaderPDHD
// Plugin Type: producer (Unknown Unknown)
// File:        DAPHNEReaderPDHD_module.cc
//
// Generated at Fri Aug 18 12:13:50 2023 by Jacob Calcutt using cetskelgen
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
#include "duneprototypes/Protodune/hd/ChannelMap/DAPHNEChannelMapService.h"
#include "art/Utilities/make_tool.h" 



#include "detdataformats/daphne/DAPHNEFrame.hpp"
#include "detdataformats/daphne/DAPHNEStreamFrame.hpp"
#include "detdataformats/daphne/DAPHNEFrame2.hpp"
#include "detdataformats/daphne/DAPHNEStreamFrame2.hpp"
#include "dunecore/DuneObj/DUNEHDF5FileInfo2.h"
#include "dunecore/HDF5Utils/HDF5RawFile2Service.h"

#include "DAPHNEInterfaceBase.h"

#include "lardataobj/RawData/OpDetWaveform.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "TTree.h"
#include "art_root_io/TFileService.h"

#include <memory>
namespace pdhd {

using dunedaq::daqdataformats::SourceID;
using dunedaq::daqdataformats::Fragment;
using dunedaq::daqdataformats::FragmentHeader;
using dunedaq::daqdataformats::FragmentType;
using DAPHNEStreamFrame = dunedaq::fddetdataformats::DAPHNEStreamFrame;
using DAPHNEFrame = dunedaq::fddetdataformats::DAPHNEFrame;
using DAPHNEStreamFrame2 = dunedaq::fddetdataformats::Daphnestreamframe2;
using DAPHNEFrame2 = dunedaq::fddetdataformats::Daphneframe2;

//For brevity 
using WaveformVector = std::vector<raw::OpDetWaveform>;

class DAPHNEReaderPDHD;

  // clang complained -- commenting out
  //const Int_t kMaxFrames = 50;

class DAPHNEReaderPDHD : public art::EDProducer {
public:
  explicit DAPHNEReaderPDHD(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  DAPHNEReaderPDHD(DAPHNEReaderPDHD const&) = delete;
  DAPHNEReaderPDHD(DAPHNEReaderPDHD&&) = delete;
  DAPHNEReaderPDHD& operator=(DAPHNEReaderPDHD const&) = delete;
  DAPHNEReaderPDHD& operator=(DAPHNEReaderPDHD&&) = delete;

  // Required functions.
  void produce(art::Event& e) override;
  void beginJob() override;

private:

  std::unique_ptr<daphne::DAPHNEInterfaceBase> fDAPHNETool;
  std::string fInputLabel, fOutputLabel, fFileInfoLabel, fSubDetString;
  TTree * fWaveformTree;

  bool fExportWaveformTree;
  //vars per event
  int _Run;
  // clang complained -- commenting out
  //int _SubRun;
  int _Event;
  int _TriggerNumber;
  long int _TimeStamp;
  long int _Window_end;
  long int _Window_begin;
  int _NFrames;
  int _Slot;
  int _Crate;
  int _DaphneChannel;
  int _OfflineChannel;
  long int _FrameTimestamp;
  short _adc_value[1024];
  int _TriggerSampleValue;
  int _Threshold;
  int _Baseline;
};
}

void pdhd::DAPHNEReaderPDHD::beginJob() {
  art::ServiceHandle<art::TFileService> tfs;

  if(fExportWaveformTree)
  {
    fWaveformTree = tfs->make<TTree>("WaveformTree","Waveforms Tree");
    fWaveformTree->Branch("Run", &_Run, "Run/I");
    fWaveformTree->Branch("Event", &_Event, "Event/I");
    fWaveformTree->Branch("TriggerNumber", &_TriggerNumber, "TriggerNumber/I");
    fWaveformTree->Branch("TimeStamp", &_TimeStamp, "TimeStamp/l");
    fWaveformTree->Branch("Window_begin", &_Window_begin, "Window_begin/l");
    fWaveformTree->Branch("Window_end", &_Window_end, "Window_end/l");

    fWaveformTree->Branch("Slot", &_Slot, "Slot/I");
    fWaveformTree->Branch("Crate", &_Crate, "Crate/I");
    fWaveformTree->Branch("DaphneChannel",& _DaphneChannel, "DaphneChannel/I");
    fWaveformTree->Branch("OfflineChannel", &_OfflineChannel, "OfflineChannel/I");
    fWaveformTree->Branch("FrameTimestamp",& _FrameTimestamp, "FrameTimestamp/l");
    fWaveformTree->Branch("adc_channel", _adc_value, "adc_value[1024]/S");

    fWaveformTree->Branch("TriggerSampleValue", &_TriggerSampleValue, "TriggerSampleValue/I"); //only for self-trigger
    fWaveformTree->Branch("Threshold", &_Threshold, "Threshold/I"); //only for self-trigger
    fWaveformTree->Branch("Baseline", &_Baseline, "Baseline/I"); //only for self-trigger

  }

}

pdhd::DAPHNEReaderPDHD::DAPHNEReaderPDHD(fhicl::ParameterSet const& p)
  : EDProducer{p}, 
    fDAPHNETool{
        art::make_tool<daphne::DAPHNEInterfaceBase>(
            p.get<fhicl::ParameterSet>("DAPHNEInterface"))},
    fInputLabel(p.get<std::string>("InputLabel", "daq")),
    fOutputLabel(p.get<std::string>("OutputLabel", "daq")),
    fFileInfoLabel(p.get<std::string>("FileInfoLabel", "daq")),
    fSubDetString(p.get<std::string>("SubDetString","HD_PDS")),
    fExportWaveformTree(p.get<bool>("ExportWaveformTree",true)) {
  produces<std::vector<raw::OpDetWaveform>> (fOutputLabel);
  produces<std::vector<recob::OpHit>> (fOutputLabel);
}

void pdhd::DAPHNEReaderPDHD::produce(art::Event& evt) {

//std::cout << "RUNNIN NEW EVENT ==================" << std::endl;

  std::vector<raw::OpDetWaveform> opdet_waveforms;
  std::unordered_map<unsigned int, WaveformVector> wf_map;
  std::vector<recob::OpHit> optical_hits;

  //Process the event
  fDAPHNETool->Process(evt, fFileInfoLabel, fSubDetString, wf_map);

  //Convert map to vector for output
  for (auto & chan_wf_vector : wf_map) {//Loop over channels
    //std::cout << "Inserting " << chan_wf_vector.first << " " << chan_wf_vector.second.size() << std::endl;
    opdet_waveforms.insert(opdet_waveforms.end(),
                           chan_wf_vector.second.begin(),
                           chan_wf_vector.second.end());
    //Remove elements from wf_map to save memory
    chan_wf_vector.second.clear();
  }
  //std::cout << "EventNumber " << _Event << std::endl;

  evt.put(
      std::make_unique<decltype(opdet_waveforms)>(std::move(opdet_waveforms)),
      fOutputLabel
  );

  evt.put(
      std::make_unique<decltype(optical_hits)>(std::move(optical_hits)),
      fOutputLabel
  );

}

DEFINE_ART_MODULE(pdhd::DAPHNEReaderPDHD)
