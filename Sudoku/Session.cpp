#include "stdafx.h"
#include "Session.h"
#include "SudokuDlg.h"

Session::Session(boost::asio::io_service &ioService, CSudokuDlg *dlg)
  : _socket(ioService)
  , _dlg(dlg)
{

}

Session::~Session()
{

}

tcp::socket &Session::socket()
{
  return _socket;
}

void Session::start()
{
  char str[] = "OK";
  boost::asio::async_write(_socket,
    boost::asio::buffer(str, sizeof(str)),
    boost::bind(&Session::handleOK, shared_from_this(),
    boost::asio::placeholders::error));
}

void Session::stop()
{
  _socket.close();
}

void Session::handleOK(const boost::system::error_code &error)
{
  _socket.async_read_some(boost::asio::buffer(_data, max_length),
    boost::bind(&Session::handleGetFrame, shared_from_this(),
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred)); 
}

void Session::handleGetFrame(const boost::system::error_code& error, size_t bytes_transferred)
{
  if (_data[0] == 'G') {
    _dlg->OnBnClickedBtnAcquire();
    start();
  }
  else {
    handleOK(error);
  }
}