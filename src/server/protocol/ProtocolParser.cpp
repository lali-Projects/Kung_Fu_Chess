#include "ProtocolParser.hpp"


#include "ClickPacket.hpp"



//================================================
// Parse Packet
//================================================

std::optional<ClickCommand>
ProtocolParser::parse(
    const ClientPacket& packet)
{
    switch(packet.getType())
    {

        case PacketType::CLICK:
        {
            return parseClick(packet);
        }


        default:
        {
            return std::nullopt;
        }

    }
}



//================================================
// Parse Click
//================================================

std::optional<ClickCommand>
ProtocolParser::parseClick(
    const ClientPacket& packet)
{

    const ClickPacket* clickPacket =
        dynamic_cast<const ClickPacket*>(&packet);



    if(clickPacket == nullptr)
    {
        return std::nullopt;
    }



    return ClickCommand(
        clickPacket->getPosition());
}