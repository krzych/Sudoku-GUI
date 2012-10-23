#ifndef __Session_H
#define __Session_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost\noncopyable.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <boost\shared_ptr.hpp>
#include <string>

using boost::asio::ip::tcp;

class CSudokuDlg;

class Session
  : public boost::enable_shared_from_this<Session>
  , private boost::noncopyable
{

public:
  Session(boost::asio::io_service &ioService, CSudokuDlg *dlg);
  ~Session();
  tcp::socket &socket();
  void start();
  void stop();

private:
  void handleOK(const boost::system::error_code &error);
  void handleGetFrame(const boost::system::error_code& error, size_t bytes_transferred);

  tcp::socket _socket;
  CSudokuDlg *_dlg;
  enum { max_length = 81 };
  char _data[max_length];
  unsigned int _lineCounter;
};

typedef boost::shared_ptr<Session> SessionPtr;

#endif // __Session_H
