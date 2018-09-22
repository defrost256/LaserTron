#pragma once

#include "ofThread.h"
#include "ofxEtherdream.h"
#include "ofxIldaFrame.h"
#include "CountdownPoints.h"
#include "GameMap.h"
#include "SoundThread.h"
#include <queue>

using namespace std;

/**
 * Class handling the main loop of the game.
 * Also responsible for sending data to the laser, and handling keyboard inputs.
 */
class GameThread : public ofThread
{
public:
	/**
	 * Creates a new GameThread
	 * Creates the frame buffer, and initializes the laser
	 * @param ed The etherdream device to send data to
	 */
	GameThread(ofxEtherdream* ed);
	~GameThread();

	/**
	 * The main loop of the game, functionality depends on the game state.
	 * GT_Stopped: Checks periodically if the game is started
	 * GT_Running: Updates the game
	 * GT_Countdown: Initiates countdown sequence
	 */
	virtual void threadedFunction() override;

	/**
	 * Sends the last frame to the laser
	 */
	void SendData();

#ifndef PI
	/**
	 * Adds a keyboard key to the input queue
	 * @param key The key to add
	 */
	void QueueInput(int key);
#endif
	/**
	 * Draws one frame on the display (if it's enabled)
	 */
	void Draw();
	
	/**
	 * Restarts the game, the map, and the Etherdream, and initiates the countdown sequence
	 */
	void Restart();
private:

	/**
	 * The state of the game
	 * @dot
	 * digraph GT_State {
	 * 		GT_Stopped -> GT_Countdown [xlabel = "Restart()"]
	 * 		GT_Countdown -> GT_Running [label="Countdown\nreached 0", labelangle=180, labeldistance=20]
	 * 		GT_Running -> GT_Stopped [xlabel="No more bikes\nalive"]
	 * }
	 * @enddot
	 */
	enum GT_State{ GT_Stopped, GT_Running, GT_Countdown };

	long Tick = 0;									///< The current frame processed (incremented with each update)

	vector<ofxIlda::Frame> frames;					///< The frame buffer
	int frameBufferSize = 2;						///< The size of the frame buffer
	int writeBufferIdx = 0, readBufferIdx = -1;		///< The indexes for the written and read buffers (-1 for not initialized)
#ifdef PI
	int targetPoints = 500;							///< The number of points a frame should ideally contain \n(the program tries to reduce the number of points to match this)
#else
	queue<int> InputQueue;							///< Queue containing the keyboard inputs recieved this frame
#endif
	ofxEtherdream* ED;								///< Pointer to the etherDream library
	SoundThread Sound;								///< The thread responsible for sound processing

	GameMap* Map;									///< Pointer to the playfield
	vector<Player*> Players;						///< List of players playing the game
	GT_State state;									///< The current state of the game
	int countdown;									///< The state of the countdown 0: GO, 1-3: numbers
	CountdownPoints cdp;							///< The point buffer used for countdown display

#ifndef PI
	int SendCount;									///< UNUSED
	/**
	 * Checks keyboard inputs, and sends them to the respective player
	 */
	void ProcessInputs();
#endif
	/**
	 * Handles the order of updates for the game components, and generates a frame.
	 */
	void Update();
	/**
	 * Runs the countdown sequence, then sets the game state to running
	 */
	void Countdown();
	/**
	 * Draws the countdown's current state to the current frame
	 * @param c The character to draw (1-3: number, G: GO)
	 */
	void DrawFont(char c);
};
