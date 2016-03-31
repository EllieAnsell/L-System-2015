#include "MainWindow.h"
#include "ui_MainWindow.h"

//----------------------------------------------------------------------------------------------------------------------
// main window containing funcitons for widgets for the UI
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  // creates image pixmap
  QPixmap pix("obj/clouds.png");
  m_ui->label_pic->setPixmap(pix);
  // create an openGL format and pass to the new GLWidget
  QGLFormat format;
  format.setVersion(4,1);
  format.setProfile( QGLFormat::CoreProfile);

  m_gl=new  GLWindow(format,this);

  m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,2,1);

  // sun widgets
  connect(m_ui->m_toggleSun,SIGNAL(toggled(bool)),m_gl,SLOT(toggleSun(bool)));
  connect(m_ui->m_sunPosX,SIGNAL(valueChanged(double)),m_gl,SLOT(sunPosX(double)));
  connect(m_ui->m_sunPosY,SIGNAL(valueChanged(double)),m_gl,SLOT(sunPosY(double)));
  connect(m_ui->m_sunPosZ,SIGNAL(valueChanged(double)), m_gl,SLOT(sunPosZ(double)));
  connect(m_ui->m_sunIntensity,SIGNAL(valueChanged(double)), m_gl,SLOT(sunIntensity(double)));
  // wind widgets
  connect(m_ui->m_toggleWind,SIGNAL(toggled(bool)),m_gl,SLOT(toggleWind(bool)));
  connect(m_ui->m_windIntensity,SIGNAL(valueChanged(double)), m_gl,SLOT(windIntensity(double)));
  connect(m_ui->m_windDirX,SIGNAL(valueChanged(double)),m_gl,SLOT(windDirX(double)));
  connect(m_ui->m_windDirY,SIGNAL(valueChanged(double)),m_gl,SLOT(windDirY(double)));
  connect(m_ui->m_windDirZ,SIGNAL(valueChanged(double)),m_gl,SLOT(windDirZ(double)));
  // plant widgets
  connect(m_ui->m_depth,SIGNAL(valueChanged(int)),m_gl,SLOT(setDepth(int)));
  connect(m_ui->m_height,SIGNAL(valueChanged(double)),m_gl,SLOT(plantHeight(double)));
  connect(m_ui->m_smoothLevel,SIGNAL(valueChanged(int)),m_gl,SLOT(smoothLevel(int)));
  connect(m_ui->m_toggleLeaf,SIGNAL(toggled(bool)),m_gl,SLOT(toggleLeaf(bool)));
  connect(m_ui->m_angleIncrementX,SIGNAL(valueChanged(double)),m_gl,SLOT(angleIncrementX(double)));
  connect(m_ui->m_angleIncrementY,SIGNAL(valueChanged(double)),m_gl,SLOT(angleIncrementY(double)));
  connect(m_ui->m_angleIncrementZ,SIGNAL(valueChanged(double)),m_gl,SLOT(angleIncrementZ(double)));
  // rain widgets
  connect(m_ui->m_toggleRain,SIGNAL(toggled(bool)),m_gl,SLOT(toggleRain(bool)));
  connect(m_ui->m_confetti,SIGNAL(toggled(bool)),m_gl,SLOT(toggleConfetti(bool)));
  // camera slider
  connect(m_ui->m_camSlideY,SIGNAL(valueChanged(int)),m_gl,SLOT(camSlideY(int)));
}

// Destructor function for main window
MainWindow::~MainWindow()
{
    delete m_ui;
}

//----------------------------------------------------------------------------------------------------------------------
///@brief switch to toggle line on and off
//----------------------------------------------------------------------------------------------------------------------
int lineSwitch=0;

//----------------------------------------------------------------------------------------------------------------------
// function to press key event
void MainWindow::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quit
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  case Qt::Key_D: {m_gl->m_sunPos.m_x+=1;} break;
  case Qt::Key_A: {m_gl->m_sunPos.m_x-=1;} break;
  case Qt::Key_S:{m_gl->m_sunPos.m_z+=1;} break ;
  case Qt::Key_W:{m_gl->m_sunPos.m_z-=1;} break ;
  case Qt::Key_L:
    {
      if (lineSwitch==0)
      {
          glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); lineSwitch=1;
      }
      else if(lineSwitch==1)
      {
          glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); lineSwitch=0;
      }
    } break;
  default : break;
  }
  // finally update the GLWindow and re-draw
  //if (isExposed())
  update();
}

//----------------------------------------------------------------------------------------------------------------------
// function to get variables from the UI and return them to the GLWindow
void MainWindow::on_m_addRules_clicked()
{

  QString text;
  text = m_ui->m_axiom->text();
  std::string textAxiom = text.toStdString();
  char axiom = textAxiom[0];
  m_gl->getAxiomString(axiom);

  QString text1;
  text1 = m_ui->m_rule->text();
  std::string textRule = text1.toStdString();
  m_gl->getRuleString(textRule);

  QString text2;
  text2 = m_ui->m_axiom2->text();
  std::string textAxiom2 = text2.toStdString();
  char axiom2 = textAxiom2[0];
  m_gl->getAxiom2String(axiom2);

  QString text3;
  text3 = m_ui->m_rule2->text();
  std::string textRule2 = text3.toStdString();
  m_gl->getRule2String(textRule2);

  m_ui->m_startingAxiom->setText(QString("Starting Axiom: %1").arg(text));
  m_ui->m_axiomToRule->setText(QString("%1 --> %2").arg(text).arg(text1));
  m_ui->m_axiomToRule1->setText(QString("%1 --> %2").arg(text2).arg(text3));

}

//----------------------------------------------------------------------------------------------------------------------
// function to toggle rain off when confetti is checked
void MainWindow::on_m_confetti_toggled(bool checked)
{
  m_ui->m_toggleRain->setChecked(false);
}

//----------------------------------------------------------------------------------------------------------------------
// function to toggle confetti off when rain is checked
void MainWindow::on_m_toggleRain_toggled(bool checked)
{
  m_ui->m_confetti->setChecked(false);
}

//----------------------------------------------------------------------------------------------------------------------
// camera slider
void MainWindow::on_m_camSlideY_valueChanged(int value)
{
  m_gl->m_camSlideY = value;
}

//----------------------------------------------------------------------------------------------------------------------
// Toggle wind on/off
void MainWindow::on_m_toggleWind_toggled(bool checked)
{
  m_gl->m_windOnOff = checked;
  if (checked ==false)
  {
    m_ui->m_windIntensity->setValue(0);
  }
  else if (checked ==true)
  {
    m_ui->m_windIntensity->setValue(1);
  }
}

//----------------------------------------------------------------------------------------------------------------------
// Save to config file
void MainWindow::on_m_saveValues_clicked()
{
    m_gl->fileOut();
}
