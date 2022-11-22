////////////////////////////////////////////////////////////////////////
// Class:       PDHDTPCReader
// Plugin Type: producer (Unknown Unknown)
// File:        PDHDTPCReader_module.cc
//
//   Module to exercise the PDHDDataInterfaceWIB3 tool.
//    Read raw::RawDigits into the event for a hardcoded list of APAs
//    
// Generated at Thu Nov 17 17:05:55 2022 by Thomas Junk using cetskelgen
// from  version .
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/make_tool.h" 
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "canvas/Persistency/Common/Assns.h"
#include "art/Persistency/Common/PtrMaker.h"

#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/RDTimeStamp.h"
#include "dunecore/DuneObj/RDStatus.h"
#include "duneprototypes/Protodune/hd/RawDecoding/PDHDDataInterface.h"
#include "dunecore/DuneObj/DUNEHDF5FileInfo2.h"

#include <memory>

class PDHDTPCReader;


class PDHDTPCReader : public art::EDProducer {
public:
  explicit PDHDTPCReader(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  PDHDTPCReader(PDHDTPCReader const&) = delete;
  PDHDTPCReader(PDHDTPCReader&&) = delete;
  PDHDTPCReader& operator=(PDHDTPCReader const&) = delete;
  PDHDTPCReader& operator=(PDHDTPCReader&&) = delete;

  // Required functions.
  void produce(art::Event& e) override;

private:

  std::string m_InputLabel;
  std::string m_OutputInstance;
  std::vector<int> m_APAList;
  std::unique_ptr<PDHDDataInterface> m_DecoderTool;

};


PDHDTPCReader::PDHDTPCReader(fhicl::ParameterSet const& p)
  : EDProducer{p},
  m_InputLabel(p.get<std::string>("InputLabel","daq")),
  m_OutputInstance(p.get<std::string>("OutputInstance","daq")),
  m_APAList(p.get<std::vector<int>>("APAList")),
  m_DecoderTool{art::make_tool<PDHDDataInterface>(p.get<fhicl::ParameterSet>("DecoderToolParams"))}
{
  produces<std::vector<raw::RawDigit>>(m_OutputInstance);
  produces<std::vector<raw::RDStatus>>(m_OutputInstance);
  produces<std::vector<raw::RDTimeStamp>>(m_OutputInstance);
  produces<art::Assns<raw::RawDigit,raw::RDTimeStamp>>(m_OutputInstance);
  consumes<raw::DUNEHDF5FileInfo2>(m_InputLabel);  // the tool actually does the consuming of this product
}

void PDHDTPCReader::produce(art::Event& e)
{
  std::vector<raw::RawDigit> rawdigitcol;
  std::vector<raw::RDStatus> rdstatuscol;
  std::vector<raw::RDTimeStamp> rdtscol;
  art::Assns<raw::RawDigit,raw::RDTimeStamp> rdtacol;
  std::vector<int> apalist;

  m_DecoderTool->retrieveDataForSpecifiedAPAs(e, rawdigitcol, rdtscol, rdstatuscol, m_APAList);

  // make associations between raw digits and RDTimestamps

  art::PtrMaker<raw::RawDigit> rdpm(e,m_OutputInstance);
  art::PtrMaker<raw::RDTimeStamp> tspm(e,m_OutputInstance);

  for (size_t i=0; i < rawdigitcol.size(); ++i)
    {
      auto const rawdigitptr = rdpm(i);
      auto const rdtimestampptr = tspm(i);
      rdtacol.addSingle(rawdigitptr,rdtimestampptr);
    }

  e.put(std::make_unique<decltype(rawdigitcol)>(std::move(rawdigitcol)),m_OutputInstance);
  e.put(std::make_unique<decltype(rdtscol)>(std::move(rdtscol)),m_OutputInstance);
  e.put(std::make_unique<decltype(rdtacol)>(std::move(rdtacol)),m_OutputInstance);
  e.put(std::make_unique<decltype(rdstatuscol)>(std::move(rdstatuscol)),m_OutputInstance);

}

DEFINE_ART_MODULE(PDHDTPCReader)
