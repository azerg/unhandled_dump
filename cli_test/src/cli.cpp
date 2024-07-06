#include "last_dumper.h"
#include <filesystem>

#include <Windows.h>

using namespace crash_handler;

int main() {
  const auto dumper = InitLastDumper("");
  int* ii{};
  *ii = GetTickCount() > 0 ? 0 : 1234;
}