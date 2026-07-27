#pragma once


#include <string>
#include <utility>
#include <optional>



enum class MessageType
{

    UNKNOWN,

    GAME_STATE,

    COMMAND,

    COMMAND_RESULT,

    SYSTEM_ERROR,

    DISCONNECT,

    HEARTBEAT

};






class NetworkMessage
{

public:


    NetworkMessage();



    NetworkMessage(
        MessageType type,
        const std::string& payload);



    NetworkMessage(
        MessageType type,
        std::string&& payload);





public:


    //---------------------------------
    // Type
    //---------------------------------

    void setType(
        MessageType type);



    MessageType getType() const;



    std::string typeName() const;



    static MessageType
    typeFromString(
        const std::string& name);





public:


    //---------------------------------
    // Payload
    //---------------------------------

    void setPayload(
        const std::string& payload);



    void setPayload(
        std::string&& payload);



    const std::string&
    getPayload() const;






public:


    //---------------------------------
    // Serialization
    //---------------------------------


    std::string serialize() const;



    static std::optional<NetworkMessage>
    deserialize(
        const std::string& data);






public:


    //---------------------------------
    // State
    //---------------------------------

    bool empty() const;



    bool valid() const;



    void clear();





public:


    //---------------------------------
    // Helpers
    //---------------------------------

    bool isCommand() const;



    bool isCommandResult() const;



    bool isGameState() const;



    bool isError() const;



    bool isDisconnect() const;



    bool isHeartbeat() const;





private:


    MessageType m_type{
        MessageType::UNKNOWN
    };


    std::string m_payload;

};