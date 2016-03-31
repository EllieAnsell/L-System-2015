#include "Weather.h"

//----------------------------------------------------------------------------------------------------------------------
// Function to randomise the starting vector for the particles and set number of particles
/// Adapted from Jon Macey's simpleParticle demo
Weather::Weather( ngl::Vec3 _pos, int _numParticles )
{
  m_hitCount = 0;
  for (int i=0; i< _numParticles; ++i)
  {
    double randXpos = rand() %5-2.5;
    double randZpos = rand() %5-2.5;
    _pos.m_x =randXpos;
    _pos.m_y =40;
    _pos.m_z =randZpos;
    m_particles.push_back(Particle(_pos, this));
  }
  m_numParticles=_numParticles;
}

//----------------------------------------------------------------------------------------------------------------------
// Method to update each of the particles contained in the system
///Adapted from Jon Macey's simpleParticle demo
int Weather::update(bool _rainOnOff, bool _confetti)
{
  for(int i=0; i<m_numParticles; ++i)
  {
    m_hitCount+=m_particles[i].update(_rainOnOff, _confetti);
    if (m_hitCount<0)
    {
      m_hitCount=1;
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------
// a method to draw all the particles contained in the system
void Weather::draw(bool _rainOnOff, bool _confetti)
{
  m_transform.reset();
  if (_rainOnOff==true || _confetti==true)
  {
    for(int i=0; i<m_numParticles; ++i)
    {
      m_particles[i].draw(_confetti);
    }
  }
}
