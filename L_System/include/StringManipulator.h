#ifndef __L_LIBRARY_H__
#define __L_LIBRARY_H__

#include <iostream>
#include <string>
#include <vector>
//----------------------------------------------------------------------------------------------------------------------
/// @file StringManipulator.h
/// @author Ellie Ansell
/// @version 3.0
/// @date 09/05/15
/// @class  StringManipulator.cpp
/// @brief  input axioms and rules here to generate a string
//----------------------------------------------------------------------------------------------------------------------
class StringManipulator
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  constructor for the StringManipulator class
  //----------------------------------------------------------------------------------------------------------------------
  StringManipulator();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  destructor for the StringManipulator class
  //----------------------------------------------------------------------------------------------------------------------
  ~StringManipulator();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  final rule is the resulting string to be passed to lsystem class to be translated into vectors
  //----------------------------------------------------------------------------------------------------------------------
  std::string  m_finalRule;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  method to load axiom and rule to m_axiom and m_rule
  /// @param[in]  _axiom the input axiom to build the string
  /// @param[in]  _axiom1 the input axiom to build the string
  /// @param[in]  _rule the input rule to build the string
  /// @param[in]  _rule1 the input rule to build the string
  //----------------------------------------------------------------------------------------------------------------------
  void addRule(char _axiom, char _axiom1, std::string _repRule, std::string _repRule1);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  returns the built string to lsystem class
  /// @param[in] _depth the depth of the initial string
  /// @param[in] _inputString the first input string to start building the final string
  //----------------------------------------------------------------------------------------------------------------------
  std::string createString(int _depth, std::string _inputString);
private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  vector of characters for axiom used to build the string
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<char> m_axiom;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  vector of strings of the rules used to replace the axiom characters
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<std::string> m_replaceRule;
};
#endif
