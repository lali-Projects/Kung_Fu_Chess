#include <iostream>


#include <ixwebsocket/IXNetSystem.h>


#include "ClientSocket.hpp"

#include "AuthenticationClient.hpp"

#include "ConsoleClient.hpp"





int main()
{


    //---------------------------------
    // Init network
    //---------------------------------

    ix::initNetSystem();





    try
    {


        //---------------------------------
        // Socket
        //---------------------------------

        ClientSocket socket;




        //---------------------------------
        // Connect server
        //---------------------------------

        if(!socket.connect(
            "ws://127.0.0.1:8080"))
        {

            std::cerr
                << "[CLIENT] Connection failed\n";


            ix::uninitNetSystem();


            return 1;
        }





        //---------------------------------
        // Authentication
        //---------------------------------

        AuthenticationClient auth(
            socket);







        //---------------------------------
        // Console client
        //---------------------------------

        ConsoleClient console(
            socket,
            auth);





        console.run();





        //---------------------------------
        // Disconnect
        //---------------------------------

        socket.disconnect();


    }


    catch(const std::exception& e)
    {

        std::cerr
            << "[CLIENT ERROR] "
            << e.what()
            << "\n";


        ix::uninitNetSystem();


        return 1;

    }






    ix::uninitNetSystem();


    return 0;
}