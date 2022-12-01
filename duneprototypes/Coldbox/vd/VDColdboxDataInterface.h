#ifndef VDColdboxDataInterface_H
#define VDColdboxDataInterface_H

#include "art/Utilities/ToolMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Persistency/Common/PtrMaker.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/RDTimeStamp.h"
#include "artdaq-core/Data/Fragment.hh"
#include "dunecore/DuneObj/PDSPTPCDataInterfaceParent.h"
#include "daqdataformats/v3_3_3/Fragment.hpp"
#include <hdf5.h>

typedef dunedaq::daqdataformats::Fragment duneFragment;
typedef std::vector<duneFragment> duneFragments; 

typedef std::vector<raw::RawDigit> RawDigits;
typedef std::vector<raw::RDTimeStamp> RDTimeStamps;

class VDColdboxDataInterface : public PDSPTPCDataInterfaceParent {

 public:

  VDColdboxDataInterface(fhicl::ParameterSet const& ps);
  ~VDColdboxDataInterface() {
    if (fForceOpen) {
      H5Fclose(fHDFFile);
    }
  };
  int retrieveData(art::Event &evt, std::string inputlabel,
                   std::vector<raw::RawDigit> &raw_digits,
                   std::vector<raw::RDTimeStamp> &rd_timestamps,
                   std::vector<raw::RDStatus> &rdstatuses );

  void readFragmentsForEvent (art::Event &evt);

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
  void _collectRDStatus(std::vector<raw::RDStatus> &rdstatuses){};
  void getFragmentsForEvent(hid_t the_group, RawDigits& raw_digits,
                            RDTimeStamps &timestamps, int apano,
                            int maxchan);
  void getMedianSigma(const raw::RawDigit::ADCvector_t &v_adc, float &median,
                      float &sigma);

  //For nicer log syntax
  std::string logname = "VDColdboxDataInterface";
  hid_t fPrevStoredHandle = -1;
  hid_t fHDFFile = -1;
  bool fForceOpen;
  std::string fFileInfoLabel;

  int fMaxChan = 1000000;  // no maximum for now

};

#endif
