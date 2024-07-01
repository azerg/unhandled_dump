#ifdef WIN32
#include "iapi.h"
#include "last_dumper.h"
#include "handlers/windows/minidump.h"
#include <fstream>
#include <optional>

#include <Windows.h>
#include <DbgHelp.h>

namespace dumper_api {
  namespace {
    std::vector<std::unique_ptr<ICatchingMode>> crashHandlers;
    std::string dumpPath{};
    std::atomic_flag dumped_already;

    LONG VectoredExceptionHandler(_EXCEPTION_POINTERS* exceptionInfo)
    {
      if (dumped_already.test_and_set()) {
        // looks like user code causes a crash, we cant do here anything but dump and quit
        handlers::details::genMinidump(exceptionInfo, dumpPath);
        return EXCEPTION_EXECUTE_HANDLER;
      }

      // skip non-critical exceptions
      if (exceptionInfo->ExceptionRecord->ExceptionCode < 0x80000000)
        return EXCEPTION_CONTINUE_SEARCH;

      for (auto& handler : crashHandlers) {
        handler->Handle(dumper_api::ExceptionInfo{
          exceptionInfo->ExceptionRecord->ExceptionCode,
          reinterpret_cast<void*>(exceptionInfo)
          }, dumpPath);
      }
      return EXCEPTION_CONTINUE_SEARCH;
    }
  } // namespace

  struct WinCore: ICore {
    void* InitCrashHandler([[maybe_unused]] std::string_view dumpFolderPath, std::vector<std::unique_ptr<ICatchingMode>> handlers) override {
      crashHandlers = std::move(handlers);

      dumpPath = dumpFolderPath;
      void* res = {};
      res = AddVectoredExceptionHandler(0, VectoredExceptionHandler);
      return res;
    };

    void FreeCrashHandler(void* handle = nullptr) override {
      RemoveVectoredExceptionHandler(handle);
    };
  };
} // namespace dumper_api

namespace crash_handler {

  namespace {
    std::unique_ptr<dumper_api::ICatchingMode> CreateHandler(Handler handlerId) {
      switch (handlerId)
      {
      case Handler::genMinidump:
        return std::make_unique<dumper_api::handlers::MinidumpCatchingMode>();
      default:
        throw std::logic_error("Not implemented");
      }
    }
  } // namespace

  std::unique_ptr<LastDumper> InitLastDumper(const std::string& target_folder){
    return InitLastDumper(target_folder, { Handler::genMinidump });
  }
  std::unique_ptr<LastDumper> InitLastDumper(const std::string& target_folder, std::vector<Handler> handlers) {
    std::vector<std::unique_ptr<dumper_api::ICatchingMode>> obs;
    for (auto& id : handlers)
      obs.emplace_back(CreateHandler(id));

    return std::make_unique<LastDumper>(target_folder, std::make_unique<dumper_api::WinCore>(), std::move(obs));
  }
}

#endif