#ifdef WIN32
#include "handlers/windows/minidump.h"
#include "last_dumper.h"

#include <optional>
#include <Windows.h>
#include <Dbghelp.h>

namespace dumper_api::handlers {

  IOResult MinidumpCatchingMode::Handle(ExceptionInfo info, std::string_view outputFolder)
  {
    if (!info.rawInfo)
      return IOResult::failed;
    details::genMinidump(reinterpret_cast<_EXCEPTION_POINTERS*>(info.rawInfo));
    return IOResult::ok;
  }

  namespace details {
    std::optional<std::string> genMinidump(void* e, std::string destinationFolder) {
      auto hDbgHelp = LoadLibraryA("dbghelp");
      if (hDbgHelp == nullptr)
        return {};
      auto pMiniDumpWriteDump = (decltype(&MiniDumpWriteDump))GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
      if (pMiniDumpWriteDump == nullptr)
        return {};

      char name[MAX_PATH];
      {
        auto nameEnd = name + GetModuleFileNameA(GetModuleHandleA(0), name, MAX_PATH);
        SYSTEMTIME t;
        GetSystemTime(&t);
        wsprintfA(nameEnd - strlen(".exe"),
          "_%4d%02d%02d_%02d%02d%02d.dmp",
          t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
      }

      auto hFile = CreateFileA(name, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
      if (hFile == INVALID_HANDLE_VALUE)
        return {};

      MINIDUMP_EXCEPTION_INFORMATION exceptionInfo{};
      exceptionInfo.ThreadId = GetCurrentThreadId();
      exceptionInfo.ExceptionPointers = reinterpret_cast<EXCEPTION_POINTERS*>(e);
      exceptionInfo.ClientPointers = FALSE;

      auto dumped = pMiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        MINIDUMP_TYPE(
          MiniDumpWithFullMemory |
          MiniDumpWithFullMemoryInfo |
          MiniDumpScanMemory |
          MiniDumpWithProcessThreadData |
          MiniDumpWithHandleData |
          MiniDumpWithThreadInfo |
          MiniDumpWithUnloadedModules |
          MiniDumpWithDataSegs
        ),
        e ? &exceptionInfo : nullptr,
        nullptr,
        nullptr);

      CloseHandle(hFile);

      return dumped == TRUE ? std::make_optional(name) : std::nullopt;
    }
  }// namespace details
} // namespace dumper_api::handlers

#endif