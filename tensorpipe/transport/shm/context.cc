/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <tensorpipe/transport/shm/context.h>

#include <tensorpipe/common/system.h>
#include <tensorpipe/transport/shm/connection.h>
#include <tensorpipe/transport/shm/listener.h>
#include <tensorpipe/transport/shm/loop.h>
#include <tensorpipe/transport/shm/socket.h>

namespace tensorpipe {
namespace transport {
namespace shm {

namespace {

// Prepend descriptor with transport name so it's easy to
// disambiguate descriptors when debugging.
const std::string kDomainDescriptorPrefix{"shm:"};

std::string generateDomainDescriptor() {
  auto bootID = getBootID();
  TP_THROW_ASSERT_IF(!bootID) << "Unable to read boot_id";
  return kDomainDescriptorPrefix + bootID.value();
}

} // namespace

Context::Context()
    : loop_(std::make_shared<Loop>()),
      domainDescriptor_(generateDomainDescriptor()) {}

void Context::close() {
  loop_->close();
}

void Context::join() {
  close();
  loop_->join();
}

Context::~Context() {
  join();
}

std::shared_ptr<transport::Connection> Context::connect(std::string addr) {
  return std::make_shared<Connection>(
      Connection::ConstructorToken(), loop_, std::move(addr));
}

std::shared_ptr<transport::Listener> Context::listen(std::string addr) {
  return std::make_shared<Listener>(
      Listener::ConstructorToken(), loop_, std::move(addr));
}

const std::string& Context::domainDescriptor() const {
  return domainDescriptor_;
}

} // namespace shm
} // namespace transport
} // namespace tensorpipe
