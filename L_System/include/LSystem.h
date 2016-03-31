#ifndef LSYSTEM_H
#define LSYSTEM_H

#include "StringManipulator.h"

#include <deque>
#include <ngl/Vec3.h>
#include <vector>
//----------------------------------------------------------------------------------------------------------------------
/// @file LSystem.h
/// @author Ellie Ansell
/// @version 3.0
/// @date 09/05/15
/// @class  LSystem.cpp
/// @brief  converts string to vectors
//----------------------------------------------------------------------------------------------------------------------
class LSystem
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for the LSystem class
  //----------------------------------------------------------------------------------------------------------------------
  LSystem();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the depth to be inputted to string manipulator
  //----------------------------------------------------------------------------------------------------------------------
  int m_depth;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief axiom string
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_axiom;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the second axiom string
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_axiom1;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the rule string
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_rule;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the second rule string
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_rule1;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the finalrule from stringmanipulator
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_finalRule;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the resulting lists of vectors to be returned to GLWindow, then passed to plant.cpp for drawing
  //----------------------------------------------------------------------------------------------------------------------
  std::deque <ngl::Vec3> m_vectorList, m_leafList, m_angleList;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the resulting list of branchEnd which records '1' whenever vectorList is at the end of a branch
  //----------------------------------------------------------------------------------------------------------------------
  std::deque <int> m_branchEnd;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the pointer to stringmanipulator class
  //----------------------------------------------------------------------------------------------------------------------
  StringManipulator *stringManipulator;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the depth to be inputted to string manipulator
  /// @param[in] _height height of the plant
  /// @param[in] _angleIncrement the vector storing the incementation of the angles
  /// @param[in] _depth the depth of the string
  /// @param[in] _sunPos the vector for the sun's position which the branch will tend towards
  /// @param[in] _axiom the initial axiom
  /// @param[in] _axiom1 the second axiom
  /// @param[in] _rule the initial rule
  /// @param[in] _rule1 the second rule
  /// @param[in] _smoothLevel the smoothness level, used to bend the branches for a more natural look
  /// @param[in] _sunIntensity the intensity of the sun which amplifies the amount the branches tend towards the sun
  /// @param[in] _wind the intensity of the wing which changes the amount vectors sway by
  /// @param[in] _windVec the vector for the direction of the wind
  /// @param[in] _frames the total running number of frames used to calculate the swaying of the branches
  /// @param[in] _hitCount the hit count for the rain hitting the pot. This influences sun intensity, height and angleIncrement with the more rain the plant recieves.
  //----------------------------------------------------------------------------------------------------------------------
  void buildVectorList(float _height, ngl::Vec3 _angleIncrement, int _depth, ngl::Vec3 _sunPos, char axiom, char axiom1, std::string _rule, std::string _rule1, int _smoothLevel, float _sunIntensity, float _wind, ngl::Vec3 _windVec, int _frames, int _hitCount);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief call to stringmanipulator function and returns the string
  /// @param[in] _depth the Recursive depth of the string
  /// @param[in] _axiom Ininitial axiom
  /// @param[in] _axiom1 Second axiom
  /// @param[in] _rule initial rule
  /// @param[in] _rule1 Second rule
  /// @param[in] _startString the starting character for building the string
  //----------------------------------------------------------------------------------------------------------------------
  std::string loadString(int _depth, char _axiom, char _axiom1, std::string _rule, std::string _rule1);
};
#endif
