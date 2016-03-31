#include "Particle.h"
#include "Weather.h"

#include <ngl/Random.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>

//----------------------------------------------------------------------------------------------------------------------
// Initialise the initial particles positions
// Modified from Jon Macey's code
Particle::Particle(ngl::Vec3 _pos, Weather *_emitter )
{
  m_pos=_pos;
  m_hitCount = 0;
  m_origin=_pos;
  ngl::Random *rand=ngl::Random::instance();
  m_dir=rand->getRandomNormalizedVec3();
  m_colour=rand->getRandomColour();
  m_emitter = _emitter;
  }
//----------------------------------------------------------------------------------------------------------------------
// Method to update the particles positions
// Modified from Jon Macey's code
int Particle::update(bool _rainOnOff, bool _confetti)
{
  int hitCount;
  m_pos.m_y-=m_dir.m_y;

  if(m_pos.m_y<5)
  {
    m_pos=m_origin;
    ++m_hitCount;
    if (_rainOnOff==true && _confetti==false)
    {
      hitCount = 1;
    }
    else if(_rainOnOff==false && _confetti==false)
    {
      hitCount = 0;
    }
    else if(_confetti==true && _rainOnOff==false)
    {
      hitCount = -1;
    }
    return hitCount;
  }
}

//----------------------------------------------------------------------------------------------------------------------
// Method to draw the particle
/// Adapted from Jon Macey's code
void Particle::draw(bool _confetti) const
{
  // get the VBO instance
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::Transformation trans;
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use(m_emitter->getShaderName());

  if (_confetti==true)
  {
    shader->setShaderParam4f("Colour", m_colour.m_r, m_colour.m_g, 0.9, 1.0);
  }
  else if (_confetti==false)
  {
    shader->setShaderParam4f("Colour", 0.2, 0.2, 1-(m_colour.m_b*0.4), 1.0);
  }

  trans.setPosition(m_pos);
  trans.setScale(0.5, 0.5,0.5);
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=trans.getMatrix()*m_emitter->m_mouseGlobalTX;
  MV=M*m_emitter->getCam()->getViewMatrix();
  MVP=MV*m_emitter->getCam()->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
  prim->draw("sphere");
}
