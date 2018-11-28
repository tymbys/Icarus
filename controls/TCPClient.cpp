#include "TCPClient.h"

using boost::asio::ip::tcp;
using namespace std;

template <typename SyncReadStream, typename MutableBufferSequence>
size_t readWithTimeout(SyncReadStream& s, const MutableBufferSequence& buffers, const boost::asio::deadline_timer::duration_type& expiry_time) {
    size_t len = 0;
    boost::optional<boost::system::error_code> timer_result;
    boost::asio::deadline_timer timer(s.get_io_service());
    timer.expires_from_now(expiry_time);
    timer.async_wait([&timer_result] (const boost::system::error_code & error) {
        timer_result.reset(error); });

    boost::optional<boost::system::error_code> read_result;


    boost::asio::async_read(s, buffers, [&read_result, &len] (const boost::system::error_code& error, size_t N) {
        len = N;

        printf("N: %d\n", N);
        read_result.reset(error); });


    boost::system::error_code ec;
    s.get_io_service().reset();
    s.get_io_service().poll_one(ec);

    while (s.get_io_service().run_one()) {
        cout << "1";
        if (read_result) {
            cout << "timer.cancel" << endl;
            timer.cancel();
            break;

        } else if (timer_result) {
            cout << "s.cancel" << endl;
            s.cancel();
            //s.close();
            break;

        }
    }




    cout << "2" << endl;
    return len;

    //    if (*read_result)
    //        throw boost::system::system_error(*read_result);
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
    try {

        boost::asio::async_connect(socket_, endpoint_iterator,
                [this](boost::system::error_code ec, tcp::resolver::iterator) {
                    if (!ec) {
                        //do_read_header();
                        //do_read_body();
                        do_write();
                    }
                });
    } catch (...) {
        cout << "unexpected exception" << endl;
    }
}

void TCPClient::do_read_body() {

    //    boost::system::error_code r_ec;
    //    std::size_t bytes_transferred = boost::asio::read(socket_, boost::asio::buffer(&read_msg_, sizeof (Beacon)), r_ec);
    //    
    //    if (r_ec) {
    //        
    //    }

    //boost::asio::const_buffers_1 b(&read_msg_, sizeof (read_msg_));


    boost::system::error_code read_result;
    read_len = socket_.read_some(
            boost::asio::buffer(&read_msg_, sizeof (read_msg_)),
            read_result);

    //read_len = readWithTimeout(socket_, boost::asio::buffer(&read_msg_, sizeof (read_msg_)), boost::posix_time::seconds(1));


    printf("read_len: %d\n", read_len);
    //    for (int i = 0; i < read_len; i++) {
    //        printf("0x%02hhX ", read_msg_[i]);
    //    }

    socket_.close();


    //        Semaphore r_sem;
    //        boost::system::error_code r_ec;
    //    
    //        boost::asio::async_read(socket_,
    //                boost::asio::buffer(&read_msg_, sizeof (read_msg_)),
    //                [this/*, &r_ec, &r_sem*/](boost::system::error_code ec, std::size_t /*length*/) {
    //    
    //                    //r_ec = ec;
    //                    //r_sem.notify();
    //    
    //                    if (!ec) {
    //                        std::cout << std::hex;
    //                        //std::cout.write(reinterpret_cast<char  *>(read_msg_.data), 1);
    //                        //read_msg_[1] = 0;
    //                        //read_msg_[2] = 0;
    //                        //read_msg_[3] = 0;
    //                        std::cout << (int) read_msg_[0];
    //                        std::cout << "\n";
    //    
    //                        //                    switch (read_msg_.b1) {
    //                        //
    //                        //                    }
    //    
    //    
    //    
    //                        //do_read_body();
    //                    } else {
    //                        //socket_.close();
    //                    }
    //                    socket_.close();
    //    
    //                });

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
    //do_read_body();
    socket_.close();


}

//tcp::socket TCPClient::get(){
//    return socket_;
//}

size_t TCPClient::getReadData(char *msg) {

    memcpy(msg, read_msg_, read_len);

    return read_len;
}

size_t TCPClient::getReadLen() {
    return read_len;
}

size_t TCPClient::getReadBodyData(char *msg) {

    if (read_len >= sizeof (M_Header)) {
        memcpy(msg, read_msg_ + sizeof (M_Header), read_len - sizeof (M_Header));

        return read_len - sizeof (M_Header);
    }

    return 0;
}

size_t TCPClient::getReadBodyLen() {
    if (read_len <= 0)return 0;
    else if (read_len >= sizeof (M_Header)) return read_len - sizeof (M_Header);
}


