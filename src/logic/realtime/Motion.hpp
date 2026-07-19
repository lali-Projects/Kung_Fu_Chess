class Motion
{
public:
    std::shared_ptr<Piece> movingPiece;
    Position source;
    Position destination;

    int startTime;
    int finishTime;

    Motion(
        std::shared_ptr<Piece> piece,
        Position src,
        Position dst,
        int startTime,
        int travelTime)
        :
        movingPiece(piece),
        source(src),
        destination(dst),
        startTime(startTime),
        finishTime(startTime + travelTime)
    {
    }

    bool hasArrived(int currentTime) const
    {
        return currentTime >= finishTime;
    }
};