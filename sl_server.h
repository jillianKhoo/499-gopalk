#ifndef CHIRP_SL_SERVER_H
#define CHIRP_SL_SERVER_H

#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/util/time_util.h>
#include <grpcpp/grpcpp.h>
#include "Backend.grpc.pb.h"
#include "Backend.pb.h"
#include "KeyValueStore.grpc.pb.h"
#include "KeyValueStore.pb.h"
#include "ServiceLayer.grpc.pb.h"
#include "ServiceLayer.pb.h"
#include "kvs_client.h"
#include "sl_functionality.h"

using chirp::Chirp;
using chirp::ChirpReply;
using chirp::ChirpRequest;
using chirp::Chirps;
using chirp::Followers;
using chirp::FollowReply;
using chirp::FollowRequest;
using chirp::MonitorReply;
using chirp::MonitorRequest;
using chirp::ReadReply;
using chirp::ReadRequest;
using chirp::RegisterReply;
using chirp::RegisterRequest;
using chirp::Replies;
using chirp::ServiceLayer;
using chirp::StreamReply;
using chirp::StreamRequest;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using grpc::StatusCode;

// implementation of service layer
// takes request from command line clients
class ServiceLayerServer final : public ServiceLayer::Service {
 public:
  // constructor, initializes KeyValueClient
  ServiceLayerServer();
  // register user with backend service
  Status registeruser(ServerContext* context, const RegisterRequest* request,
                      RegisterReply* reply) override;
  // allow user to send chirp and register with backend
  Status chirp(ServerContext* context, const ChirpRequest* request,
               ChirpReply* reply) override;
  // allow user to follow another user (store in backend)
  Status follow(ServerContext* context, const FollowRequest* request,
                FollowReply* reply) override;
  // allow user to read a thread
  Status read(ServerContext* context, const ReadRequest* request,
              ReadReply* reply) override;
  // allow user to stream chirps with a given hashtag
  Status stream(ServerContext* context, const StreamRequest* request,
                ServerWriter<StreamReply>* writer) override;
  // allow user to monitor followers
  Status monitor(ServerContext* context, const MonitorRequest* request,
                 ServerWriter<MonitorReply>* writer) override;

 private:
  // ServiceLayerFunctionality object is used to perform user commands
  // accessing the backend
  ServiceLayerFunctionality sl_func_;
};
#endif  // CHIRP_SL_SERVER_H
