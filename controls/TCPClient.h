#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <thread>
#include <iostream>
#include <sstream>
#include <deque>
#include <boost/asio.hpp>
#include <boost/optional.hpp>

#include "../models/Beacon.h"
#include "../models/Message.h"
#include "../semaphore.hpp"

#include <mutex>
#include <condition_variable>




using boost::asio::ip::tcp;

typedef std::deque<Message> message_queue;

class TCPClient {
public:

    TCPClient(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator);
    void write(const Message msg);
    void close();
    
    //tcp::socket get();
    size_t getReadData(char *msg);
    size_t getReadLen();
    
    size_t getReadBodyData(char *msg);
    size_t getReadBodyLen();

private:
    void do_connect(tcp::resolver::iterator endpoint_iterator);
    void do_read_body();
    void do_write();


private:
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    char read_msg_[100];
    message_queue write_msgs_;
    size_t read_len;
};



#endif /* TCPCLIENT_H */

