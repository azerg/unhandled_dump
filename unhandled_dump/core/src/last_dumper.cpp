#include "last_dumper.h"

namespace crash_handler {

  namespace {
    std::atomic_flag already_init;
  } // namespace

  LastDumper::~LastDumper()
  {
    core_->FreeCrashHandler(hCrashHandler_);
  }

  LastDumper::LastDumper(const std::string& target_folder, std::unique_ptr<dumper_api::ICore> core, std::vector<std::unique_ptr<dumper_api::ICatchingMode>> handlers) :
    target_folder_(target_folder),
    core_(std::move(core)){

    if (already_init.test_and_set())
      throw std::logic_error("We can init LastDumper only once per process");

    hCrashHandler_ = core_->InitCrashHandler(target_folder, std::move(handlers));
  }
} // namespace crash_handler
