#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>

// 16:9 aspect ratio
#define WINDOW_HEIGHT sf::VideoMode::getDesktopMode().height / 4 * 3
#define WINDOW_WIDTH WINDOW_HEIGHT / 9 * 16

#ifdef _DEBUG
#define DEBUG_INFO(msg) std::cout << msg << "\n"
#define DEBUG_ERROR(msg) std::cerr << msg << "\n"
#else // strip the debug messages for release build 
#define DEBUG_INFO(msg)
#define DEBUG_ERROR(msg)
#endif 
#endif
