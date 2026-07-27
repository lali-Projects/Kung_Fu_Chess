#include <iostream>


#include "Application.hpp"

#include <ixwebsocket/IXNetSystem.h>





int main()
{

    //---------------------------------
    // Init network system
    //---------------------------------

    ix::initNetSystem();



    try
    {

        //---------------------------------
        // Create server application
        //---------------------------------

        Application app;



        //---------------------------------
        // Start server
        //---------------------------------

        app.start();



        std::cout
            << "[SERVER MAIN] Running\n";


        std::cout
            << "Press ENTER to stop...\n";


        std::cin.get();



        //---------------------------------
        // Stop server
        //---------------------------------

        app.stop();


    }


    catch(const std::exception& e)
    {

        std::cerr
            << "[SERVER ERROR] "
            << e.what()
            << "\n";


        ix::uninitNetSystem();


        return 1;

    }





    //---------------------------------
    // Shutdown network
    //---------------------------------

    ix::uninitNetSystem();



    return 0;
}