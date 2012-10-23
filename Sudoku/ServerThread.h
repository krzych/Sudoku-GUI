#ifndef __ServerThread_H
#define __ServerThread_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>

class Server;
class CSudokuDlg;

class ServerThread
{

public:
  ServerThread(CSudokuDlg *dlg);
  ~ServerThread();

private:
  void run();

  CSudokuDlg *_dlg;
  boost::asio::io_service _ioService;
  Server *_server;
  boost::thread _thread;
};

#endif // __ServerThread_H
