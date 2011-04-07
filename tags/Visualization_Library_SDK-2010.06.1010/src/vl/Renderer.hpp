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

#ifndef Renderer_INCLUDE_ONCE
#define Renderer_INCLUDE_ONCE

#include <vl/Camera.hpp>
#include <vl/Renderable.hpp>
#include <vl/Shader.hpp>
#include <vl/Effect.hpp>
#include <vl/OpenGLContext.hpp>
#include <vector>
#include <map>
#include <set>

namespace vl
{
  class RenderQueue;

  /**
   * The Renderer class executes the actual rendering on the given RenderQueue.
   *
   * \sa Rendering
  */
  class Renderer: public Object
  {
  public:
    virtual const char* className() { return "Renderer"; }
    Renderer();
    virtual ~Renderer() {}
    virtual void render(const RenderQueue* render_queue, Camera* camera);

    void setCollectStatistics(bool on) { mCollectStatistics = on; }
    bool collectStatistics() const { return mCollectStatistics; }

    size_t renderedRenderablesCount() const { return mRenderedRenderableCount; }
    size_t renderedTrianglesCount() const { return mRenderedTriangleCount; }
    size_t renderedLinesCount() const { return mRenderedLineCount; }
    size_t renderedPointsCount() const { return mRenderedPointCount; }

    void applyEnables( const EnableSet* prev, const EnableSet* cur );
    void applyRenderStates( const RenderStateSet* prev, const RenderStateSet* cur, const Camera* camera );

    const OpenGLContext* openglContext() const { return mOpenGLContext; }
    void setOpenGLContext(const OpenGLContext* ogl_context) { mOpenGLContext = ogl_context; }

    void resetEnables();
    void resetRenderStates();

    /** Whether occlusion culling is enabled or not.
    See also \ref pagGuideOcclusionCulling "OpenGL-Accelerated Occlusion Culling Tutorial".
    */
    void setOcclusionCullingEnabled(bool enabled) { mOcclusionCullingEnabled = enabled; }
    /** Whether occlusion culling is enabled or not.
    See also \ref pagGuideOcclusionCulling "OpenGL-Accelerated Occlusion Culling Tutorial".
    */
    bool occlusionCullingEnabled() const { return mOcclusionCullingEnabled; }

    /** The number of pixels visible for an actor to be considered occluded (default = 0) */
    void setOcclusionThreshold(int threshold) { mOcclusionThreshold = threshold; }
    /** The number of pixels visible for an actor to be considered occluded (default = 0) */
    int occlusionThreshold() const { return mOcclusionThreshold; }

  protected:
    bool mCollectStatistics;
    size_t mRenderedRenderableCount;
    size_t mRenderedTriangleCount;
    size_t mRenderedLineCount;
    size_t mRenderedPointCount;

    // table

    int mEnableTable[EN_EnableCount];
    int mRenderStateTable[RS_COUNT];

    // current state

    bool mCurrentEnable[EN_EnableCount];
    const RenderState* mCurrentRenderState[RS_COUNT];
    std::map< String, Uniform* > mCurrentUniform;

    // default render states
    ref<RenderState> mDefaultRenderStates[RS_COUNT];
    void setupDefaultRenderStates();

    const OpenGLContext* mOpenGLContext;

    bool mOcclusionCullingEnabled;
    // shader used to render proxy shapes during occlusion cull tests
    ref<Shader> mOcclusionShader;
    int mOcclusionThreshold;
    unsigned mOcclusionQueryTick;
    unsigned mOcclusionQueryTickPrev;
  };
  //------------------------------------------------------------------------------
}

#endif