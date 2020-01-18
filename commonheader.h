/*******************************************************************
 * CommonHeader:  A header file to reduce repetition of #define
 * and #include statements.
 * Edward C. Eberle <eberdeed@eberdeed.net>
 * January 2020 San Diego, California USA
 * ****************************************************************/

/** \class CommonHeader
 * A header file to reduce repetition of #define
 * and #include statements.
 */
#ifndef COMMONHEADER_H
#define COMMONHEADER_H

//! #defines to make changing value simple.
#define NUM_INSTANCES 30
#define NUM_IMAGES 16
#define NUM_VERTICES 36

//! GLEW The OpenGL library manager
#define GLEW_STATIC
#include <glew.h>

//! GLM The OpenGL math library
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <common.hpp>
#include <vec3.hpp>
#include <geometric.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/matrix_interpolation.hpp>
#include <gtc/matrix_access.hpp> 
#include <gtx/euler_angles.hpp>

//! ClanLib
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

//! FreeImagePlus
#include <FreeImagePlus.h>

//! CPP Library
#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>

//! Boost
#include <filesystem.hpp>

using namespace std;
using namespace glm;
using namespace boost::filesystem;

//! Structures to manage the location and data associated
//! with each object.
struct PosOrient{
    vec3 locon;
    double dist;
    float index[6];
    float angles;
    vec3 xaxis;
    vec3 yaxis;
};

//! Data for the buffer going to the shader.
struct InstData{
    vec4 instIndex1[NUM_IMAGES * NUM_INSTANCES];
    vec2 instIndex2[NUM_IMAGES * NUM_INSTANCES];
    vec2 distance[NUM_IMAGES * NUM_INSTANCES];
    mat4 instModel[NUM_IMAGES * NUM_INSTANCES];
};

//! Structure for the uniform holding the lighting data.
struct Lights {
    vec3 lightPos;
    vec4 lightColor;
};    

#endif //! COMMONHEADER_H
