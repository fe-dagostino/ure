

#include "ure_application.h"
#include "ure_resources_collector.h"
#include "ure_window.h"
#include "ure_window_options.h"
#include "ure_scene_graph.h"
#include "ure_scene_layer_node.h"
#include "ure_view_port.h"
#include "widgets/ure_layer.h"
#include "ure_utils.h"

#include "core/utils.h"
#include "core/stop_watch.h"

class OglGui : public ure::ApplicationEvents, public ure::WindowEvents
{
public:
  OglGui( int argc, char** argv )
    : m_bFullScreen(false), m_position{0,0}, m_size{1024,768}, m_fb_size{0,0}
  {
    init(argc, argv);
  }

private:
  void loadResources()
  {
    ure::Image    bkImage; 

    bkImage.load( ure::Image::loader_t::eStb, "./resources/media/wall.jpg" );

    ure::Texture*  pTexture = new(std::nothrow) ure::Texture( std::move(bkImage) );

    ure::ResourcesCollector::get_instance()->attach("wall", pTexture);
  }

  void addCamera()
  {
    ure::Camera*  pCamera =  new ure::Camera( true );
  
    glm::vec3 cameraPosition = glm::vec3(0,0,1);  // Camera is at (4,3,3), in World Space
    glm::vec3 cameraTarget   = glm::vec3(0,0,0);  // and looks at the origin
    glm::vec3 upVector       = glm::vec3(0,1,0);  // Head is up (set to 0,-1,0 to look upside-down)

    glm::mat4 CameraMatrix = glm::lookAt(
                                            cameraPosition, // the position of your camera, in world space
                                            cameraTarget,   // where you want to look at, in world space
                                            upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
                                        );
    pCamera->set_view_matrix( CameraMatrix );
  
    m_pViewPort->get_scene()->add_scene_node( new ure::SceneCameraNode( "MainCamera", pCamera ) );
  }

  void addWallLayer()
  {
    ure::widgets::Layer* pLayer = new(std::nothrow) ure::widgets::Layer( *m_pViewPort );
    
    m_pWindow->connect(pLayer);

    pLayer->set_visible( true );
    pLayer->set_enabled( true );

    pLayer->set_position( -1.0f*m_size.width/2, -1.0f*m_size.height/2, true );

    ure::Texture* pTexture = ure::ResourcesCollector::get_instance()->find<ure::Texture>("wall");
    
    pLayer->set_background( pTexture, ure::widgets::Widget::BackgroundOptions::eboAsIs );

    ure::SceneLayerNode* pNode = new(std::nothrow) ure::SceneLayerNode( "Layer1", pLayer );
   
    glm::mat4 mModel =  glm::ortho( -1.0f*(float)m_size.width/2, (float)m_size.width/2, (float)m_size.height/2, -1.0f*(float)m_size.height/2, 0.1f, 1000.0f );
    pNode->set_model_matrix( mModel );
    pNode->get_model_matrix().translate( 0, 0,   0 );

    m_pViewPort->get_scene()->add_scene_node( pNode );    
  }

  void init( [[__maybe_unused__]] int argc, [[__maybe_unused__]] char** argv )
  {
    const std::string sShadersPath( "./resources/shaders/" );
    const std::string sMediaPath( "./resources/media/" );

    ure::Application::initialize( core::unique_ptr<ure::ApplicationEvents>(this,false), sShadersPath, sMediaPath );

    //
    m_pWindow = new(std::nothrow) ure::Window();
    if ( m_pWindow == nullptr )
    {
      ure::utils::log( "Unable to start application" );
      return ;
    }

    // Add this as listener for WindowEvents
    m_pWindow->connect( this );
    
    std::unique_ptr<ure::window_options> options = 
          std::make_unique<ure::window_options>( 
                (m_bFullScreen==true)?"main":"", 
								 "GLES GUI Application",
								 m_position, 
								 m_size 
          );
    
    m_pWindow->create( std::move(options), static_cast<ure::enum_t>(ure::Window::ProcessingFlags::epfCalling) );
    m_pWindow->set_swap_interval(1);

    //////////////////////////////////////////////////////////////////////////

    const ure::Renderer* renderer = m_pWindow->get_renderer();

    std::string  sVendor;
    std::string  sRenderer;
    std::string  sVersion;
    std::string  sShaderVersion;
    
    renderer->get_vendor  ( sVendor );
    ure::utils::log( core::utils::format( "Vendor:         [%s]", sVendor.c_str() )        );
    renderer->get_renderer( sRenderer );
    ure::utils::log( core::utils::format( "Renderer:       [%s]", sRenderer.c_str() )      );
    renderer->get_version( sVersion );
    ure::utils::log( core::utils::format( "Version:        [%s]", sVersion.c_str() )       );
    renderer->get_shader_version( sShaderVersion );
    ure::utils::log( core::utils::format( "Shader Version: [%s]", sShaderVersion.c_str() ) );

    //////////////////////////////////////////////////////////////////////////

    ure::SceneGraph* pSceneGraph = new ure::SceneGraph();
    if ( pSceneGraph == nullptr )
    {
      ure::utils::log( "Unable to allocate SceneGraph" );
      return ;
    }

    glm::mat4 mProjection = glm::perspectiveFov(45.0f, (float)m_size.width, (float)m_size.height, 0.1f, 500.0f);
  
    m_pViewPort   = new ure::ViewPort( pSceneGraph, mProjection );


    loadResources();

    addCamera();

    addWallLayer();

  }

public:
  /***/
  void run()
  {
    ure::utils::log( "OglGui::Run(): ENTER" );
    ure::Application::get_instance()->run();
    ure::utils::log( "OglGui::Run(): EXIT"  );
  }

  /***/
  void dispose()
  {
    m_pWindow->destroy();

    ure::Application::get_instance()->finalize();
  }

protected:

// ure::ApplicationEvents implementation  
protected:
  virtual void on_run() override 
  {
    if ( m_pWindow->check( ure::Window::WindowFlags::eWindowShouldClose ) )
    {
      ure::Application::get_instance()->quit(true);
    }

    m_pWindow->get_framebuffer_size( m_fb_size );
    
    ///////////////
    m_pViewPort->set_area( 0, 0, m_fb_size.width, m_fb_size.height );
    m_pViewPort->use();

    // Update background color
    m_pViewPort->get_scene()->set_background( 0.2f, 0.2f, 0.2f, 0.0f );

    ure::SceneLayerNode* pLayer1 = m_pViewPort->get_scene()->get_scene_node<ure::SceneLayerNode>("SceneNode","Layer1");
    if ( pLayer1 != nullptr )
    {
      pLayer1->get_model_matrix().rotateX(0.01);
      pLayer1->get_model_matrix().rotateY(0.01);
      pLayer1->get_model_matrix().rotateZ(0.01);
    }

    ///////////////
    m_pViewPort->clear_buffer( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

    ///////////////
    m_pViewPort->render();


    ///////////////
    m_pWindow->swap_buffers();
    
    ///////////////
    // Will process messages that requires to be executed on main thread.
    m_pWindow->process_message();

    ure::Application::get_instance()->poll_events();

    //std::cout << "fps: " << (double(1000) / m_sw.peek()) << std::endl;
    m_sw.reset();
  }

  /***/
  virtual void on_initialize_error(/* @todo */) override {}
  /***/
  virtual void on_error( [[maybe_unused]] int32_t error, [[maybe_unused]] const std::string& description ) override {}
  /***/
  virtual void on_finalize_error(/* @todo */) override {};

private:
  bool                        m_bFullScreen;
  ure::position_t<ure::int_t> m_position;
  ure::Size                   m_size;
  ure::Size                   m_fb_size;

//  BOOL            m_bDefaultKeys;

//  BOOL            m_bVerticalSync;
//  BOOL            m_bShowStats;

  ure::Window*      m_pWindow;
  ure::ViewPort*    m_pViewPort;


  core::stop_watch<std::chrono::milliseconds,true>  m_sw;
};

extern "C" int main(int argc, char** argv)
{
  OglGui _oglgui(argc, argv);

  _oglgui.run();
  
  _oglgui.dispose();

  return 0;
}
