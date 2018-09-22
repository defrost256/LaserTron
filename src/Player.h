#pragma once

#include "ofColor.h"
#include "ofThread.h"
#include "GameMap.h"
#include "Bike.h"
#ifdef PI
#include "wiringPi.h"
#endif

class GameMap;

/**
 * Class representing a player.
 * Binds the bike, and its callers, and handles input
 */
class Player : public ofThread
{
public:
#ifdef PI
	/**
	 * __PI__
	 * Creates a new player with color and input pins.
	 * Also sets the GPIO pins up.
	 * @param color The color of the wall the player's bike draws
	 * @param leftPin The GPIO pin on which to listen to turn left input
	 * @param rightPin The GPIO pin on which to listen to turn right input
	 */
	Player(ofColor color, int leftPin, int rightPin);
#else
	/**
	 * __WIN__
	 * Creates a new player with color and input controls
	 * @param color The color of the wall the player's bike draws
	 * @param intputString The characters to listen for. (in order: UP, LEFT, DOWN, RIGHT, RESPAWN)
	 */
	Player(ofColor color, string intputString);
#endif
	/**
	 * Copy constructor for players
	 * @param p the player to copy
	 */
	Player(const Player& p);
	~Player();

	ofColor Color;	///< The color of the wall the player's bike draws
	int BikeIdx;	///< The index of the bike belonging to this player
#ifdef PI
	int LeftPin;	///< The GPIO pin corresponding to left turn __PI__
	int RightPin;	///< The GPIO pin corresponding to right turn __PI__
#else
	string InputString;		///< The characters corresponding to turn directions (in order: UP, LEFT, DOWN, RIGHT, RESPAWN) __WIN__
#endif
	Bike* bike;		///< Pointer to the bike belonging to the player
	GameMap* Map;	///< Pointer to the map.

#ifdef PI
	/**
	 * __PI__
	 * Sends the last input to the bike.
	 * Mutex
	 */
	void ExecuteInput();
#else
	/**
	 * Updates the last input recieved. (Called by GameThread)
	 * @param input The new input's idx (0-3: UP, LEFT, DOWN, RIGHT, RESPAWN)
	 */
	void AddInput(int input);
#endif

private:
#ifdef PI
	bool leftLast = false;	///< True if the state of the left input was PRESSED
	bool rightLast = false;	///< True if the state of the right input was PRESSED
	bool hasInput;			///< True if there was a new input
#endif
	int LastInput = -1;		///< The idx of the last keyboard input

	/**
	 * Checks the input periferia, and updates the input states of the player accordingly.
	 * Mutex
	 */
	void ProcessInput();
	/**
	 * Loop for processing inputs, runs on a separate thread.
	 */
	void threadedFunction() override;
};
