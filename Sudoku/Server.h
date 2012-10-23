#ifndef __Server_H
#define __Server_H

#include "Session.h"

#include <boost/bind.hpp>
#include <boost/asio.hpp>

class CSudokuDlg;

class Server
{

public:
  Server(boost::asio::io_service &ioService, short port, CSudokuDlg *dlg);
  ~Server();

private:
  void startAccept();
  void handleAccept(const boost::system::error_code& error);

  SessionPtr _pSession;
  boost::asio::io_service &_ioService;
  tcp::acceptor _acceptor;
  CSudokuDlg *_dlg;
};

#endif // __Server_H
