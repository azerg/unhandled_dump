#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-nice-strict.h>
#include "last_dumper.h"

using namespace testing;
using namespace crash_handler;
using namespace dumper_api;

class MockCore: public ICore {
public:
  MOCK_METHOD(void*, InitCrashHandler, (std::string_view dumpFolderPath, std::vector<std::unique_ptr<ICatchingMode>> handlers));
  MOCK_METHOD(void, FreeCrashHandler, (void* handle), (const));
};

class LastDumperTest : public testing::Test {
protected:
  LastDumperTest() = default;

  void SetUp() override {
    lastDumper_ = std::make_unique<LastDumper>("Z:\\test", std::make_unique<NiceMock<MockCore>>(), std::vector<std::unique_ptr<dumper_api::ICatchingMode>>{});
  }

  void TearDown() override {
  }

  std::unique_ptr<LastDumper> lastDumper_;
};

TEST_F(LastDumperTest, DoubleInitializationExceptionTest) {
  EXPECT_THROW({
      auto lastDumper2 = std::make_unique<LastDumper>("Z:\\test2", std::make_unique<MockCore>(), std::vector<std::unique_ptr<dumper_api::ICatchingMode>>{});
    }, std::logic_error);
}