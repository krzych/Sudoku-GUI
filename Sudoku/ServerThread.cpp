#include "stdafx.h"
#include "ServerThread.h"
#include "Server.h"
#include <boost/bind.hpp>
#include "resource.h"
#include "SudokuDlg.h"

ServerThread::ServerThread(CSudokuDlg *dlg) 
  : _dlg(dlg)
  , _thread(boost::bind(&ServerThread::run, this))

{

}

ServerThread::~ServerThread()
{
  _ioService.stop();
  _thread.join();
}

void ServerThread::run()
{
  _server = new Server(_ioService, 23, _dlg);
  _ioService.run();
  delete _server;
}