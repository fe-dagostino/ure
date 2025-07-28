

#include "ure_application.h"
#include "ure_resources_collector.h"
#include "ure_resources_fetcher.h"
#include "ure_websocket.h"
#include "ure_window.h"
#include "ure_window_options.h"
#include "ure_scene_graph.h"
#include "ure_scene_layer_node.h"
#include "ure_view_port.h"
#include "widgets/ure_layer.h"
#include "ure_utils.h"

#include "core/utils.h"
#include "core/stop_watch.h"

#if defined(_IMGUI_ENABLED)
# include "imgui.h"
#endif

class OglGui : public ure::ApplicationEvents, public ure::WindowEvents, public ure::ResourcesFetcherEvents, public ure::WebSocketEvents
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

    std::shared_ptr<ure::Texture> texture = std::make_shared<ure::Texture>( std::move(bkImage), ure::Texture::lifecycle_t::eObject );
    if ( texture )
    {
      m_rc->attach<ure::Texture,std::shared_ptr<ure::Texture>>("wall", std::move(texture) );
    }
  }

  void addCamera()
  {
    ure::camera_ptr camera = std::make_shared<ure::Camera>( true );
    if ( camera == nullptr )
      return ;

    glm::vec3 cameraPosition = glm::vec3(0,0,1);  // Camera is at (4,3,3), in World Space
    glm::vec3 cameraTarget   = glm::vec3(0,0,0);  // and looks at the origin
    glm::vec3 upVector       = glm::vec3(0,1,0);  // Head is up (set to 0,-1,0 to look upside-down)

    glm::mat4 CameraMatrix = glm::lookAt(
                                            cameraPosition, // the position of your camera, in world space
                                            cameraTarget,   // where you want to look at, in world space
                                            upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
                                        );
                                        
    /* Initially we are going to use 2D in 3D space, that means no view matrix to be applied in the MVP */
    camera->set_view_matrix( CameraMatrix );

    if ( m_view_port->has_scene_graph() )
    {
      m_view_port->get_scene().add_scene_node( new ure::SceneCameraNode( "MainCamera", camera ) );
    }
  }

  void addWallLayer()
  {
    std::shared_ptr<ure::widgets::Layer> layer = std::make_shared<ure::widgets::Layer>( *m_view_port );
    
    m_window->connect( layer->get_windows_events() );

    layer->set_visible( true );
    layer->set_enabled( true );

    layer->set_position( -1.0f*m_size.width/2, -1.0f*m_size.height/2, true );

    auto texture = m_rc->find<ure::Texture>("wall");
    if ( texture.has_value() )
    {
      layer->set_background( texture.value(), ure::widgets::Widget::BackgroundOptions::eboAsIs );
    }

    ure::SceneLayerNode* pNode = new(std::nothrow) ure::SceneLayerNode( "Layer1", layer );
   
    glm::mat4 mModel =  glm::ortho( -1.0f*(float)m_size.width/2, (float)m_size.width/2, (float)m_size.height/2, -1.0f*(float)m_size.height/2, 0.1f, 500.0f );
    //glm::mat4 mModel =  glm::ortho( 0.0f, (float)m_size.width, (float)m_size.height, 0.0f, 0.1f, 1.0f );

    pNode->set_model_matrix( mModel );
    pNode->get_model_matrix().translate( 0, 0, 0 );

    if ( m_view_port->has_scene_graph() )
      m_view_port->get_scene().add_scene_node( pNode );    
  }

  void init( [[__maybe_unused__]] int argc, [[__maybe_unused__]] char** argv )
  {
    const std::string sShadersPath( "./resources/shaders/" );
    const std::string sMediaPath( "./resources/media/" );
    ure::Application::initialize( core::unique_ptr<ure::ApplicationEvents>(this,false), sShadersPath );

    //
    m_window = std::make_unique<ure::Window>();
    if ( m_window == nullptr )
    {
      ure::utils::log( "Unable to start application" );
      return ;
    }

    // Add this as listener for WindowEvents
    m_window->connect( this );
    
    std::unique_ptr<ure::window_options> options = std::make_unique<ure::window_options>( 
                                                                                          (m_bFullScreen==true)?"main":"", 
                                                                                          "GLES GUI Application",
                                                                                          m_position, 
                                                                                          m_size 
                                                                                        );
    
    m_window->create( std::move(options), static_cast<ure::enum_t>(ure::Window::processing_flag_t::epfCalling) );
    m_window->set_swap_interval(1);

    //////////////////////////////////////////////////////////////////////////

    const ure::Renderer* renderer = m_window->get_renderer();

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

    std::unique_ptr<ure::SceneGraph> scene_graph = std::make_unique<ure::SceneGraph>();
    if ( scene_graph == nullptr )
    {
      ure::utils::log( "Unable to allocate SceneGraph" );
      return ;
    }

    glm::mat4 mProjection = glm::perspectiveFov(45.0f, (float)m_size.width, (float)m_size.height, 0.1f, 500.0f);
    //glm::mat4 mProjection = glm::mat4(1);
    //glm::mat4 mProjection = glm::ortho( -1.0f*m_size.width/2, 1.0f*m_size.width/2, 1.0f*m_size.height/2, -1.0f*m_size.height/2 ); //, -1.0f, 1.0f );

    m_view_port   = std::make_unique<ure::ViewPort>( std::move(scene_graph), mProjection );


    loadResources();

    addCamera();

    addWallLayer();

    ure::ResourcesFetcher::get_instance()->fetch( *this, "map-0-0-0", typeid(ure::Texture), "https://tile.openstreetmap.org/0/0/0.png",
                                                  ure::customer_request_t::Get,
                                                  ure::ResourcesFetcher::http_headers_t{ "Application", "json" },
                                                  ure::ResourcesFetcher::http_body_t{}
                                                );
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
    m_window->destroy();

    ure::Application::get_instance()->finalize();
  }

// ure::ApplicationEvents implementation  
protected:
  /***/
  virtual ure::void_t on_initialize() noexcept(true) override
  {
    m_rc = std::make_unique<ure::ResourcesCollector>();

    /* If your application requires external resources than resource fetcher 
       will make this easier.
       In order to control singleton lifetime it is strongly suggested to create
       and destroy it with the application lifetim, so using on_initialize() and on_finalize() events. */
    ure::ResourcesFetcher::initialize();
  }
  /***/
  virtual ure::void_t on_initialized() noexcept(true) override
  {  
  }

  /***/
  virtual ure::void_t on_finalize() noexcept(true) override
  {
    /* Finalize Resource Fetcher */
    ure::ResourcesFetcher::get_instance()->finalize();
  }
  /***/
  virtual ure::void_t on_finalized() noexcept(true) override
  {  }

  virtual ure::void_t on_run() noexcept(true) override 
  {
    if ( m_window->check( ure::Window::window_flag_t::eWindowShouldClose ) )
    {
      ure::Application::get_instance()->exit(true);
      return;
    }

    m_window->get_framebuffer_size( m_fb_size );
    
    ///////////////
    m_view_port->set_area( 0, 0, m_fb_size.width, m_fb_size.height );
    m_view_port->use();

    // Update background color
    if ( m_view_port->has_scene_graph() )
    {
      ure::SceneGraph& sgr = m_view_port->get_scene();
      
      sgr.set_background( 0.2f, 0.2f, 0.2f, 0.0f );

      ure::SceneLayerNode* pLayer1 = sgr.get_scene_node<ure::SceneLayerNode>("SceneNode","Layer1");
      if ( pLayer1 != nullptr )
      {
        pLayer1->get_model_matrix().rotateX(0.01);
        pLayer1->get_model_matrix().rotateY(0.01);
        pLayer1->get_model_matrix().rotateZ(0.01);
      }
    }

    ///////////////
    m_view_port->clear_buffer( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
 
#if defined(_IMGUI_ENABLED)
    ImGui::ShowDemoWindow(); // Show demo window! :)
#endif

    ///////////////
    m_view_port->render();

    ///////////////
    m_window->swap_buffers();
    
    ///////////////
    // Will process messages that requires to be executed on main thread.
    m_window->process_message();

    ure::Application::get_instance()->poll_events();

    //std::cout << "fps: " << (double(1000) / m_sw.peek()) << std::endl;
    m_sw.reset();

#ifdef _USE_WEBSOCKETS
    static bool _ws_init = false;
    static bool _ws_sent = false;
    if (_ws_init==false)
    {
      m_ws = std::make_unique<ure::WebSocket>( "ws test", *this, ure::websocket_options{"ws://127.0.0.1:8080/api/v1/cpt/notifications", ure::websocket_options::ws_mode_t::text, false } );
      
      m_ws->open();
      _ws_init = true;
    }
    else
    {
      if ( _ws_sent == false )
      {
        if ( m_ws->is_ready() )
        {
          if ( m_ws->send( (const uint8_t*)m_msg.c_str(), m_msg.length() ) == true )
          {
            _ws_sent = true;
          }
          else
          {
            printf("Socket failed to send\n");
          }
        }
      }
    }
#endif /* _USE_WEBSOCKETS */

  }

  /***/
  virtual ure::void_t on_initialize_error(/* @todo */) noexcept(true) override {}
  /***/
  virtual ure::void_t on_error( [[maybe_unused]] int32_t error, [[maybe_unused]] std::string_view description ) noexcept(true) override {}
  /***/
  virtual ure::void_t on_finalize_error(/* @todo */) noexcept(true) override {};


/* ure::WindowsEvents implementation */
protected:  

  virtual ure::void_t  on_key_pressed ( [[maybe_unused]] ure::Window* window, [[maybe_unused]] ure::key_t key, [[maybe_unused]] ure::int_t scan_code, [[maybe_unused]] ure::word_t mods ) noexcept(true) override
  {
    switch (key)
    {
      case ure::key_t::KEY_C:
      {
        //glm::mat4 view_matrix = glm::mat4(1);

        //m_view_port->get_scene()->get_active_camera()->get_camera()->get_view_matrix() = view_matrix;


      }; break;
    
      default:
      {
        ure::WindowEvents::on_key_pressed( window, key, scan_code, mods );
      }; break;
    }
    
  }

/* ure::ResourcesFetcherEvents implementation */
protected:  
  virtual ure::void_t on_download_succeeded(
                                             [[maybe_unused]] std::string_view name,
                                             [[maybe_unused]] ure::customer_request_t cr,
                                             [[maybe_unused]] const std::type_info& type,
                                             [[maybe_unused]] const ure::byte_t* data,
                                             [[maybe_unused]] ure::uint_t length
                                           ) noexcept(true) override
  {
    ure::Image image;
    if ( image.create( ure::Image::loader_t::eStb, data, length ) )
    {
      std::cout << "Well done " << name << std::endl;
    }

  }

  virtual ure::void_t on_download_failed   ( [[maybe_unused]] std::string_view name,
                                             [[maybe_unused]] ure::customer_request_t cr
                                           ) noexcept(true) override
  {

  }

/* ure::WebSocketEvents implementation */
  /***/
  virtual ure::bool_t    on_websocket_open   ( [[maybe_unused]] const std::string_view id ) noexcept(true) override
  { std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl; return true; }
  /***/
  virtual ure::bool_t    on_websocket_close  ( [[maybe_unused]] const std::string_view id, uint16_t code ) noexcept(true) override
  { std::cout << __FUNCTION__ << ":" << __LINE__ << " code:" << code << std::endl; return true; }
  /***/
  virtual ure::bool_t    on_websocket_message( [[maybe_unused]] const std::string_view id, [[maybe_unused]] ure::websocket_options::ws_mode_t mode, [[maybe_unused]] const uint8_t* data, [[maybe_unused]] uint32_t length ) noexcept(true) override
  { std::cout << __FUNCTION__ << ":" << __LINE__ << " MODE = " << static_cast<std::size_t>(mode) << ", DATA LEN = " << length << std::endl; return true; }
  /***/
  virtual ure::bool_t    on_websocket_error  ( [[maybe_unused]] const std::string_view id ) noexcept(true) override
  { std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl; return true; }

private:
  using resources_collector_t = std::unique_ptr<ure::ResourcesCollector>;
#ifdef _USE_WEBSOCKETS
  using websocket_t_ptr       = std::unique_ptr<ure::WebSocket>;
#endif /* _USE_WEBSOCKETS */

  bool                        m_bFullScreen;
  ure::position_t<ure::int_t> m_position;
  ure::Size                   m_size;
  ure::Size                   m_fb_size;

  resources_collector_t       m_rc;                   /* resource collector */

  std::unique_ptr<ure::Window>      m_window;
  std::unique_ptr<ure::ViewPort>    m_view_port;

#ifdef _USE_WEBSOCKETS
  websocket_t_ptr             m_ws;
#endif /* _USE_WEBSOCKETS */

  const std::string                                 m_msg = "{}";
  core::stop_watch<std::chrono::milliseconds,true>  m_sw;
};


#define OSM_ENDPOINT_SD "https://%s.tile.openstreetmap.org/%s/%s/%s.png"
#define OSM_ENDPOINT "https://tile.openstreetmap.org/%s/%s/%s.png"

int main(int argc, char** argv)
{
  OglGui _oglgui(argc, argv);

  _oglgui.run();
  
  _oglgui.dispose();

  return 0;
}
