#ifndef DRAW_H
#define DRAW_H

#include "LSystem.h"

#include <ngl/VAOPrimitives.h>
#include <ngl/Mat4.h>
#include <ngl/Vec3.h>
#include <ngl/Transformation.h>
#include <ngl/Obj.h>
#include <deque>
#include <vector>
#include <ngl/Camera.h>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Vec3.h>
#include <cmath>
#include <ngl/Text.h>
//----------------------------------------------------------------------------------------------------------------------
/// @file drawPlant.h
/// @author Ellie Ansell
/// @version 3.0
/// @date 09/05/15
/// @class drawPlant.cpp
/// @brief The class to draw the plant and sun
//----------------------------------------------------------------------------------------------------------------------
class drawPlant
{
public:
//----------------------------------------------------------------------------------------------------------------------
/// @brief the constructor for the Drawplant class
//----------------------------------------------------------------------------------------------------------------------
  drawPlant();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the function to initialise plant pot and soil
  //----------------------------------------------------------------------------------------------------------------------
   void initializeObjects();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the function to return the point data to buildPlant function
  /// @param[in] _startPoint the vector which the end of the branch circle rotates around
  /// @param[in] _radius the radius of the circle
  /// @param[in] _angle the angle the circle rotates by
  /// @param[in] _circleRotate the vector of angles the circle rotates by
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 circleData(const ngl::Vec3 _startPoint, const float _radius, const float _angle, const ngl::Vec3 _circleRotate);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief uses the camera from GLWindow and loads matrices to shader to project onto the screen
  /// @param[in] _sunPos the vector containing the sun's position
  /// @param[in] _sunPlantDist the distance between the sun and plant which determines the plant's branch colour
  /// @param[in] _intensitySun the brightness of the sun, which determines the sun's colour
  /// @param[in] _toggleSun query of whether the sun is on or off
  //----------------------------------------------------------------------------------------------------------------------
  void drawScene(ngl::Vec3 _sunPos, const int _sunPlantDist, const float _intensitySun, bool _toggleSun);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief building the GLPoints into cylindrical shapes
  /// @param[in] _radius the radius of the plant
  /// @param[in] _height the height of each branch
  /// @param[in] _vectorList a list of vectors making the tree
  /// @param[in] _branchEnd a list specifying where the branches end in order to stop drawing points
  /// @param[in] _leafList a list containing vectors for where the leaves are
  /// @param[in] _angleList a list containing the angles in order to do circle rotation at the tips of the branches
  /// @param[in] _folliage queries whether there's folliage. If value is 1, draw leaves.
  /// @param[in] _wind queries intensity of the wind.
  //----------------------------------------------------------------------------------------------------------------------
  void buildPlant(float _radius, const float _height, std::deque<ngl::Vec3> _vectorList, std::deque<int>_branchEnd, std::deque<ngl::Vec3> _leafList, std::deque<ngl::Vec3> _angleList, const int _folliage, const int _wind);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief VAO objects to draw the leaf
  /// @param[in] _option if 1, a sphere is drawn
  //----------------------------------------------------------------------------------------------------------------------
  void drawFoliage(const int _option);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief function to set the camera from GLWindow
  /// @param[in] *_cam the camera loaded to the scene
  //----------------------------------------------------------------------------------------------------------------------
  inline void setCam(ngl::Camera *_cam){m_cam=_cam;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns m_cam from GLWindow
  //----------------------------------------------------------------------------------------------------------------------
  inline ngl::Camera * getCam()const {return m_cam;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief function to set the shader from GLWindow
  /// @param[in] &_n assignes the shader name to m_shaderName
  //----------------------------------------------------------------------------------------------------------------------
  inline void setShaderName(const std::string &_n){m_shaderName=_n;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the shadername from GLWindow
  //----------------------------------------------------------------------------------------------------------------------
  inline const std::string getShaderName()const {return m_shaderName;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the transform matrix from GLWindow
  /// @param[in] _transform sets the transformation from GLWindow
  //----------------------------------------------------------------------------------------------------------------------
  inline void setTransform(ngl::Transformation _transform){m_transform = _transform;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set to the mousetransform matrices from GLWindow
  /// @param[in] _mouseGlobalTX sets the mouse transform matrices from GLWindow
  //----------------------------------------------------------------------------------------------------------------------
  inline void setMouseTransform(ngl::Mat4 _mouseGlobalTX){m_mouseGlobalTX = _mouseGlobalTX;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief transform for objects
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Transformation m_transform;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector list used to query parts of the plant positions
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <ngl::Vec3> m_vectorList;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief mouse global position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Mat4 m_mouseGlobalTX;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the shader name
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_shaderName;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief create the camera
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Camera *m_cam;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the pointer vertex array object
  //----------------------------------------------------------------------------------------------------------------------
  ngl::VertexArrayObject *m_vao;

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Loads MVP matrices to shader
  //----------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader();
};
#endif
