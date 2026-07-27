#include "NetworkMessage.hpp"


#include <sstream>





NetworkMessage::NetworkMessage()
:
m_type(MessageType::UNKNOWN)
{
}






NetworkMessage::NetworkMessage(
    MessageType type,
    const std::string& payload)
:
m_type(type),
m_payload(payload)
{
}






NetworkMessage::NetworkMessage(
    MessageType type,
    std::string&& payload)
:
m_type(type),
m_payload(std::move(payload))
{
}







void NetworkMessage::setType(
    MessageType type)
{
    m_type = type;
}






MessageType NetworkMessage::getType() const
{
    return m_type;
}







std::string NetworkMessage::typeName() const
{

    switch(m_type)
    {

        case MessageType::GAME_STATE:
            return "GAME_STATE";


        case MessageType::COMMAND:
            return "COMMAND";


        case MessageType::COMMAND_RESULT:
            return "COMMAND_RESULT";


        case MessageType::SYSTEM_ERROR:
            return "SYSTEM_ERROR";


        case MessageType::DISCONNECT:
            return "DISCONNECT";


        case MessageType::HEARTBEAT:
            return "HEARTBEAT";


        default:
            return "UNKNOWN";
    }

}









MessageType NetworkMessage::typeFromString(
    const std::string& name)
{

    if(name=="GAME_STATE")
        return MessageType::GAME_STATE;


    if(name=="COMMAND")
        return MessageType::COMMAND;


    if(name=="COMMAND_RESULT")
        return MessageType::COMMAND_RESULT;


    if(name=="SYSTEM_ERROR")
        return MessageType::SYSTEM_ERROR;


    if(name=="DISCONNECT")
        return MessageType::DISCONNECT;


    if(name=="HEARTBEAT")
        return MessageType::HEARTBEAT;



    return MessageType::UNKNOWN;

}









void NetworkMessage::setPayload(
    const std::string& payload)
{
    m_payload = payload;
}






void NetworkMessage::setPayload(
    std::string&& payload)
{
    m_payload = std::move(payload);
}







const std::string&
NetworkMessage::getPayload() const
{
    return m_payload;
}









//================================================
// Serialize
//================================================

std::string NetworkMessage::serialize() const
{

    return
        typeName()
        +
        "|"
        +
        m_payload;

}









//================================================
// Deserialize
//================================================

std::optional<NetworkMessage>
NetworkMessage::deserialize(
    const std::string& data)
{

    auto separator =
        data.find('|');



    if(separator == std::string::npos)
    {
        return std::nullopt;
    }





    std::string type =
        data.substr(
            0,
            separator);




    std::string payload =
        data.substr(
            separator + 1);




    MessageType messageType =
        typeFromString(type);




    if(messageType ==
       MessageType::UNKNOWN)
    {
        return std::nullopt;
    }





    return NetworkMessage(
        messageType,
        payload);

}









bool NetworkMessage::empty() const
{
    return m_payload.empty();
}








bool NetworkMessage::valid() const
{

    if(m_type ==
       MessageType::UNKNOWN)
    {
        return false;
    }



    if(m_type ==
       MessageType::DISCONNECT ||
       m_type ==
       MessageType::HEARTBEAT)
    {
        return true;
    }



    return !m_payload.empty();

}









void NetworkMessage::clear()
{

    m_type =
        MessageType::UNKNOWN;


    m_payload.clear();

}









bool NetworkMessage::isCommand() const
{
    return m_type ==
        MessageType::COMMAND;
}







bool NetworkMessage::isCommandResult() const
{
    return m_type ==
        MessageType::COMMAND_RESULT;
}








bool NetworkMessage::isGameState() const
{
    return m_type ==
        MessageType::GAME_STATE;
}








bool NetworkMessage::isError() const
{
    return m_type ==
        MessageType::SYSTEM_ERROR;
}








bool NetworkMessage::isDisconnect() const
{
    return m_type ==
        MessageType::DISCONNECT;
}








bool NetworkMessage::isHeartbeat() const
{
    return m_type ==
        MessageType::HEARTBEAT;
}