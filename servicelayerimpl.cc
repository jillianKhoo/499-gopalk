#include "servicelayerimpl.h"

#include <google/protobuf/util/time_util.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;

using chirp::Chirp;
using chirp::RegisterRequest;
using chirp::RegisterReply;
using chirp::ChirpRequest;
using chirp::ChirpReply;
using chirp::FollowRequest;
using chirp::FollowReply;
using chirp::ReadRequest;
using chirp::ReadReply;
using chirp::MonitorRequest;
using chirp::MonitorReply;

// register user with backend service
Status ServiceLayerImpl::registeruser(ServerContext* context, const RegisterRequest* request,
                RegisterReply* reply){
  // TODO: register with backend
  if(users.find(request->username()) == users.end()) {
    users.insert(request->username());
    return Status::OK;
  }
  else {
    std::cout << "User is already in the database" << std::endl;
    return Status(StatusCode::ALREADY_EXISTS, "username has already been taken");
  }
}

// allow user to send chirp and register with backend
Status ServiceLayerImpl::chirp(ServerContext* context, const ChirpRequest* request,
                ChirpReply* reply){
  // TODO: insert chirp into backend service
  if(users.find(request->username()) == users.end()) {
    return Status(StatusCode::INVALID_ARGUMENT, "user does not exist");
  }
  if(!request->parent_id().empty() && std::stoi(request->parent_id()) >= chirps.size()) {
    return Status(StatusCode::INVALID_ARGUMENT, "parent_id not valid");
  }
  Chirp *chirp = new Chirp();
  //google::protobuf::util::TimeUtil::Timestamp ts = google::protobuf::util::TimeUtil::GetCurrentTime();
  int64_t seconds = google::protobuf::util::TimeUtil::TimestampToSeconds(google::protobuf::util::TimeUtil::GetCurrentTime());
  int64_t useconds = google::protobuf::util::TimeUtil::TimestampToMicroseconds(google::protobuf::util::TimeUtil::GetCurrentTime());
  chirp::Timestamp* ts = new chirp::Timestamp();
  ts->set_seconds(seconds);
  ts->set_useconds(useconds);
  chirp->set_allocated_timestamp(ts);
  chirp->set_username(request->username());
  chirp->set_text(request->text());
  chirp->set_id(std::to_string(chirps.size()));
  chirp->set_parent_id(request->parent_id());
  reply->set_allocated_chirp(chirp);
  return Status::OK;
}

// allow user to follow another user (store in backend)
Status ServiceLayerImpl::follow(ServerContext* context, const FollowRequest* request,
                FollowReply* reply){
  //TODO: allow chirp to follow another user by calling backend service
  return Status::OK;
}

// allow user to read a thread
Status ServiceLayerImpl::read(ServerContext* context, const ReadRequest* request,
                ReadReply* reply){
  //TODO: get thread from backend service and return
  return Status::OK;
}

// allow user to monitor followers
Status ServiceLayerImpl::monitor(ServerContext* context, const MonitorRequest* request,
                MonitorReply* reply){
 //TODO: process user's following list and broadcast chirps
  return Status::OK;
}
