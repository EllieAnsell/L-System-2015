#include "drawPlant.h"

const static float pi = M_PI;

//----------------------------------------------------------------------------------------------------------------------
// Constructor to drawPlant
drawPlant::drawPlant()
{;}

//----------------------------------------------------------------------------------------------------------------------
// initializes grid
void drawPlant::initializeObjects()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use(getShaderName());

  ngl::VAOPrimitives::instance()->createLineGrid("grid", 20,20,20);
  ngl::VAOPrimitives::instance()->draw("grid");

  shader->setShaderParam4f("Colour", 1.0, 0.0, 0.0, 1.0);
  ngl::VAOPrimitives::instance()->createCylinder("axisXRed", 0.2, 5, 10, 10);
  m_transform.setRotation(0, 90, 0);
  m_transform.setPosition(10, 0, 10);
  loadMatricesToShader();
  ngl::VAOPrimitives::instance()->draw("axisXRed");

  shader->setShaderParam4f("Colour", 0.0, 0.0, 1.0, 1.0);
  ngl::VAOPrimitives::instance()->createCylinder("axisZBlue", 0.2, 5, 10, 10);
  m_transform.addRotation(0, -90, 0);
  loadMatricesToShader();
  ngl::VAOPrimitives::instance()->draw("axisZBlue");

  shader->setShaderParam4f("Colour", 0.0, 1.0, 0.0, 1.0);
  ngl::VAOPrimitives::instance()->createCylinder("axisYGreen", 0.2, 5, 10, 10);
  m_transform.addRotation(90, 0, 0);
  loadMatricesToShader();
  ngl::VAOPrimitives::instance()->draw("axisYGreen");
  m_transform.reset();
}

//----------------------------------------------------------------------------------------------------------------------
// Function to draw leaves
void drawPlant::drawFoliage(const int _option)
{
  if (_option == 0)
  {
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    shader->use(getShaderName());
    shader->setShaderParam4f("Colour", 0.7, 0.5, 0.5, 1.0);
    ngl::VAOPrimitives::instance()->createSphere("sphere", .3, 20);
    m_transform.setScale(2,2,2);
    loadMatricesToShader();
    ngl::VAOPrimitives::instance()->draw("sphere");
  }
}

//----------------------------------------------------------------------------------------------------------------------
// Function to create circle for cylinders of the branch
ngl::Vec3 drawPlant::circleData(ngl::Vec3 _startPoint, float _radius, const float _angle, ngl::Vec3 _circleRotate)
{
  ngl::Vec3 point;
  ngl::Vec3 temp;

  // Using trigonometry to find the point's x and z position around the circle of the cylinder.
  point.m_x = _radius*sin(_angle*pi/180);
  point.m_z = _radius*cos(_angle*pi/180);

  point += _startPoint;
  return point;
}

//----------------------------------------------------------------------------------------------------------------------
// vertData to build the points
struct vertData
{
  GLfloat u;
  GLfloat v;
  GLfloat nx;
  GLfloat ny;
  GLfloat nz;
  GLfloat x;
  GLfloat y;
  GLfloat z;
};


//----------------------------------------------------------------------------------------------------------------------
// main function to draw the plant
void drawPlant::buildPlant(float _radius, const float _height, std::deque <ngl::Vec3> _vectorList, std::deque <int> _branchEnd, std::deque <ngl::Vec3> _leafList, std::deque <ngl::Vec3> _angleList, const int _folliage, const int _wind)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->setShaderParam4f("Colour", 0.7, 0.5, 0.7, 1.0);
  m_vao =  ngl::VertexArrayObject::createVOA(GL_TRIANGLES);

  ngl::Vec3 circleRotate;
  float segments = 6.0;
  float angle = 360/segments;
  float rotLeaf;
  ngl::Vec3 point[6];
  ngl::Vec3 rotationVector;
  ngl::Vec3 centerPoint(0, 1,0);
  std::vector <vertData> data;
  vertData d;

  int rotateSwitch=0;
  _radius = 0.3;

  if (_folliage==1)
  {
    for (unsigned int i=0; i<_leafList.size(); ++i)
    {
      for (unsigned int j=0; j<_folliage; ++j)
      {
        m_transform.setPosition(_leafList[i]);
        drawFoliage(0);
        m_transform.setScale(0.2, 0.2, 0.2);
        m_transform.setPosition(_leafList[i].m_x+(j/10), _leafList[i].m_y+(i*j/30), _leafList[i].m_z+(j/10));
        drawFoliage(0);
      }
    }
  }
  for (unsigned int j=0; j<_vectorList.size(); ++j)
  {
    if (j>1)
    {

      ngl::Vec3 vec;
      vec = _vectorList[j]-_vectorList[j-1];
      // find the angle between vec and the x axis
      ngl::Vec3 xAxis(1,0,0);
      float dot = xAxis.dot(vec);
      float modulus = xAxis.length()*vec.length();
      circleRotate.m_x = ((acos((dot/modulus))));
    }
    for (unsigned int i=0;i<segments;++i)
    {
      if (_branchEnd[j]==1)
      {
        data.end();
      }

      // Calculating the points of data when at the base of the tree (j==0)
      else if (j==0)
      {

        // Here I would calculate the radius of the branch if the rotation had worked, with: _radius = 1-0.1*(_vectorList.size()-j+1);
        // Using the circleDada function to find the point of the vector on the circle
        point[0] = circleData((0.0,0.0,0.0), _radius,(angle*i), circleRotate);
        d.x = point[0].m_x;
        d.y = point[0].m_y;
        d.z = point[0].m_z;
        // pushing the point data onto the VAO list
        data.push_back(d);

        point[1] = circleData((0.0,0.0,0.0), _radius, (angle*(i+1)), circleRotate);
        d.x = point[1].m_x;
        d.y = point[1].m_y;
        d.z = point[1].m_z;
        data.push_back(d);

        point[2] = circleData(_vectorList[j], _radius, (angle*(i+1)), circleRotate);
        d.x = point[2].m_x;
        d.y = point[2].m_y;
        d.z = point[2].m_z;
        data.push_back(d);

        point[3] = circleData(_vectorList[j], _radius,(angle*(i)), circleRotate);
        d.x = point[3].m_x;
        d.y = point[3].m_y;
        d.z = point[3].m_z;
        data.push_back(d);

        point[4] = circleData((0.0,0.0,0.0), _radius, (angle*(i)), circleRotate);
        d.x = point[4].m_x;
        d.y = point[4].m_y;
        d.z = point[4].m_z;
        data.push_back(d);

        point[5] = circleData(_vectorList[j], _radius, (angle*(i+1)), circleRotate);
        d.x = point[5].m_x;
        d.y = point[5].m_y;
        d.z = point[5].m_z;
        data.push_back(d);
      }
      // When j>0, I can draw between the current and previous vectors.
      else if (j>0)
      {
        //_radius = 1-0.1*(_vectorList.size()-j+2);
        rotateSwitch = 0;
        point[0] = circleData(_vectorList[j-1], _radius,(angle*i), circleRotate);
        d.x = point[0].m_x;
        d.y = point[0].m_y;
        d.z = point[0].m_z;
        data.push_back(d);

        point[1] = circleData(_vectorList[j-1], _radius, (angle*(i+1)), circleRotate);
        d.x = point[1].m_x;
        d.y = point[1].m_y;
        d.z = point[1].m_z;
        data.push_back(d);

        //_radius = 1-0.1*(_vectorList.size()-j);
        rotateSwitch = 1;
        point[2] = circleData(_vectorList[j], _radius, (angle*(i+1)), circleRotate);
        d.x = point[2].m_x;
        d.y = point[2].m_y;
        d.z = point[2].m_z;
        data.push_back(d);

        point[3] = circleData(_vectorList[j], _radius,(angle*(i)), circleRotate);
        d.x = point[3].m_x;
        d.y = point[3].m_y;
        d.z = point[3].m_z;
        data.push_back(d);

        //_radius = 1-0.1*(_vectorList.size()-j+2);
        rotateSwitch = 0;
        point[4] = circleData(_vectorList[j-1], _radius, (angle*(i)), circleRotate);
        d.x = point[4].m_x;
        d.y = point[4].m_y;
        d.z = point[4].m_z;
        data.push_back(d);

        //_radius = 1-0.1*(_vectorList.size()-j);
        rotateSwitch = 1;
        point[5] = circleData(_vectorList[j], _radius, (angle*(i+1)), circleRotate);
        d.x = point[5].m_x;
        d.y = point[5].m_y;
        d.z = point[5].m_z;
        data.push_back(d);
      }
    }
  }
  // Binding the VAO to make it active
  m_vao->bind();
  // Finding the size of the data
  unsigned int buffSize=data.size();
  // Allocate the data using tri's
  m_vao->setData(buffSize*sizeof(vertData),data[0].u);
  m_vao->setVertexAttributePointer(0,3,GL_FLOAT,sizeof(vertData),5);
  m_vao->setVertexAttributePointer(1,2,GL_FLOAT,sizeof(vertData),0);
  m_vao->setVertexAttributePointer(2,3,GL_FLOAT,sizeof(vertData),2);
  m_vao->setNumIndices(buffSize);
  // unbind the VAO and set 0 to be the active array
  m_vao->unbind();
}

void drawPlant::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform.getMatrix()*m_mouseGlobalTX;
  MV=M*getCam()->getViewMatrix();
  MVP=MV*getCam()->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MVP",MVP);
}

void drawPlant::drawScene(ngl::Vec3 _sunPos, const int _sunPlantDist, const float _intensitySun, bool _toggleSun)
{
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::Transformation trans;
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  shader->use(getShaderName());
  // setting the colour of the branches
  shader->setShaderParam4f("Colour", 0.5, 0.2, 0.1, 1.0);
  loadMatricesToShader();
  // if there's a sun, the branch colour lightens/darkens in accordance to the distance between the sun and plant.
  if (_toggleSun==true)
  {
    shader->setShaderParam4f("Colour", 0.5-(_sunPlantDist*0.01), 0.2-(_sunPlantDist*0.01), 0.1-(_sunPlantDist*0.01), 1.0);
  }
  m_transform.reset();
  loadMatricesToShader();
  // Binding the VAO to make it active
  m_vao->bind();
  // Drawing the bound VAO
  m_vao->draw();
  // Unbinding the VAO.
  m_vao->unbind();

  // if the sun is active, draw a sun. The value of intensity changes scale.
  if (_toggleSun==true)
  {
    m_transform.reset();
    // creating VAO primative sphere
    ngl::VAOPrimitives::instance()->createSphere("sphereSun", 0.3, 20);
    m_transform.setScale(5,5,5);
    //changing the scale relative to the sun intensity
    m_transform.addScale(1+_intensitySun*2, 1+_intensitySun*2, 1+_intensitySun*2);
    shader->setShaderParam4f("Colour", 0.48+_intensitySun, 0.43+_intensitySun, 0.30+_intensitySun, 1.0);
    // Set the sun's position
    m_transform.setPosition(_sunPos.m_x, _sunPos.m_y, _sunPos.m_z);
    loadMatricesToShader();
    prim->draw("sphereSun");
    loadMatricesToShader();
    // Resetting the transformations
    m_transform.reset();
  }
}
