#include "stdafx.h"
#include "Server.h"
#include "SudokuDlg.h"

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_service &ioService, short port, CSudokuDlg *dlg)
  : _ioService(ioService)
  , _acceptor(_ioService)
  , _dlg(dlg)
{
  _acceptor.open(tcp::v4());
  _acceptor.bind(tcp::endpoint(tcp::v4(), port));
  _acceptor.listen();
  startAccept();
}

Server::~Server()
{
  _acceptor.close();
  _pSession->stop();
}

void Server::startAccept()
{
  _pSession.reset(new Session(_ioService, _dlg));
  _acceptor.async_accept(_pSession->socket(),
    boost::bind(&Server::handleAccept, this,
    boost::asio::placeholders::error));
}

void Server::handleAccept(const boost::system::error_code& error)
{
  if (!_acceptor.is_open()) {
    return;
  }

  if (!error) {
    _pSession->start();
  }

  startAccept();
}