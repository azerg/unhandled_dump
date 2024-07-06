#pragma once

#include "types.h"

#include <filesystem>
#include <grpcpp/grpcpp.h>
#include "agent.grpc.pb.h"

using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using google::protobuf::Empty;
using google::protobuf::Struct;

namespace native {
  class AgentClient {
  public:
    AgentClient(std::shared_ptr<Channel> channel)
      : stub_(Agent::NewStub(channel)) {}

    // Executes a command on the remote system
    ProcessResponse Exec(const CommandRequest& request) {
      ProcessResponse response;
      ClientContext context;

      Status status = stub_->Exec(&context, request, &response);

      if (status.ok()) {
        return response;
      }
      else {
        // Handle error
        return ProcessResponse();
      }
    }

    // Lists files and directories at the given path
    FilesResponse List(std::filesystem::path& startFolder) {
      ListRequest request;
      request.set_path(startFolder.u8string());
      request.set_recursive(true);

      FilesResponse response;
      ClientContext context;

      std::unique_ptr<grpc::ClientReader<FilesResponse>> reader(
        stub_->List(&context, request));

      while (reader->Read(&response)) {
        for (auto file : response.files()) {
          std::cout << file.name() << std::endl;
        }
      }

      Status status = reader->Finish();

      if (status.ok()) {
        return response;
      }
      else {
        // Handle error
        return FilesResponse();
      }
    }

    FilePart PullFile(const std::string& path, uint64_t offset, int64_t size) {
      PullRequest request;
      request.set_path(path);
      if (offset) {
        request.set_offset(offset);
      }
      if (size) {
        request.set_size(size);
      }

      FilePart response;
      ClientContext context;

      std::unique_ptr<ClientReader<FilePart>> reader(stub_->PullFile(&context, request));

      while (reader->Read(&response)) {
        std::cout << "Read " << response.data().size() << " bytes" << std::endl;
        auto data = response.data();
        data.c_str();
      }

      Status status = reader->Finish();

      if (!status.ok()) {
        std::cerr << "PullFile RPC failed: " << status.error_code()
          << ", " << status.error_message() << std::endl;
      }

      return response;
    }

    PullResponse Backup(const std::string& starting_path, const std::string& deprecated_path) {
      BackupRequest request;
      request.set_path(starting_path);
      request.set_starting_path(deprecated_path);

      PullResponse response;
      ClientContext context;

      std::unique_ptr<ClientReader<PullResponse>> reader(stub_->Backup(&context, request));

      while (reader->Read(&response)) {
        auto nFiles = response.files_size();
        for (int i = 0; i < nFiles; i++) {
          auto file = response.files(i);
          std::cout << file.file().name() << std::endl;
        }
        
      }

      Status status = reader->Finish();

      if (!status.ok()) {
        std::cerr << "Backup RPC failed: " << status.error_code()
          << ", " << status.error_message() << std::endl;
      }

      return response;
    }

    auto CountFiles(const std::string& path) -> std::pair<uint64_t, uint64_t> {
      CountFilesRequest request;
      request.set_path(path);

      CountFilesResponse response;
      ClientContext context;

      std::unique_ptr<ClientReader<CountFilesResponse>> reader(
        stub_->CountFiles(&context, request));

      uint64_t count = 0;
      uint64_t total_size = 0;

      while (reader->Read(&response)) {
        count += response.count();
        total_size = response.total_size();
      }

      Status status = reader->Finish();
      if (status.ok()) {
        return std::make_pair(count, total_size);
      }
      else {
        std::cout << "CountFiles rpc failed with error code "
          << status.error_code() << ": " << status.error_message()
          << std::endl;
        return std::make_pair(0,0);
      }
    }

    Struct GetProperties() {
      Empty request;
      Struct response;
      ClientContext context;

      Status status = stub_->GetProperties(&context, request, &response);

      if (status.ok()) {
        return response;
      }
      else {
        std::cout << "GetProperties rpc failed with error code "
          << status.error_code() << ": " << status.error_message()
          << std::endl;
        return Struct();
      }
    }

  private:
    std::unique_ptr<Agent::Stub> stub_;
  };
} // namespaxce native