#pragma once
#ifdef WIN32
#include "iapi.h"
#include <optional>

namespace dumper_api::handlers{

  struct MinidumpCatchingMode:ICatchingMode {
    IOResult Handle(ExceptionInfo info, std::string_view outputFolder) override;
  };

  namespace details {
    std::optional<std::string> genMinidump(void* exceptionData, std::string destinationFolder = "");
  } // namespace details
} // namespace dumper_api::handlers
#endif