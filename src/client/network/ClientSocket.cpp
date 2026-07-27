#include "ClientSocket.hpp"

#include <utility>
#include <chrono>
#include <iostream>



//================================================
// Constructor
//================================================

ClientSocket::ClientSocket()
{
}




//================================================
// Destructor
//================================================

ClientSocket::~ClientSocket()
{
    disconnect();
}




//================================================
// Connect
//================================================

bool ClientSocket::connect(
    const std::string& url)
{

    {
        std::lock_guard<std::mutex> lock(
            m_mutex);


        if(m_connected)
        {
            return true;
        }


        m_failed = false;
    }



    m_socket.setUrl(url);



    m_socket.setOnMessageCallback(
        [this]
        (
            const ix::WebSocketMessagePtr& msg
        )
        {

            switch(msg->type)
            {

            case ix::WebSocketMessageType::Open:
            {

                std::cout
                    << "[CLIENT] WebSocket connected\n";


                {
                    std::lock_guard<std::mutex> lock(
                        m_mutex);


                    m_connected = true;
                }


                m_connectionCondition.notify_all();


                break;
            }



            case ix::WebSocketMessageType::Close:
            {

                std::cout
                    << "[CLIENT] WebSocket closed\n";


                {
                    std::lock_guard<std::mutex> lock(
                        m_mutex);


                    m_connected = false;
                }


                break;
            }




            case ix::WebSocketMessageType::Error:
            {

                std::cout
                    << "[CLIENT] WebSocket error: "
                    << msg->errorInfo.reason
                    << "\n";


                {
                    std::lock_guard<std::mutex> lock(
                        m_mutex);


                    m_failed = true;
                }


                m_connectionCondition.notify_all();


                break;
            }




            case ix::WebSocketMessageType::Message:
            {

                MessageHandler handler;


                {
                    std::lock_guard<std::mutex> lock(
                        m_mutex);


                    handler =
                        m_messageHandler;
                }



                if(handler)
                {
                    handler(
                        msg->str);
                }


                break;
            }



            default:
                break;

            }

        });




    /*
        Start connection.
        This launches internal ixwebsocket thread.
    */

    m_socket.start();




    /*
        Wait for handshake.
    */

    std::unique_lock<std::mutex> lock(
        m_mutex);



    bool result =
        m_connectionCondition.wait_for(
            lock,
            std::chrono::seconds(5),
            [this]()
            {
                return
                    m_connected ||
                    m_failed;
            });



    if(!result)
    {

        std::cout
            << "[CLIENT] Connection timeout\n";


        return false;
    }



    return m_connected;
}






//================================================
// Disconnect
//================================================

void ClientSocket::disconnect()
{

    {
        std::lock_guard<std::mutex> lock(
            m_mutex);



        if(!m_connected)
        {
            return;
        }


        m_connected=false;
    }



    m_socket.stop();

}






//================================================
// Is Connected
//================================================

bool ClientSocket::isConnected() const
{

    std::lock_guard<std::mutex> lock(
        m_mutex);


    return m_connected;
}






//================================================
// Send
//================================================

bool ClientSocket::send(
    const std::string& message)
{

    {
        std::lock_guard<std::mutex> lock(
            m_mutex);



        if(!m_connected)
        {
            return false;
        }
    }




    auto result =
        m_socket.send(
            message);



    return result.success;
}







//================================================
// Message Handler
//================================================

void ClientSocket::setMessageHandler(
    MessageHandler handler)
{

    std::lock_guard<std::mutex> lock(
        m_mutex);



    m_messageHandler =
        std::move(handler);

}