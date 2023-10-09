#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/make_tool.h" 
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "canvas/Persistency/Common/Assns.h"
#include "art/Persistency/Common/PtrMaker.h"

#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/RDTimeStamp.h"
#include "dunecore/DuneObj/RDStatus.h"
#include "dunecore/DuneObj/DUNEHDF5FileInfo2.h"
#include "dunecore/HDF5Utils/HDF5RawFile2Service.h"
#include "daqdataformats/v3_4_1/Fragment.hpp"
#include "detdataformats/trigger/TriggerPrimitive.hpp"
#include "detdataformats/trigger/TriggerActivityData.hpp"
#include "detdataformats/trigger/TriggerCandidateData.hpp"
#include "duneprototypes/Protodune/hd/RawDecoding/PDHDDataInterface.h"

#include <memory>
#include <iostream>

class PDHDTriggerReader;

class PDHDTriggerReader : public art::EDProducer {
public:
  explicit PDHDTriggerReader(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  PDHDTriggerReader(PDHDTriggerReader const&) = delete;
  PDHDTriggerReader(PDHDTriggerReader&&) = delete;
  PDHDTriggerReader& operator=(PDHDTriggerReader const&) = delete;
  PDHDTriggerReader& operator=(PDHDTriggerReader&&) = delete;

  // Required functions.
  void produce(art::Event& e) override;

private:

  std::string fInputLabel;
  std::string fOutputInstance;
  int fDebugLevel;
};


PDHDTriggerReader::PDHDTriggerReader(fhicl::ParameterSet const& p)
  : EDProducer{p},
  fInputLabel(p.get<std::string>("InputLabel","daq")),
  fOutputInstance(p.get<std::string>("OutputInstance","daq")),
  fDebugLevel(p.get<int>("DebugLevel",0))
{
  produces<std::map<dunedaq::daqdataformats::SourceID, std::vector<dunedaq::trgdataformats::TriggerPrimitive>>>(fOutputInstance);
  produces<std::map<dunedaq::daqdataformats::SourceID, std::vector<dunedaq::trgdataformats::TriggerActivityData>>>(fOutputInstance);
  produces<std::map<dunedaq::daqdataformats::SourceID, std::vector<dunedaq::trgdataformats::TriggerCandidateData>>>(fOutputInstance);
  consumes<raw::DUNEHDF5FileInfo2>(fInputLabel);  // the tool actually does the consuming of this product
}



void PDHDTriggerReader::produce(art::Event& e)
{

  // As a user of TP data, I can imagine wanting an std::map given to my art module that has the TP SourceIDs as the map keys, and vectors of TriggerPrimitives as the map values.
  std::map<dunedaq::daqdataformats::SourceID, std::vector<dunedaq::trgdataformats::TriggerPrimitive>> source_trig_map;
  std::map<dunedaq::daqdataformats::SourceID, std::vector<dunedaq::trgdataformats::TriggerActivityData>> source_trigact_map;  
  std::map<dunedaq::daqdataformats::SourceID, std::vector<dunedaq::trgdataformats::TriggerCandidateData>> source_trigcan_map;  

  auto infoHandle = e.getHandle<raw::DUNEHDF5FileInfo2>(fInputLabel);
  const std::string & file_name = infoHandle->GetFileName();
  uint32_t runno = infoHandle->GetRun();
  size_t   evtno = infoHandle->GetEvent();
  size_t   seqno = infoHandle->GetSequence();

  dunedaq::hdf5libs::HDF5RawDataFile::record_id_t rid = std::make_pair(evtno, seqno);
  
  if (fDebugLevel > 0)
    {
      std::cout << "PDHDDataInterface HDF5 FileName: " << file_name << std::endl;
      std::cout << "PDHDDataInterface Run:Event:Seq: " << runno << ":" << evtno << ":" << seqno << std::endl;
    }


  // Fetches SourceIDs for the set of Fragments that have TriggerPrimitive data in them.
  art::ServiceHandle<dune::HDF5RawFile2Service> rawFileService;
  auto rf = rawFileService->GetPtr();
 
  auto tp_sourceids = rf->get_source_ids_for_fragment_type(rid, dunedaq::daqdataformats::FragmentType::kTriggerPrimitive);
  auto ta_sourceids = rf->get_source_ids_for_fragment_type(rid, dunedaq::daqdataformats::FragmentType::kTriggerActivity);
  auto tc_sourceids = rf->get_source_ids_for_fragment_type(rid, dunedaq::daqdataformats::FragmentType::kTriggerCandidate);


  // Loop over SourceIDs, Calculates the number of TriggerPrimitive objects in the individual Fragment Payload 
  if (fDebugLevel > 0)
    {  
      std::cout << "runno:" << runno << " ; " << rf->get_source_ids(rid).size() << " ;  " << tp_sourceids.size() << " ; " << ta_sourceids.size() << " ; " << tc_sourceids.size() << std::endl;
    }

 
  for (auto const& source_id : tp_sourceids)
    {
      // Only want trigger info
      if (source_id.subsystem != dunedaq::daqdataformats::SourceID::Subsystem::kTrigger) continue;
      
      auto frag_ptr = rf->get_frag_ptr(rid, source_id);
      auto frag_size = frag_ptr->get_size();
      size_t fhs = sizeof(dunedaq::daqdataformats::FragmentHeader);
      
      if (frag_size <= fhs) continue; // Too small to even have a header
      
      size_t tps = sizeof(dunedaq::trgdataformats::TriggerPrimitive);
      size_t no_of_tps = (frag_size - fhs) / tps; 
      void* frag_payload_ptr = frag_ptr->get_data();

      
      // Now you can take the block of data where frag_payload_ptr points to and copy this block of data into a vector of TriggerPrimitives,
      // trig_vector
      source_trig_map[source_id].resize(no_of_tps);
      memcpy(&source_trig_map.at(source_id).at(0), frag_payload_ptr, no_of_tps * tps);

      const auto &trig_vector = source_trig_map.at(source_id);
      for (size_t i = 0; i < no_of_tps; ++i)
	{
	  if (fDebugLevel > 0)
	    {
	      std::cout << source_id << "   ;    " << i << "    ;    "  << trig_vector.at(i).channel << "    ;    " << trig_vector.at(i).time_start << "  ;  " << trig_vector.at(i).version << std::endl;
	    }
	} 
      
    
    } // for (auto const& source_id : tp_sourceids)
  
  

  for (auto const& source_id : ta_sourceids)
    {
      if (source_id.subsystem != dunedaq::daqdataformats::SourceID::Subsystem::kTrigger) continue;

      auto frag_ptr = rf->get_frag_ptr(rid, source_id);
      auto frag_size = frag_ptr->get_size();
      size_t fhs = sizeof(dunedaq::daqdataformats::FragmentHeader);

      if (frag_size <= fhs) continue;

      size_t tas = sizeof(dunedaq::trgdataformats::TriggerActivityData);
      size_t no_of_tas = (frag_size - fhs) / tas;
      void* frag_payload_ptr = frag_ptr->get_data();

      source_trigact_map[source_id].resize(no_of_tas);
      memcpy(&source_trigact_map.at(source_id).at(0), frag_payload_ptr, no_of_tas * tas);

    }

      
      
  for (auto const& source_id : tc_sourceids)
    {
      if (source_id.subsystem != dunedaq::daqdataformats::SourceID::Subsystem::kTrigger) continue;

      auto frag_ptr = rf->get_frag_ptr(rid, source_id);
      auto frag_size = frag_ptr->get_size();
      size_t fhs = sizeof(dunedaq::daqdataformats::FragmentHeader);

      if (frag_size <= fhs) continue;

      size_t tcs = sizeof(dunedaq::trgdataformats::TriggerCandidateData);
      size_t no_of_tcs = (frag_size - fhs) / tcs;
      void* frag_payload_ptr = frag_ptr->get_data();

      source_trigcan_map[source_id].resize(no_of_tcs);
      memcpy(&source_trigcan_map.at(source_id).at(0), frag_payload_ptr, no_of_tcs * tcs);

    }



  e.put(std::make_unique<decltype(source_trig_map)>(std::move(source_trig_map)),fOutputInstance);
  e.put(std::make_unique<decltype(source_trigact_map)>(std::move(source_trigact_map)),fOutputInstance);
  e.put(std::make_unique<decltype(source_trigcan_map)>(std::move(source_trigcan_map)),fOutputInstance);

  
}

DEFINE_ART_MODULE(PDHDTriggerReader)
