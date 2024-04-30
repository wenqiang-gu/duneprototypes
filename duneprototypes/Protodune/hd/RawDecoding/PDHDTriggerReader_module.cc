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
#include "detdataformats/trigger/TriggerObjectOverlay.hpp"
#include "detdataformats/trigger/TriggerPrimitive.hpp"
#include "detdataformats/trigger/TriggerActivityData.hpp"
#include "detdataformats/trigger/TriggerCandidateData.hpp"

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
  produces<std::vector<dunedaq::trgdataformats::TriggerPrimitive>>(fOutputInstance);

  //TriggerActivity objects are TriggerActivityData with a list of the contained TPs.
  //Implement that as Assn between TriggerActivityData and TriggerPrimitive here
  produces<std::vector<dunedaq::trgdataformats::TriggerActivityData>>(fOutputInstance);
  //produces<art::Assns<dunedaq::trgdataformats::TriggerActivityData,dunedaq::trgdataformats::TriggerPrimitive>>(fOutputInstance);

  //TriggerCandidate objects are TriggerCandidateData with a list of the contained TAs.
  //Implement that as Assn between TriggerCandidateData and TriggerActivityData here
  produces<std::vector<dunedaq::trgdataformats::TriggerCandidateData>>(fOutputInstance);
  //produces<art::Assns<dunedaq::trgdataformats::TriggerCandidateData,dunedaq::trgdataformats::TriggerActivityData>>(fOutputInstance);

  consumes<raw::DUNEHDF5FileInfo2>(fInputLabel);  // the tool actually does the consuming of this product
}



void PDHDTriggerReader::produce(art::Event& e)
{

  std::vector<dunedaq::trgdataformats::TriggerPrimitive> tp_col;
  std::vector<dunedaq::trgdataformats::TriggerActivityData> ta_col;
  std::vector<dunedaq::trgdataformats::TriggerCandidateData> tc_col;

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
      // Perform a check to make sure we are only grabbing information from the trigger
      if (source_id.subsystem != dunedaq::daqdataformats::SourceID::Subsystem::kTrigger) continue;
      
      auto frag_ptr = rf->get_frag_ptr(rid, source_id);
      auto frag_size = frag_ptr->get_size();
      size_t fhs = sizeof(dunedaq::daqdataformats::FragmentHeader);
      
      if (frag_size <= fhs) continue; // Too small to even have a header
      
      size_t tps = sizeof(dunedaq::trgdataformats::TriggerPrimitive);
      size_t this_sid_no_of_tps = (frag_size - fhs) / tps;
      size_t current_no_of_tps = tp_col.size();
      void* frag_payload_ptr = frag_ptr->get_data();

      
      // Now you can take the block of data where frag_payload_ptr points to and copy this block of data into a vector of TriggerPrimitives,
      // trig_vector
      tp_col.resize(curent_no_of_tps+this_sid_no_of_tps);
      memcpy(&source_trig_map.at(source_id).at(current_no_of_tps), frag_payload_ptr, this_sid_no_of_tps * tps);

      for (size_t i = current_no_of_tps; i < tp_col.size(); ++i)
      {
	    if (fDebugLevel > 0)
	    {
	      std::cout << source_id << "   ;    " << i << "    ;    "  << tp_col.at(i).channel << "    ;    " << tp_col.at(i).time_start << "  ;  " << tp_col.at(i).version << std::endl;
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

      //loop over the data, one TA at a time
      long remaining_data_size = (long)get_data_size();
      void* data_ptr = frag_ptr->get_data();
      while(remaining_data_size>0) {
          //create our overlay object
          const dunedaq::trgdataformats::TriggerActivity& overlay =
                  *reinterpret_cast<const dunedaq::trgdataformats::TriggerActivity*>(data_ptr);

          //get its size
          auto this_size = sizeof(overlay::data_t) + //size of TriggerActivityData
                   sizeof(uint64_t) + //n_inputs is uint64_t
                   overlay.n_inputs*sizeof(overlay::input_t); //size of TP inputs

          //put our TA data on the output collection
          ta_col.emplace_back(overlay.data);

          //move the read position forward
          remaining_data_size = remaining_data_size-(long)this_size;
          data_ptr += this_size;

      } //end while(remaining_data_size>0)
    }

      
      
  for (auto const& source_id : tc_sourceids)
    {
      if (source_id.subsystem != dunedaq::daqdataformats::SourceID::Subsystem::kTrigger) continue;

      auto frag_ptr = rf->get_frag_ptr(rid, source_id);
      auto frag_size = frag_ptr->get_size();
      size_t fhs = sizeof(dunedaq::daqdataformats::FragmentHeader);

      if (frag_size <= fhs) continue;

      //loop over the data, one TA at a time
      long remaining_data_size = (long)get_data_size();
      void* data_ptr = frag_ptr->get_data();
      while(remaining_data_size>0) {
          //create our overlay object
          const dunedaq::trgdataformats::TriggerCandidate& overlay =
                  *reinterpret_cast<const dunedaq::trgdataformats::TriggerCandidate*>(data_ptr);

          //get its size
          auto this_size = sizeof(overlay::data_t) + //size of TriggerCandidateData
                  sizeof(uint64_t) + //n_inputs is uint64_t
                  overlay.n_inputs*sizeof(overlay::input_t); //size of TA inputs

          //put our TC data on the output collection
          tc_col.emplace_back(overlay.data);

          //move the read position forward
          remaining_data_size = remaining_data_size-(long)this_size;
          data_ptr += this_size;

      } //end while(remaining_data_size>0)
    }



  e.put(std::make_unique<std::vector<dunedaq::trgdataformats::TriggerPrimitive>>(std::move(tp_col)),fOutputInstance);
  e.put(std::make_unique<std::vector<dunedaq::trgdataformats::TriggerActivityData>>(std::move(ta_col)),fOutputInstance);
  e.put(std::make_unique<std::vector<dunedaq::trgdataformats::TriggerCandidateData>>(std::move(tc_col)),fOutputInstance);
  
}

DEFINE_ART_MODULE(PDHDTriggerReader)
