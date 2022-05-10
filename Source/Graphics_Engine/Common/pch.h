#pragma once

// Std includes
#include <string>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

// GL includes
#include <Universal/3rdparty/include/gl/glew.h>
#include <Universal/3rdparty/include/GLFW/glfw3.h>

#include <Universal/3rdparty/include/glm/glm.hpp>
#include <Universal/3rdparty/include/glm/gtc/matrix_transform.hpp>
#include <Universal/3rdparty/include/glm/gtc/type_ptr.hpp>
#include <Universal/3rdparty/include/glm/gtx/transform.hpp>

#define BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
