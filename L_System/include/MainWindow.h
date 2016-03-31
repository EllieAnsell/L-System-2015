#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GLWindow.h"

#include <QMainWindow>
//----------------------------------------------------------------------------------------------------------------------
/// @file MainWindow.h
/// @author Jon Macey, adapted by Ellie Ansell
/// @version 3.0
/// @date 09/05/15
/// @class  MainWindow.cpp
/// @brief MainWindow for the gui containing widget functions
//----------------------------------------------------------------------------------------------------------------------
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief constructor for MainWindow
  //----------------------------------------------------------------------------------------------------------------------
    explicit MainWindow(QWidget *parent = 0);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief destructor for MainWindow
  //----------------------------------------------------------------------------------------------------------------------
    ~MainWindow();

private slots:

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief add rules when clicked
  //----------------------------------------------------------------------------------------------------------------------
  void on_m_addRules_clicked();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief toggle confetti true/false
  /// @param[in] checked true/false
  //----------------------------------------------------------------------------------------------------------------------
  void on_m_confetti_toggled(bool checked);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief toggle rain true/false
  /// @param[in] checked true/false
  //----------------------------------------------------------------------------------------------------------------------
  void on_m_toggleRain_toggled(bool checked);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief slide the camera when value is changed
  /// @param[in] value the value input from the slider
  //----------------------------------------------------------------------------------------------------------------------
  void on_m_camSlideY_valueChanged(int value);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief toggle wind true/false
  /// @param[in] wind on/off
  //----------------------------------------------------------------------------------------------------------------------
  void on_m_toggleWind_toggled(bool checked);

  void on_m_saveValues_clicked();

private:
  Ui::MainWindow *m_ui;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief for openGL widget
  //----------------------------------------------------------------------------------------------------------------------
  GLWindow *m_gl;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the keyPressEvent function
  //----------------------------------------------------------------------------------------------------------------------
  void keyPressEvent(QKeyEvent *_event);
};
#endif // MAINWINDOW_H
