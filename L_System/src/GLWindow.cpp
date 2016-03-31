#include "GLWindow.h"

#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Light.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Material.h>
#include <ngl/ShaderLib.h>
#include <QColorDialog>
#include <cmath>
#include <ctime>

//----------------------------------------------------------------------------------------------------------------------
/// @brief  pi for converting degrees to radians
//----------------------------------------------------------------------------------------------------------------------
const static float pi = M_PI;
//----------------------------------------------------------------------------------------------------------------------
/// @brief  increment for mouse position values
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.01f;
//----------------------------------------------------------------------------------------------------------------------
///This function was adapted from Jon Macey's code
GLWindow::GLWindow(const QGLFormat _format, QWidget *_parent ) : QGLWidget( _format, _parent )
{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());

  // set start values
  m_hitCount = 0;
  m_sunPos=(0.0, 100.0, 0.0);//
  m_rainPos = (0.0, 00.0, 0.0); //
  m_windVec = (10, 2, 10);
	m_scale=1.0;
	m_position=0.0;
  m_folliage = 0;
  m_wind = 0;
  m_rotate=false;
  m_spinXFace=0.0f;
  m_spinYFace=0.0f;
  m_scalePlant = 10.0f;
  m_depth = 0;
  m_height = 2.0f;
  m_camSlideY = 0;
  m_radius = 0.1;
  m_angleIncrement=(0,0,0);
  m_fps=0;
  m_frames=0;
  m_frameTotal=0;
  m_timer.start();
  m_intensitySun=0.1;
  m_sunOnOff = false;
  m_rainOnOff = false;
  m_confetti = false;
  m_axiom = 'F';
  m_rule = "F+F";
  m_rule1 = "X+F";
  m_smoothLevel = 1;
}

//----------------------------------------------------------------------------------------------------------------------
// This virtual function is called once before the first call to paintGL() or resizeGL(),
// and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.
/// This function was adapted from Jon Macey's code
void GLWindow::initializeGL()
{
  ngl::NGLInit::instance();
  glClearColor(0.8f, 0.7f, 0.7f, 1.0f);
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);


  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // create empty shaders for frag and vertex
  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/Phong.vs");
  shader->loadShaderSource("PhongFragment","shaders/Phong.fs");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");
  //link the shader after associating the data
  shader->linkProgramObject("Phong");

  //creating a new shader for texturing objects
  shader->createShaderProgram("TextureShader");
  shader->attachShader("TextureVertex",ngl::VERTEX);
  shader->attachShader("TextureFragment",ngl::FRAGMENT);
  shader->loadShaderSource("TextureVertex","shaders/TextureVertex.glsl");
  shader->loadShaderSource("TextureFragment","shaders/TextureFragment.glsl");
  shader->compileShader("TextureVertex");
  shader->compileShader("TextureFragment");
  shader->attachShaderToProgram("TextureShader","TextureVertex");
  shader->attachShaderToProgram("TextureShader","TextureFragment");
  shader->linkProgramObject("TextureShader");

  (*shader)["TextureShader"]->use();
  (*shader)["nglColourShader"]->use();
  (*shader)["Phong"]->use();
  ngl::Material m(ngl::GOLD);
  m.loadToShader("material");

  // Set from, up and to vectors for camera and create camera
  ngl::Vec3 from(0,40,20);
  ngl::Vec3 to(0,15,0);
  ngl::Vec3 up(0,1,0);

  m_cam= new ngl::Camera(from,to,up);
  m_cam->setShape(45.0f,(float)720.0/576.0f,0.05f,350.0f);
  // now to load the shader and set the values
  // grab an instance of shader manager
  // pass model view and projection values to the shader
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

  // get view matrices from camera and transpose to eye space
  ngl::Mat4 iv=m_cam->getViewMatrix();
  iv.transpose();
  // Create light and pass through the transpose of the camera matrix
  m_light = new ngl::Light(ngl::Vec3(-2,5,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT );
  m_light->setTransform(iv);
  //load transformations to shader
  m_light->loadToShader("light");

  // initialise primative objects
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",1.0,40);
  // set size for text to window
  m_text = new  ngl::Text(QFont("Arial",18));
  m_text->setScreenSize(this->size().width(),this->size().height());

  //initialise objects and assign textures
  m_pot = new ngl::Obj("obj/pot.obj","obj/potTexture.jpg");
  m_soil = new ngl::Obj("obj/soil.obj","obj/soil.jpg");

  m_pot->createVAO();
  m_soil->createVAO();

  // link data to shader
  (*shader)["nglColourShader"]->use();
  shader->setShaderParam4f("Colour", 0.4, 0.4, 0.9 ,1.0);

  // call to emitter constructor, particle position, quantity of particles
  m_weather = new Weather(ngl::Vec3(0,10,0), 100);
  // load camera, transform and shader from scene to m_weather
  m_weather->setCam(m_cam);
  m_weather->setTransform(m_transform);
  m_weather->setShaderName("Phong");
  m_weather->setShaderName("nglColourShader");

  // load camera, transform and shader from scene to m_draw
  m_draw = new drawPlant();
  m_draw->setCam(m_cam);
  m_draw->setTransform(m_transform);   //mnot needed
  m_draw->setShaderName("nglColourShader");

  m_text=new ngl::Text(QFont("Ariel",10));
  m_text->setScreenSize(width(),height());

  // start timer for program's duration
  m_fpsTimer =startTimer(0);
}

//----------------------------------------------------------------------------------------------------------------------
///This function was originally written by Jon Macey
void GLWindow::resizeGL( int _w, int _h )
{
  glViewport(0,0,_w,_h);
  m_cam->setShape(45,float(_w/_h),0.05,350);
  m_text->setScreenSize(this->size().width(),this->size().height());
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
/// This function was originally written by Jon Macey
void GLWindow::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // create model, view and projection matrices
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  // Multiplying matrices to give result MVP matrix
  M=m_mouseGlobalTX;
  MV=  M*m_cam->getViewMatrix();
  MVP= m_transform.getMatrix()*M*m_cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
}

//----------------------------------------------------------------------------------------------------------------------
// This virtual function is called whenever the widget needs to be painted.
// this is the main drawing routine
///This function was modified from Jon Macey's code
void GLWindow::paintGL()
{
  // clear depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Rotation based on the mouse position for the global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;

  // Create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);

  // multiply the rotation matrices
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  // get the VBO instance and draw the objects
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  (*shader)["nglColourShader"]->use();
  loadMatricesToShader();

  // Adding mouse transform matrices to weather.cpp.
  m_weather->setMouseTransform(m_mouseGlobalTX);
  m_weather->draw(m_rainOnOff, m_confetti);
  m_hitCount=m_weather->m_hitCount;


  m_lsystem = new LSystem();
  // function calls stringmanipulator to build the string
  // lsystem takes the returned string and converts to a list of vectors
  if (m_windOnOff==0)
  {
    m_wind==0;
  }
  m_lsystem->buildVectorList(m_height, m_angleIncrement, m_depth, m_sunPos, m_axiom, m_axiom1, m_rule, m_rule1, m_smoothLevel, m_intensitySun, m_wind, m_windVec, m_frameTotal, m_hitCount);
  // grabbing returned lists of vectors from lsystem
  m_vectorList = m_lsystem->m_vectorList;
  m_branchEnd = m_lsystem->m_branchEnd;
  m_leafList = m_lsystem->m_leafList;
  m_angleList = m_lsystem->m_angleList;

  shader->setShaderParam4f("Colour", 1.0,1.0,1.0,1.0);
  loadMatricesToShader();
  // drawing the grid
  m_draw->initializeObjects();

  // distance between the sun and plant rounded to 2 decimal places
  double _sunPlantDist =floor(((sqrt(abs(m_sunPos.m_x)*2+abs(m_sunPos.m_y)*2+abs(m_sunPos.m_z)*2)))*100+0.5)/100;
  shader->setShaderParam4f("Colour", 1-_sunPlantDist*0.05, _sunPlantDist*0.1+0.3,  1-_sunPlantDist*0.05,1.0);
  loadMatricesToShader();
  m_draw->buildPlant(m_radius, m_height, m_vectorList, m_branchEnd, m_leafList, m_angleList, m_folliage, m_wind);

  (*shader)["TextureShader"]->use();
  loadMatricesToShader();
  m_pot->draw();
  m_soil->draw();
  m_soil->calcDimensions();

  (*shader)["TextureShader"]->use();
  m_draw->setMouseTransform(m_mouseGlobalTX);
  m_draw->drawScene(m_sunPos, _sunPlantDist, m_intensitySun, m_sunOnOff);

  // camera slide position
  m_cam->slide(0, m_camSlideY/3, m_camSlideY);
  loadMatricesToShader();
  // increments the frames for the particles
  // increments frame total to be used to calculate the wind swaying of the plant
  ++m_frames;
  ++m_frameTotal;

  // Printing the text onto the screen
  QString text_m_frameTotal=QString("Frames total: %1 ").arg(m_frameTotal);
  QString text_m_hit=QString("Hit count: %1 ").arg(m_weather->m_hitCount);
  QString text_m_sunPos=QString("Sun Pos ( %1, %2, %3 )").arg(m_sunPos.m_x).arg(m_sunPos.m_y).arg(m_sunPos.m_z);
  QString text_sunPlantDist=QString("Plant-sun distance: %1 ").arg(_sunPlantDist);

  // Rendering the text onto the display.
  m_text->renderText(700,200,text_m_sunPos);
  m_text->renderText(700,215,text_m_hit);
  m_text->renderText(700,230,text_sunPlantDist);
  m_text->renderText(700,245,text_m_frameTotal);
}

//----------------------------------------------------------------------------------------------------------------------
// Save parameters to config file
void GLWindow::fileOut()
{
  std::ofstream fileOut;
  fileOut.open("Config.txt");
  if (!fileOut.is_open())
  {
      std::cerr<<"File open error\n";
      exit(EXIT_FAILURE);
  }

  fileOut<<"\nAxiom : "<<m_axiom;
  fileOut<<"\nAxiom 1 :"<<m_axiom1;
  fileOut<<"\nRule : "<<m_rule;
  fileOut<<"\nRule1 : "<<m_rule1;
  fileOut<<"\nHeight: "<<m_height;
  fileOut<<"\nDepth: "<<m_depth;
  fileOut<<"\nPlant rotation XYZ: "<<m_angleIncrement.m_x<<", "<<m_angleIncrement.m_y<<", "<<m_angleIncrement.m_z;
  fileOut<<"\nLeaves On/Off"<<m_folliage;
  fileOut<<"\nSmoothness"<<m_smoothLevel<<"\n";
  fileOut<<"\nSun On/Off: "<<m_sunOnOff;
  fileOut<<"\nSun Intensity: "<<m_intensitySun;
  fileOut<<"\nSun position: "<<m_sunPos.m_x<<", "<<m_sunPos.m_y<<", "<<m_sunPos.m_z<<"\n";
  fileOut<<"\nWind On/Off: "<<m_windOnOff<<m_axiom;
  fileOut<<"\nWind intensity: "<<m_wind;
  fileOut<<"\nWind Direction: "<<m_windVec.m_x<<", "<<m_windVec.m_y<<", "<<m_windVec.m_z<<"\n";
  fileOut<<"\nConfetti On/Off:"<<m_confetti;
  fileOut<<"\nRain On/Off: "<<m_rainOnOff;
  fileOut<<"\nRain position XYZ: "<<m_rainPos.m_x<<", "<<m_rainPos.m_y<<", "<<m_rainPos.m_z<<"\n";
  fileOut<<"\nCamera slide: "<<m_camSlideY;

  fileOut.close();
}

//----------------------------------------------------------------------------------------------------------------------
// Changing the position of the scene when there's a mouse move event
///This function was originally written by Jon Macey
void GLWindow::mouseMoveEvent ( QMouseEvent * _event )
{
  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx=_event->x()-m_origX;
    int diffy=_event->y()-m_origY;
    m_spinXFace += (float) 0.5f * diffy;
    m_spinYFace += (float) 0.5f * diffx;
    m_origX = _event->x();
    m_origY = _event->y();
    updateGL();
  }
  else if(m_translate && _event->buttons() == Qt::RightButton)
  {
    int diffX = (int)(_event->x() - m_origXPos);
    int diffY = (int)(_event->y() - m_origYPos);
    m_origXPos=_event->x();
    m_origYPos=_event->y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    updateGL();
   }
}


//----------------------------------------------------------------------------------------------------------------------
///This function was originally written by Jon Macey
void GLWindow::mousePressEvent (QMouseEvent * _event  )
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate =true;
  }
  // right mouse translate mode
  else if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translate=true;
  }
}

//----------------------------------------------------------------------------------------------------------------------
///This function was originally written by Jon Macey
void GLWindow::mouseReleaseEvent (  QMouseEvent * _event  )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
  // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
///This function was originally written by Jon Macey
void GLWindow::timerEvent(QTimerEvent *_event )
{
  m_weather->update(m_rainOnOff, m_confetti);
  if(_event->timerId() == m_fpsTimer)
  {
    if( m_timer.elapsed() > 1000.0)
    {
      m_fps=m_frames;
      m_frames=0;
      m_timer.restart();
    }
   }
updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change smoothness level of tree
void GLWindow::camSlideY(int _y)
{
  m_camSlideY = _y;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change depth level
void GLWindow::setDepth(int _i)
{
  m_depth = _i;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change smoothness level of tree
void GLWindow::smoothLevel(int _i)
{
  m_smoothLevel = _i;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change angle increment
void GLWindow::angleIncrementX(double _x)
{
  m_angleIncrement.m_x = _x;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change angle increment
void GLWindow::angleIncrementY( double _y )
{
  m_angleIncrement.m_y=_y;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change angle increment
void GLWindow::angleIncrementZ(double _z)
{
  m_angleIncrement.m_z = _z;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change plant height
void GLWindow::plantHeight(double _i)
{
  m_height = _i;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to toggle leaf on/off
void GLWindow::toggleLeaf(bool _mode)
{

  m_folliage = _mode;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change sun position
void GLWindow::sunPosX(double _x)
{
  m_sunPos.m_x = _x;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change sun position
void GLWindow::sunPosY(double _y)
{
  m_sunPos.m_y = _y;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change sun position
void GLWindow::sunPosZ(double _z)
{
  m_sunPos.m_z = _z;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to change sun intensity
void GLWindow::sunIntensity(double _i)
{
  m_intensitySun = _i;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to toggle sun on/off
void GLWindow::toggleSun(bool _mode)
{
  m_sunOnOff=_mode;
  sunPosX(5);
  sunPosY(15);
  sunPosZ(5);
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to toggle rain on/off
void GLWindow::toggleRain(bool _mode)
{
  m_rainOnOff=_mode;
  m_rainPos = (0, 40, 0);
  rainPosY(20);
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to toggle confetti on/off
void GLWindow::toggleConfetti(bool _mode)
{
  m_confetti=_mode;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function for rain position
void GLWindow::rainPosX(double _x)
{
  m_rainPos.m_x = _x;

  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function for rain position
void GLWindow::rainPosY(double _y)
{
  m_rainPos.m_y = _y;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function for rain position
void GLWindow::rainPosZ(double _z)
{
   m_rainPos.m_z = _z;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function to toggle wind on/off
void GLWindow::toggleWind(bool _mode)
{
//  m_windOnOff = _mode;
//  if (m_windOnOff==false)
//  {
//    m_wind=0;
//  }
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function for wind intensity
void GLWindow::windIntensity(double _i)
{
  if (m_windOnOff==true)
  {
    m_wind = _i;
  }
  else if(m_windOnOff==false)
  {
    m_wind = 0;
  }
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function for wind direction
void GLWindow::windDirX(double _x)
{
  m_windVec.m_x = _x;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function for wind direction
void GLWindow::windDirY(double _y)
{
  m_windVec.m_y = _y;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
//Function for wind direction
void GLWindow::windDirZ(double _z)
{
  m_windVec = _z;
  updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
// Destructor for GLWindow
GLWindow::~GLWindow()
{
}
