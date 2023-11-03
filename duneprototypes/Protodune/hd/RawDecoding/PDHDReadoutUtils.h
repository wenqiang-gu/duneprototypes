#ifndef PDHDREADOUTUTILS_H
#define PDHDREADOUTUTILS_H
#include "daqdataformats/v4_0_0/SourceID.hpp"

namespace pdhd {
namespace rawdecoding {
  
  using dunedaq::daqdataformats::SourceID;

  bool CheckSourceIsDetector(const SourceID & id);

}
}
#endif
