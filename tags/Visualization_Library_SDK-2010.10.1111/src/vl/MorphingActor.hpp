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

#ifndef MorphingActor_INCLUDE_ONCE
#define MorphingActor_INCLUDE_ONCE

#include <vl/Actor.hpp>
#include <vl/Geometry.hpp>
#include <vl/ResourceDatabase.hpp>

namespace vl
{
  /**
   * The MorphingCallback class is used by MorphingActor to implement the morphing animation on the GPU
   */
  class MorphingCallback: public ActorRenderingCallback
  {
  public:
    virtual const char* className() { return "MorphingCallback"; }

    virtual void operator()(const Camera* cam, Actor* actor, Renderable* renderable, const Shader*, int pass);
  };

  //-----------------------------------------------------------------------------
  // MorphingActor
  //-----------------------------------------------------------------------------
  /**
   * The MorphingActor class animates a Geometry by interpolating its position and normal array.
  */
  class MorphingActor: public Actor
  {
    friend class MorphingCallback;
  public:
    virtual const char* className() { return "MorphingActor"; }

    MorphingActor();

    /** Initializes a MorphingActor from a ResourceDatabase.
     *
     * The given ResourceDatabase must contain a Geometry with all the desired DrawCalls and vertex attributes setup
     * with the exception of the vertex position array and vertex normal array since they are automatically generated
     * in realtime time during the rendering from the vertex and normal frames specified below.
     * In addition ResourceDatabase must contain:
     * - N ArrayFVec3 objects named 'vertex_frame' containing the vertex position for every frame
     * - N ArrayFVec3 objects named 'normal_frame' containing the vertex normal for every frame
     * where N is the number of animation frames.
    */
    void init(ResourceDatabase* res_db);

    void blendFrames(int a, int b, float t);

    virtual void update(int lod, Camera*, Real delta_t);

    void setAnimation(int start, int end, float period);

    void startAnimation(Real time = -1);

    void stopAnimation();

    void initFrom(MorphingActor* morph_act);

    void resetGLSLBindings();

    bool glslVertexBlendEnabled() const { return mGLSLVertexBlendEnabled; }
    void setGLSLVertexBlendEnabled(bool enable) { mGLSLVertexBlendEnabled = enable; }

    Geometry* geometry() { return mGeometry.get(); }
    const Geometry* geometry() const { return mGeometry.get(); }

    bool animationStarted() const { return mAnimationStarted; }

  public:
    ref<Geometry> mGeometry;
    ref<ArrayFVec3> mVertices;
    ref<ArrayFVec3> mNormals;
    std::vector< ref<ArrayFVec3> > mVertexFrames;
    std::vector< ref<ArrayFVec3> > mNormalFrames;

  protected:
    Real mLastUpdate;
    Real mElapsedTime;
    Real mAnimationStartTime;
    int mAnimationStart;
    int mAnimationEnd;
    float mAnimationPeriod;
    bool mAnimationStarted;
    int mFrame1;
    int mFrame2;

    bool mGLSLVertexBlendEnabled;
    int mVertex2_Binding;
    int mNormal2_Binding;
    int mAnim_t_Binding;
    float mAnim_t;
  };
  //-----------------------------------------------------------------------------
}

#endif