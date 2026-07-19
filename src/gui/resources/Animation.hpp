#pragma once

#include <string>
#include "Sprite.hpp"

/**
 * @class Animation
 * @brief Represents a single animation sequence.
 *
 * This class stores the data required for animation playback.
 *
 * Responsibilities:
 * - Holding the Sprite data.
 * - Storing animation parameters.
 *
 * This class does not handle file loading or rendering.
 */
class Animation
{
private:
    Sprite sprite;
    int framesPerSecond;
    bool loop;
    std::string nextState;
    double speedMetersPerSecond;

public:
    /**
     * @brief Constructs a new Animation object.
     */
    Animation();

    //---------------------------------
    // Sprite
    //---------------------------------
    /**
     * @brief Sets the sprite associated with this animation.
     * @param sprite The sprite to set.
     */
    void setSprite(const Sprite& sprite);

    /**
     * @brief Gets the sprite associated with this animation.
     * @return The constant reference to the sprite.
     */
    const Sprite& getSprite() const;

    //---------------------------------
    // FPS
    //---------------------------------
    /**
     * @brief Sets the animation speed in frames per second.
     * @param fps The frames per second value.
     */
    void setFramesPerSecond(int fps);

    /**
     * @brief Gets the animation speed in frames per second.
     * @return The frames per second value.
     */
    int getFramesPerSecond() const;

    //---------------------------------
    // Loop
    //---------------------------------
    /**
     * @brief Sets the loop behavior.
     * @param loop True to loop, false to play once.
     */
    void setLoop(bool loop);

    /**
     * @brief Checks if the animation is set to loop.
     * @return True if looping, false otherwise.
     */
    bool isLoop() const;

    //---------------------------------
    // Next state
    //---------------------------------
    /**
     * @brief Sets the state that should follow this animation.
     * @param state The identifier for the next state.
     */
    void setNextState(const std::string& state);

    /**
     * @brief Gets the next state identifier.
     * @return The state name.
     */
    const std::string& getNextState() const;

    //---------------------------------
    // Speed
    //---------------------------------
    /**
     * @brief Sets the animation playback speed.
     * @param speed Speed in meters per second.
     */
    void setSpeed(double speed);

    /**
     * @brief Gets the animation playback speed.
     * @return Speed in meters per second.
     */
    double getSpeed() const;

    //---------------------------------
    // Convenience
    //---------------------------------
    /**
     * @brief Checks if the animation contains no valid data.
     * @return True if empty, false otherwise.
     */
    bool empty() const;
};