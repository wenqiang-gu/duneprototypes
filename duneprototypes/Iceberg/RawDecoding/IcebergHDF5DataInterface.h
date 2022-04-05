////////////////////////////////////////////////////////////////////////
// IcebergHDF5DataInterface.h
//
// Tool to unpack FELIX fragments in HDF5 format.  To be used with the HDF5RawInput source
//
// These methods take references to vectors of raw::RawDigit, raw::RDTimeStamp, and raw::RDStatus data products as arguments.
// These vectors are not cleared on input, and so when data are retrieved, they are appended to any existing data already
// in the vectors.  The RDStatus vector is an exception, where just one RDStatus instance will be in the vector.  Previously
// accumulated RDStatus values from previous calls will be logically ORed into the single RDStatus instances contents.
//
//  Methods are provided to retrieve all data for the event, or by specified APA list.  In cases where
//  data from a specified APA are requested but no labels are provided by the caller, labels are input via FCL parameters.
//  This is true because data from an APA may appear with different labels during the course of the ProtoDUNE-SP run.
//
/////////////////////////////////////////////////////////////////////////
#ifndef IcebergHDF5DataInterface_H
#define IcebergHDF5DataInterface_H

#include <vector>

#include "art/Utilities/ToolMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
//#include "art/Framework/Principal/Run.h"
//#include "art/Framework/Principal/SubRun.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "art/Persistency/Common/PtrMaker.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/RDTimeStamp.h"
#include "dunecore/DuneObj/PDSPTPCDataInterfaceParent.h"
#include <hdf5.h>

class IcebergHDF5DataInterface : public PDSPTPCDataInterfaceParent {

 public:

  IcebergHDF5DataInterface(fhicl::ParameterSet const& ps);

  int retrieveData(art::Event &evt, std::string inputlabel, std::vector<raw::RawDigit> &raw_digits, std::vector<raw::RDTimeStamp> &rd_timestamps,
                   std::vector<raw::RDStatus> &rdstatuses );

  // method to get raw digits, RDTimeStamps, RDStatuses from all input fragments specified by an input label (like "daq:ContainerTPC") but ony for
  // APA's (== crate numbers) on a list.  If the list contains a -1 in it, it returns all APA data found in the input label.

  int retrieveDataAPAListWithLabels(art::Event &evt, std::string inputlabel, std::vector<raw::RawDigit> &raw_digits, std::vector<raw::RDTimeStamp> &rd_timestamps,
                                    std::vector<raw::RDStatus> &rdstatuses, 
                                    std::vector<int> &apalist);

  // method to get raw digits, RDTimeStamps, RDStatuses for a specified list of APA's.  The list of possible labels on which to find
  // APA data is proved by fcl configuration.

  int retrieveDataForSpecifiedAPAs(art::Event &evt, std::vector<raw::RawDigit> &raw_digits, std::vector<raw::RDTimeStamp> &rd_timestamps,
                                   std::vector<raw::RDStatus> &rdstatuses,  
                                   std::vector<int> &apalist);

  // inputLabel not used for HDF5 input
  // returns:  0:  success, or   1: discarded corrupted data, or 2: kept some corrupted data

 private:

  std::map<int,std::vector<std::string>> _input_labels_by_apa;

  long int _min_offline_channel;  // min offline channel to decode.  <0: no limit
  long int _max_offline_channel;  // max offline channel to decode.  <0: no limit.  max<min: no limit
  bool     _debugprint;             // print the crate, slot and fiber number to stdout when called

  std::string _FileInfoLabel;     // art input label for the HDF5 file info data product

  // some convenience typedefs for porting old code

  typedef std::vector<raw::RawDigit> RawDigits;
  typedef std::vector<raw::RDTimeStamp> RDTimeStamps;
  typedef std::vector<raw::RDStatus> RDStatuses;

  // private methods

  void getIcebergHDF5Data(hid_t the_group, RawDigits& raw_digits, RDTimeStamps &timestamps, int apano);

  void _collectRDStatus(std::vector<raw::RDStatus> &rdstatuses);

  void computeMedianSigma(const raw::RawDigit::ADCvector_t &v_adc, 
                          float &median, 
                          float &sigma);

};


#endif
