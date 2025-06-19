#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "ICS_Color.h" // For ICS_Color
#include <string>      // For std::string
#include <utility>     // For std::pair<>

// Make a Vertex "class" to reduce verbosity
#define Vertex std::pair<double, double>

// How many pixels wide is a block
// Defines the size of the game window
const double PIXELS_PER_BLOCK = 60;

const int SCREEN_BLOCKS_WIDTH = 22;  // Width of the game window in blocks
const int SCREEN_BLOCKS_HEIGHT = 12; // Height of the game window in blocks

const int WINDOW_WIDTH = PIXELS_PER_BLOCK * SCREEN_BLOCKS_WIDTH;   // Width of the game window in pixels
const int WINDOW_HEIGHT = PIXELS_PER_BLOCK * SCREEN_BLOCKS_HEIGHT; // Height of the game window in pixels

// Number of frames that a jump buffers for
// Give user a 100ms buffer for jumping (6/60fps = 100ms)
const int JUMP_FRAMES = 6;

// Physics constants

const double GRAVITY = 94.0408163265;         // Blocks per seconds squared
const double JUMP_VELOCITY = -20.37551020410; // Blocks per seconds
const double SCROLL_SPEED = 10.3761348898;    // Blocks per seconds
const double TERMINAL_VELOCITY = 2.6;         // Blocks per seconds
const double BACKGROUND_SCROLL_SPEED = 0.8;   // Blocks per seconds

// Number of seconds it takes to move one full block
const double SECONDS_PER_BLOCK = 1 / SCROLL_SPEED;

const double GRAVITY_PIXELS = GRAVITY * PIXELS_PER_BLOCK;                                 // Pixels per seconds
const double JUMP_VELOCITY_PIXELS = JUMP_VELOCITY * PIXELS_PER_BLOCK;                     // Pixels per seconds
const double SCROLL_SPEED_PIXELS = SCROLL_SPEED * PIXELS_PER_BLOCK;                       // Pixels per seconds
const double TERMINAL_VELOCITY_PIXELS = TERMINAL_VELOCITY * PIXELS_PER_BLOCK;             // Pixels per seconds
const double BACKGROUND_SCROLL_SPEED_PIXELS = BACKGROUND_SCROLL_SPEED * PIXELS_PER_BLOCK; // Pixels per seconds

// Time screen pauses after player dies
const double DEATH_PAUSE_LENGTH = 0.2;

const double SPIKE_HITBOX_OFFSET_Y = PIXELS_PER_BLOCK / 4; // Shift the spike hitbox up
const double SPIKE_HITBOX_WIDTH = 16;                      // In pixels
const double SPIKE_HITBOX_HEIGHT = 24;                     // In pixels

const double END_MENU_WIDTH = 1102;                                           // Size of actual image in pixels
const double END_MENU_HEIGHT = 838;                                           // Size of actual image in pixels
const double END_MENU_RATIO = END_MENU_WIDTH / END_MENU_HEIGHT;               // Used to scale to window size
const double END_MENU_WIDTH_PIXELS = WINDOW_WIDTH / 3.0 * 2.0;                // In pixels
const double END_MENU_HEIGHT_PIXELS = END_MENU_WIDTH_PIXELS / END_MENU_RATIO; // In pixels

const Vertex PLAYER_STARTING_POS(PIXELS_PER_BLOCK * 8, PIXELS_PER_BLOCK * 6);

// Sprite File names

const std::string BLOCK_FILE_NAME = "data/block.bmp";
const std::string PLATFORM_FILE_NAME = "data/platform.bmp";
const std::string PLAYER_IMAGE_FILE = "data/player.bmp";
const std::string SPIKE_FILE_NAME = "data/spike.png";
const std::string LEVEL_COMPLETE_FILE_NAME = "data/level_complete.png";

const ICS_Color END_MENU_TEXT_COLOUR = ICS_Color(253, 208, 48);

#endif //! CONSTANTS_H
