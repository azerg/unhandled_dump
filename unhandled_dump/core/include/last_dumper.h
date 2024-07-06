#pragma once
#include <memory>
#include <ostream>
#include <functional>

#include <iapi.h>

namespace crash_handler {

  class LastDumper final {
  public:
    LastDumper(const LastDumper&) = delete;
    ~LastDumper();
    LastDumper(const std::string& target_folder, std::unique_ptr<dumper_api::ICore> core, std::vector<std::unique_ptr<dumper_api::ICatchingMode>> handlers);
  private:
    std::unique_ptr<dumper_api::ICore> core_;
    std::string target_folder_;
    void* hCrashHandler_{};
  };

  enum class Handler {
    genMinidump
  };

  std::unique_ptr<LastDumper> InitLastDumper(const std::string& target_folder);
  std::unique_ptr<LastDumper> InitLastDumper(const std::string& target_folder, std::vector<Handler> handlers);
} // namespace crash_handler