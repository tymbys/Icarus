#include "TCPClient.h"

using boost::asio::ip::tcp;
using namespace std;

template <typename SyncReadStream, typename MutableBufferSequence>
void readWithTimeout(SyncReadStream& s, const MutableBufferSequence& buffers, const boost::asio::deadline_timer::duration_type& expiry_time) {
    boost::optional<boost::system::error_code> timer_result;
    boost::asio::deadline_timer timer(s.get_io_service());
    timer.expires_from_now(expiry_time);
    timer.async_wait([&timer_result] (const boost::system::error_code & error) {
        timer_result.reset(error); });

    boost::optional<boost::system::error_code> read_result;
    boost::asio::async_read(s, buffers, [&read_result] (const boost::system::error_code& error, size_t) {
        read_result.reset(error); });

    s.get_io_service().reset();
    while (s.get_io_service().run_one()) {
        if (read_result)
            timer.cancel();
        else if (timer_result)
            s.cancel();
    }

    if (*read_result)
        throw boost::system::system_error(*read_result);
}

TCPClient::TCPClient(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator) : io_service_(io_service), socket_(io_service) {
    do_connect(endpoint_iterator);
}

void TCPClient::write(const Message msg) {
    io_service_.post(
            [this, msg]() {
                bool write_in_progress = !write_msgs_.empty();
                write_msgs_.push_back(msg);
                if (!write_in_progress) {
                    do_write();
                }
            });
}

void TCPClient::close() {
    io_service_.post([this]() {
        socket_.close(); });
}

void TCPClient::do_connect(tcp::resolver::iterator endpoint_iterator) {
    boost::asio::async_connect(socket_, endpoint_iterator,
            [this](boost::system::error_code ec, tcp::resolver::iterator) {
                if (!ec) {
                    //do_read_header();
                    //do_read_body();
                    do_write();
                }
            });
}

void TCPClient::do_read_body() {

    //    boost::system::error_code r_ec;
    //    std::size_t bytes_transferred = boost::asio::read(socket_, boost::asio::buffer(&read_msg_, sizeof (Beacon)), r_ec);
    //    
    //    if (r_ec) {
    //        
    //    }
    
    
    readWithTimeout(socket_, boost::asio::buffer(&read_msg_, sizeof (Beacon)), 1);


    //    Semaphore r_sem;
    //    boost::system::error_code r_ec;
    //
    //    boost::asio::async_read(socket_,
    //            boost::asio::buffer(&read_msg_, sizeof (read_msg_)),
    //            [this/*, &r_ec, &r_sem*/](boost::system::error_code ec, std::size_t /*length*/) {
    //
    //                //r_ec = ec;
    //                //r_sem.notify();
    //
    //                if (!ec) {
    //                    std::cout << std::hex;
    //                    //std::cout.write(reinterpret_cast<char  *>(read_msg_.data), 1);
    //                    //read_msg_[1] = 0;
    //                    //read_msg_[2] = 0;
    //                    //read_msg_[3] = 0;
    //                    std::cout << (int) read_msg_[0];
    //                    std::cout << "\n";
    //
    //                    //                    switch (read_msg_.b1) {
    //                    //
    //                    //                    }
    //
    //
    //
    //                    //do_read_body();
    //                } else {
    //                    //socket_.close();
    //                }
    //                socket_.close();
    //
    //            });

    /*    
            if (!r_sem.wait_for(std::chrono::seconds(3))) // wait for 3 seconds
            {            
                //socket_.cancel();
                //throw boost::system::system_error(boost::asio::error::try_again);
            } else if (r_ec)
                throw boost::system::system_error(r_ec);
     */
}

void TCPClient::do_write() {
    //    boost::asio::async_write(socket_,
    //            boost::asio::buffer(&write_msgs_.front(), sizeof (Beacon)),
    //            [this](boost::system::error_code ec, std::size_t /*length*/) {
    //                if (!ec) {
    //                    write_msgs_.pop_front();
    //                    if (!write_msgs_.empty()) {
    //                        //do_write();
    //                        do_read_body();
    //                    }
    //                } else {
    //                    socket_.close();
    //                }
    //            });

    Message m = write_msgs_.front();

    //boost::asio::write(socket_, boost::asio::buffer(&write_msgs_.front(), sizeof(Beacon)));
    boost::asio::write(socket_, boost::asio::buffer(m.data(), sizeof (m.length())));
    do_read_body();
    //socket_.close();


}