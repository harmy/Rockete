#ifndef OPENGL_H
#define OPENGL_H

#include <Rocket/Core/Platform.h>

#if defined ROCKET_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#elif defined ROCKET_PLATFORM_MACOSX
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#elif defined ROCKET_PLATFORM_UNIX
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#endif

#endif
