#ifndef    PRE_COMPILED_HPP
#define    PRE_COMPILED_HPP


/* Default C++ includes */
#include <cstdint>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <map>
#include <unordered_set>
#include <tuple>
#include <sstream>
#include <iomanip>
#include <set>
#include <fstream>
#include <numeric>
#include <regex>


/* Os Specific */
// Windows System
#ifdef __WIN32__
#include <winsock2.h>
#include <Ws2tcpip.h>
// Unix System
#else
#include <sys/time.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif


/* OpenGL GLEW/GLFW/GLM */
#define GLEW_STATIC
#include <GL/glew.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/* Eigen */
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/SparseCore>
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/QR>


/* Freetype */
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>


#endif /* PRE_COMPILED_HPP */

