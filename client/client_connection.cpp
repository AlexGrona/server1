// http://code.google.com/p/server1/
//
// You can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Author: xiliu.tang@gmail.com (Xiliu Tang)

#include "client/client_connection.hpp"
void ClientConnection::ConnectionClose() {
  VLOG(2) << "ClientConnection::ConnectionClose";
  notifier_->Notify();
}
bool ClientConnection::Connect() {
  if (IsConnected()) {
    LOG(WARNING) << "Connect but IsConnected";
    return true;
  }
  if (out_threadpool_  == NULL) threadpool_.Start();
  if (out_io_service_pool_ == NULL) io_service_pool_.Start();
  boost::asio::ip::tcp::resolver::query query(server_, port_);
  boost::asio::ip::tcp::resolver resolver(GetIOService());
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator(
      resolver.resolve(query));
  boost::asio::ip::tcp::resolver::iterator end;
  // Try each endpoint until we successfully establish a connection.
  boost::system::error_code error = boost::asio::error::host_not_found;
  boost::asio::ip::tcp::socket *socket =
    new boost::asio::ip::tcp::socket(GetIOService());
  while (error && endpoint_iterator != end) {
    socket->close();
    socket->connect(*endpoint_iterator++, error);
  }
  if (error) {
    delete socket;
    LOG(WARNING) << ":fail to connect, error:"  << error.message();
    return false;
  }
  notifier_.reset(new Notifier);
  ProtobufConnection *connection = connection_template_.Clone();
  connection->set_socket(socket);
  if (out_threadpool_) {
    connection->set_executor(out_threadpool_);
  } else {
    connection->set_executor(&threadpool_);
  }
  proxy_ = FullDualChannelProxy::Create(connection);
  proxy_->close_signal()->connect(
      boost::bind(&ClientConnection::ConnectionClose, this));
  return true;
}