#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <mutex>

// 16:9 aspect ratio
#define WINDOW_HEIGHT sf::VideoMode::getDesktopMode().height / 4 * 3
#define WINDOW_WIDTH WINDOW_HEIGHT
#define RESOLUTION sf::Vector2i{ 1080, 1080 }

//#ifdef _DEBUG
static std::mutex outputMutex;
#define DEBUG_INFO(msg) outputMutex.lock(); std::cout << msg << "\n"; outputMutex.unlock();
#define DEBUG_ERROR(msg) std::cerr << msg << "\n"
//#else // strip the debug messages for release build 
//#define DEBUG_INFO(msg)
//#define DEBUG_ERROR(msg)
//#endif 
#endif
