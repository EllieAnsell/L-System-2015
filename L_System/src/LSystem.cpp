#include "LSystem.h"

#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <iomanip>
#include <cmath>
#include <sstream>

//----------------------------------------------------------------------------------------------------------------------
/// @brief  pi for converting degrees to radians
//----------------------------------------------------------------------------------------------------------------------
const static float pi = M_PI;

//----------------------------------------------------------------------------------------------------------------------
LSystem::LSystem()
{;}

//----------------------------------------------------------------------------------------------------------------------
// function to get m_finalRule to be converted into the vector
std::string LSystem::loadString(int _depth, char _axiom, char _axiom1, std::string _rule, std::string _rule1)
{
  // load and return string from stringmanipulator class
  stringManipulator = new StringManipulator();
  stringManipulator->addRule(_axiom, _axiom1, _rule, _rule1);
  std::string startString(1, _axiom);
  m_finalRule = stringManipulator->createString(_depth, startString);
  return m_finalRule;
}

//----------------------------------------------------------------------------------------------------------------------
// Funtion for converting the string to vectors and returning the resulting list to be drawn by plant.h
void LSystem::buildVectorList(float _height, ngl::Vec3 _angleIncrement, int _depth, ngl::Vec3 _sunPos, char _axiom, char _axiom1, std::string _rule, std::string _rule1, int _smoothLevel, float _sunIntensity, float _wind, ngl::Vec3 _windVec, int _frames, int _hitCount)
{
    // Adjust the inputs relative the the amount of rain hits
    _height+=(_hitCount/100);
    _sunPos.m_y+=(_hitCount/20);
    _sunPos.m_x+=(_hitCount/150);
    _sunIntensity+=(_hitCount/20);
    _depth+=(_hitCount/80);
    _angleIncrement.m_x+=(_hitCount/10);
    _angleIncrement.m_y+=(_hitCount/10);
    _angleIncrement.m_z+=(_hitCount/10);

    // load the final rule
    m_finalRule = loadString(_depth, _axiom, _axiom1, _rule, _rule1);
    // creating stem of the plant, and empty leaf vector.
    ngl::Vec3 vector=(0,0,0), leafVector = (0,0,0);
    m_vectorList.push_front(vector);
    // Edits the height and theoretical sun position so the plant wilts
    vector.m_y = _height;
    m_vectorList.push_front(vector);

    // angle is appended to angleList to specify angle for circle rotation when built in drawPlant.cpp
    // randWind used to change the intensity of wind relative to the string list
    // branch is appended to branchList to query where the branch ends in drawPlant.cpp
    float radius = _height/10;
    float previousRadius;
    float randWind;
    int branch;

    // create empty vector lists
    std::vector <ngl::Vec3> vectorList, branchList_, leafList_;
    std::vector <float> angleList;
    ngl::Vec3 angle = (0.0,0.0,0.0), tempAngle;
    ngl::Vec3 temp, previousVector;

    // direction vector stores distance between tendPoint and the branches vector
    ngl::Vec3 dir(0,0,0);

    // iterates through the list of strings and replaces them with vectors, records where the branch ends and where there's a leaf
    for(unsigned int i=0; i<=stringManipulator->m_finalRule.length(); ++i)
    {
      char a =stringManipulator->m_finalRule[i];
      // when branch = 0, this means the vector isn't at the end of a branch.
      branch=0;

      if (a=='F' || a=='A' || a=='B')
      {
        // smoothLevel added to slightly 'smooth' the tree by slightly increasing rotation.
        angle.m_x+=_smoothLevel*0.5;
        angle.m_z+=_smoothLevel*0.5;

        // builds a vertical starting branch with height _height.
        vector.m_x = 0.0;
        vector.m_y = _height;
        vector.m_z = 0.0;

        // Rotation in the x axis
        temp = vector;
        vector.m_y = temp.m_y*cos(angle.m_x*pi/180) - temp.m_z*sin(angle.m_x*pi/180);
        vector.m_z = temp.m_y*sin(angle.m_x*pi/180) + temp.m_z*cos(angle.m_x*pi/180);
        // Rotation in the Y axis
        temp = vector;
        vector.m_x = temp.m_x*cos(angle.m_y*pi/180) + temp.m_z*sin(angle.m_y*pi/180);
        vector.m_z = -temp.m_x*sin(angle.m_y*pi/180) + temp.m_z*cos(angle.m_y*pi/180);
        // Rotation in the Z axis
        temp = vector;
        vector.m_x = temp.m_x*cos(angle.m_z*pi/180) - temp.m_y*sin(angle.m_z*pi/180);
        vector.m_y = temp.m_x*sin(angle.m_z*pi/180) + temp.m_y*cos(angle.m_z*pi/180);

        // this branch vector is added to the previous vector
        if (m_vectorList.size()>1)
        {
          vector+=m_vectorList[0];
        }

        // Finding the direction vector between tendPoint (the sun's position) and the branch vector
        dir.m_x = -_sunPos.m_x - vector.m_x;
        dir.m_y = -_sunPos.m_y - vector.m_y;
        dir.m_z = -_sunPos.m_z - vector.m_z;

        // Normalising the direction vector to find its direction
        int hyp = sqrt(dir.m_x*dir.m_x + dir.m_y * dir.m_y + dir.m_z * dir.m_z);
        dir.m_x /=hyp;
        dir.m_y /=hyp;
        dir.m_z /=hyp;

        // Adding the direction vector and multiplying by sun intensity so the branch tends towards this point quickly
        vector.m_x += dir.m_x * -_sunIntensity;
        vector.m_z += dir.m_z * -_sunIntensity;

        // This queries whether the branch is above a level threshold
        if (vector.m_y<2)
        {
          vector.m_y += dir.m_y * -_sunIntensity+1;
        }
        else if(vector.m_y>2)
        {
          vector.m_y += dir.m_y * -_sunIntensity;
        }

        // this queries whether the branch enters the bounding box for the pot
        if (i>1 && vector.m_y<5 && vector.m_x<5 && vector.m_z<5)
        {
          vector.m_y += dir.m_y * -_sunIntensity+1;
          vector.m_x += dir.m_x * -_sunIntensity+2;
          vector.m_z += dir.m_z * -_sunIntensity+2;

        }

        // Adds wind to branches using a sine wave algorithm with the number of frames.
        if (i>1 && _wind>0)
        {
          randWind = (cos(_frames)*i);
          //  calculating the wind direction amount: sin(time*frequency)*amplitude
          _windVec.m_x+=(sin(_frames*(_wind*0.02)))*(0.1);
          _windVec.m_y+=(cos(_frames*(_wind*0.02)))*(0.1);
          _windVec.m_z+=(sin(_frames*(_wind*0.02)))*(0.1);
          // normalising to just have the direction
          _windVec.normalize();
          vector.m_x+=_windVec.m_x;
          vector.m_y+=_windVec.m_y;
          vector.m_z+=_windVec.m_z;
        }

        // appends to front of deque lists
        radius -=0.05;
        m_branchEnd.push_front(branch);
        m_vectorList.push_front(vector);
        m_angleList.push_front(angle);

        // records previous vector to be used for making branches
        previousRadius = radius;
        previousVector = vector;
      }

      // increasing/decreasing rotation
      else if (a=='+')
      {
        angle.m_x+=_angleIncrement.m_x;
        angle.m_y+=_angleIncrement.m_y;
        angle.m_z+=_angleIncrement.m_z;
      }
      else if (a=='-')
      {
        angle.m_x-=_angleIncrement.m_x;
        angle.m_y-=_angleIncrement.m_y;
        angle.m_z-=_angleIncrement.m_z;

      }

      // records previous vector for branching
      else if (a=='[')
      {
        angleList.push_back(previousRadius);
        vectorList.push_back(previousVector);
      }
      // pops back vectorList to draw from the previous vector and records that the tree branches here. Records previous vector here to draw a leaf where the branch ends.
      else if (a==']')
      {
        m_leafList.push_front(previousVector);
        vector = vectorList.back();
        if (vectorList.size()>0)
        {
          vectorList.pop_back();
          branch = 1;
          m_branchEnd.push_front(branch);
          m_vectorList.push_front(vector);
          radius-=0.05;
          m_angleList.push_front(angle);
        }
      }
      // records the branch ends at the tip of the plant
      if (i==stringManipulator->m_finalRule.length()-1)
      {
        branch =1;
        m_branchEnd.push_front(branch);
      }
    }
}
