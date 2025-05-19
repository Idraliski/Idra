#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <sstream>
#include <fstream>
#include <iterator>

#include <filesystem>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Log.h"

#ifdef _WIN32
	#include <Windows.h>
#endif
