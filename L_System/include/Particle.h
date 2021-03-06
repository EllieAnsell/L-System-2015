#ifndef PARTICLE_H__
#define PARTICLE_H__

#include <ngl/Vec3.h>
#include <ngl/Colour.h>
#include <ngl/Text.h>
//----------------------------------------------------------------------------------------------------------------------
/// @file Particle.h
/// @author Ellie Ansell, adapted from Jon Macey's code
/// @version 3.0
/// @date 09/05/15
/// @class  Particle.cpp
/// @brief particle class to update particles positions for rain
//----------------------------------------------------------------------------------------------------------------------
// pre-declare weather class
class Weather;
class Particle
{
public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief constructor
  /// @param[in] _pos the initial starting vector of the rain particles
  //----------------------------------------------------------------------------------------------------------------------
  Particle(ngl::Vec3 _pos, Weather *_emitter );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to calculate the particles' new positions
  /// @param[in] _rainOnOff inputs whether the rain is on or off
  /// @param[in] _confetti inputs whether the confetti is on or off
  //----------------------------------------------------------------------------------------------------------------------
  int update(bool _rainOnOff, bool _confetti);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a method to draw the particle
  /// @param[in] _confetti query for confetti
  //----------------------------------------------------------------------------------------------------------------------
  void draw(bool _confetti) const;
  private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the curent particle position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_pos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the original particle position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_origin;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the start position
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_startPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the direction of the vectors
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_dir;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the colour of the particles
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Colour m_colour;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the hit count for rain hitting the pot
  //----------------------------------------------------------------------------------------------------------------------
  int m_hitCount;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to emitter
  //----------------------------------------------------------------------------------------------------------------------
  const Weather *m_emitter;
};
#endif
