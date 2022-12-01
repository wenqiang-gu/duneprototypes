#ifndef PDHDDataInterface_H
#define PDHDDataInterface_H

#include "art/Utilities/ToolMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Persistency/Common/PtrMaker.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/RDTimeStamp.h"
#include "dunecore/DuneObj/PDSPTPCDataInterfaceParent.h"
#include "daqdataformats/v3_4_1/Fragment.hpp"
#include "dunecore/HDF5Utils/dunedaqhdf5utils2/HDF5RawDataFile.hpp"

// typedef dunedaq::daqdataformats::Fragment duneFragment;
// typedef std::vector<duneFragment> duneFragments; 

typedef std::vector<raw::RawDigit> RawDigits;
typedef std::vector<raw::RDTimeStamp> RDTimeStamps;

class PDHDDataInterface : public PDSPTPCDataInterfaceParent {

 public:

  PDHDDataInterface(fhicl::ParameterSet const& ps);
  ~PDHDDataInterface() { };

  int retrieveData (art::Event &evt, std::string inputlabel,
                    std::vector<raw::RawDigit> &raw_digits,
                    std::vector<raw::RDTimeStamp> &rd_timestamps,
                    std::vector<raw::RDStatus> &rdstatuses);


  int retrieveDataAPAListWithLabels(
      art::Event &evt, std::string inputlabel,
      std::vector<raw::RawDigit> &raw_digits,
      std::vector<raw::RDTimeStamp> &rd_timestamps,
      std::vector<raw::RDStatus> &rdstatuses, 
      std::vector<int> &apalist);

  int retrieveDataForSpecifiedAPAs(
      art::Event &evt, std::vector<raw::RawDigit> &raw_digits,
      std::vector<raw::RDTimeStamp> &rd_timestamps,
      std::vector<raw::RDStatus> &rdstatuses,  
      std::vector<int> &apalist);

 private:

  std::map<int,std::vector<std::string>> _input_labels_by_apa;
  void getFragmentsForEvent (dunedaq::hdf5libs::HDF5RawDataFile::record_id_t &rid, RawDigits& raw_digits,
                             RDTimeStamps &timestamps, int apano);
  void getMedianSigma (const raw::RawDigit::ADCvector_t &v_adc, float &median,
                       float &sigma);

  //For nicer log syntax
  std::string logname = "PDHDDataInterface";
  std::string fFileInfoLabel;

  unsigned int fMaxChan = 1000000;  // no maximum for now
  unsigned int fDefaultCrate = 1;
  int fDebugLevel = 0;   // switch to turn on debugging printout

};

#endif
