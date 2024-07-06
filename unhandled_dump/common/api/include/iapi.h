#pragma once

#include <functional>
#include <string_view>
#include <system_error>
#include <vector>
#include <memory>


namespace dumper_api {
  enum class IOResult : int8_t {
    ok,
    failed
  };

  struct ExceptionInfo {
    uint64_t code;
    void* rawInfo = {};
  };

  struct ICatchingMode {
    virtual ~ICatchingMode() = default;
    virtual IOResult Handle(ExceptionInfo info, std::string_view outputFolder) = 0;
  };

  struct ICore {
    virtual ~ICore() = default;

    virtual void* InitCrashHandler([[maybe_unused]] std::string_view dumpFolderPath, std::vector<std::unique_ptr<ICatchingMode>> handlers) = 0;
    virtual void FreeCrashHandler(void* handle = nullptr) const = 0;
  };
} // namespace dumper_api

