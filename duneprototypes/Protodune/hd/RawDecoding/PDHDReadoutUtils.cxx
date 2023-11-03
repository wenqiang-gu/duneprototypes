#include "PDHDReadoutUtils.h"
bool pdhd::rawdecoding::CheckSourceIsDetector(const SourceID & id) {
  return (id.subsystem == SourceID::Subsystem::kDetectorReadout);
}
