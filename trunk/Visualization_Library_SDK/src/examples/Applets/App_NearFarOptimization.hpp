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

#ifndef App_NearFarOptimization_INCLUDE_ONCE
#define App_NearFarOptimization_INCLUDE_ONCE

#include "BaseDemo.hpp"
#include "vlut/GeometryPrimitives.hpp"
#include "vl/SceneManagerActorTree.hpp"
#include "vl/Actor.hpp"
#include "vl/Effect.hpp"
#include "vl/Time.hpp"
#include "vl/Light.hpp"

class App_NearFarOptimization: public BaseDemo
{
public:
  virtual void shutdown() {}

  // called once after the OpenGL window has been opened 
  void initEvent()
  {
    // allocate the Transform 
    mCubeTransform = new vl::Transform;
    // bind the Transform with the transform tree of the rendring pipeline 
    vl::VisualizationLibrary::rendering()->as<vl::Rendering>()->transform()->addChild( mCubeTransform.get() );

    float fsize = 2500;
    // create the cube's Geometry and compute its normals to support lighting 
    vl::ref<vl::Geometry> ball1 = vlut::makeIcosphere( vl::vec3(0,0,0), fsize, 2, false );
    ball1->computeNormals();
    vl::ref<vl::Geometry> ball2 = vlut::makeIcosphere( vl::vec3(0,0,0), fsize*1.001f, 2, false );
    ball2->computeNormals();

    // setup the effect1 to be used to render the cube 
    vl::ref<vl::Effect> effect1 = new vl::Effect;
    // enable depth test and lighting 
    effect1->shader()->enable(vl::EN_DEPTH_TEST);
    // add a Light to the scene, since no Transform is associated to the Light it will follow the camera 
    effect1->shader()->setRenderState( new vl::Light(0) );
    // enable the standard OpenGL lighting 
    effect1->shader()->enable(vl::EN_LIGHTING);
    // set the front and back material color of the cube 
    // "gocMaterial" stands for "get-or-create Material"
    effect1->shader()->gocMaterial()->setDiffuse( vlut::crimson );

    // setup the effect2 to be used to render the cube 
    vl::ref<vl::Effect> effect2 = new vl::Effect;
    // enable depth test and lighting 
    effect2->shader()->enable(vl::EN_DEPTH_TEST);
    // add a Light to the scene, since no Transform is associated to the Light it will follow the camera 
    effect2->shader()->setRenderState( new vl::Light(0) );
    // enable the standard OpenGL lighting 
    effect2->shader()->enable(vl::EN_LIGHTING);
    // set the front and back material color of the cube 
    // "gocMaterial" stands for "get-or-create Material"
    effect2->shader()->gocMaterial()->setDiffuse( vlut::gold );

    // install our scene manager, we use the SceneManagerActorTree which is the most generic
    vl::ref<vl::SceneManagerActorTree> scene_manager = new vl::SceneManagerActorTree;
    vl::VisualizationLibrary::rendering()->as<vl::Rendering>()->sceneManagers()->push_back(scene_manager.get());
    // add the cube to the scene using the previously defined effect and transform 
    scene_manager->tree()->addActor( ball1.get(), effect1.get(), mCubeTransform.get()  );
    scene_manager->tree()->addActor( ball2.get(), effect2.get(), mCubeTransform.get()  );
    scene_manager->computeBounds();

    trackball()->adjustView( vl::VisualizationLibrary::rendering()->as<vl::Rendering>(), vl::vec3(0,0,1), vl::vec3(0,1,0), 1.0f );
  }

  // called every frame 
  virtual void run()
  {
    // rotates the cube around the Y axis 45 degrees per second 
    vl::Real degrees = vl::Time::currentTime() * 45.0f;
    vl::mat4 matrix = vl::mat4::rotation( degrees, 0,1,0 );
    mCubeTransform->setLocalMatrix( matrix );

    // periodically toggle near/far optimization
    if ( sin( vl::Time::currentTime() * 3.14159265 / 2 ) > 0 )
      vl::VisualizationLibrary::rendering()->as<vl::Rendering>()->setNearFarClippingPlanesOptimized(true);
    else
    {
      vl::VisualizationLibrary::rendering()->as<vl::Rendering>()->setNearFarClippingPlanesOptimized(false);
      // restore default perspective near/far values
      vl::VisualizationLibrary::rendering()->as<vl::Rendering>()->camera()->setProjectionAsPerspective(60.0f, 0.5f, 10000.0f);
    }
  }

protected:
  vl::ref<vl::Transform> mCubeTransform;
  bool mNearFarOptimized;
};

#endif
