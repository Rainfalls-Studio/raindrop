#ifndef __RAINDROP_PCH_HPP__
#define __RAINDROP_PCH_HPP__

#if defined(WIN32) || defined(WIN64)
	#include <windows.h>
#endif

// data structure
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <array>
#include <deque>
#include <bitset>
#include <string>

// thread
#include <mutex>
#include <thread>
#include <barrier>
#include <condition_variable>

// other
#include <math.h>
#include <stdint.h>
#include <glm/glm.hpp>
#include <functional>
#include <utility>
#include <limits>
#include <memory>
#include <new>
#include <chrono>
#include <algorithm>

// IO
#include <fstream>
#include <iostream>

#endif