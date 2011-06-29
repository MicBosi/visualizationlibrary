/**************************************************************************************/
/*                                                                                    */
/*  Visualization Library                                                             */
/*  http://www.visualizationlibrary.com                                               */
/*                                                                                    */
/*  Copyright (c) 2005-2010, Michele Bosi                                             */
/*  All rights reserved.                                                              */
/*                                                                                    */
/*  Redistribution and use in source and binary forms, with or without modification,  */
/*  are permitted provided that the following conditions are met:                     */
/*                                                                                    */
/*  - Redistributions of source code must retain the above copyright notice, this     */
/*  list of conditions and the following disclaimer.                                  */
/*                                                                                    */
/*  - Redistributions in binary form must reproduce the above copyright notice, this  */
/*  list of conditions and the following disclaimer in the documentation and/or       */
/*  other materials provided with the distribution.                                   */
/*                                                                                    */
/*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND   */
/*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED     */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE            */
/*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR  */
/*  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    */
/*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      */
/*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON    */
/*  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           */
/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS     */
/*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                      */
/*                                                                                    */
/**************************************************************************************/

#include <vl/DiskDirectory.hpp>
#include <vlWin32/Win32Window.hpp>
#include <vl/Log.hpp>
#include <vl/Say.hpp>
#include "tests.hpp"

using namespace vl;
using namespace vlWin32;

class TestBatteryWin32: public TestBattery
{
public:
  void runGUI(float secs, const vl::String& title, BaseDemo* applet, vl::OpenGLContextFormat format, int x, int y, int width, int height, vl::fvec4 bk_color, vl::vec3 eye, vl::vec3 center)
  {
    /* used to display the application title next to FPS counter */
    applet->setApplicationName(title);

    /* open a console so we can see the applet's output on stdout */
    showWin32Console();

    /* init Visualization Library */
    vl::VisualizationLibrary::init();

    /* create a native Win32 window */
    vl::ref<vlWin32::Win32Window> win32_window = new vlWin32::Win32Window;

    setupApplet(applet, win32_window.get(), secs, bk_color, eye, center);

    /* Used to test OpenGL 3.3 Core Profile
    int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3, 
        // WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    win32_window->setContextAttribs(attribs); */

    /* Initialize the OpenGL context and window properties */
    win32_window->initWin32Window(NULL, NULL, title, format, x, y, width, height );

    /* show the window */
    win32_window->show();

    /* run the Win32 message loop */
    vlWin32::messageLoop();

    /* deallocate the window with all the OpenGL resources before shutting down Visualization Library */
    win32_window = NULL;

    /* shutdown Visualization Library */
    vl::VisualizationLibrary::shutdown();
  }
};
//-----------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE /*hCurrentInst*/, HINSTANCE /*hPreviousInst*/, LPSTR lpszCmdLine, int /*nCmdShow*/)
{
  /* parse command line arguments */
  int   test = 0;
  float secs = 0;
  String cmd = lpszCmdLine;
  std::vector<String> parms;
  cmd.split(' ', parms);
  if (parms.size()>=1)
    test = parms[0].toInt();
  if (parms.size()>=2)
    secs = parms[1].toFloat();

  /* setup the OpenGL context format */
  vl::OpenGLContextFormat format;
  format.setDoubleBuffer(true);
  format.setRGBABits( 8, 8, 8, 0 );
  format.setDepthBufferBits(24);
  format.setStencilBufferBits(8);
  format.setFullscreen(false);
  /*format.setMultisampleSamples(8);
  format.setMultisample(true);*/

  TestBatteryWin32 test_battery;
  test_battery.run(test, secs, format);

  return 0;
}