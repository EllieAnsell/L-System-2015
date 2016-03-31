#include "StringManipulator.h"

#include <string>
#include <map>

StringManipulator::StringManipulator() :    m_axiom(std::vector<char>(0)),
                                            m_replaceRule(std::vector<std::string>(0)),
                                            m_finalRule("A")
{;}
//----------------------------------------------------------------------------------------------------------------------
// Function to add rules to m_axiom and m_replaceRule lists
void StringManipulator::addRule(char _axiom, char _axiom1, std::string _repRule, std::string _repRule1)
{
  // append _axiom and _repRule to class members m_axiom and m_replaceRule
  m_axiom.push_back(_axiom);
  m_axiom.push_back(_axiom1);
  m_replaceRule.push_back(_repRule);
  m_replaceRule.push_back(_repRule1);

}

//----------------------------------------------------------------------------------------------------------------------
// Function to create string using recursion
std::string StringManipulator::createString(int _depth, std::string _inputString)
{
  // create start and temporary strings to be used to build the final string
  std::string str = _inputString;
  std::string temp;
  // create variable characters for the axioms for easy comparison
  char r1=m_axiom[0];
  char r2=m_axiom[1];

  int counter = 0;
  int j=0;
  while(counter < _depth)
  {
    // map used to compare rules to axioms
    std::map<char,std::string> my_map;

    my_map[r1]=m_replaceRule[0];
    my_map[r2]=m_replaceRule[1];

    j=0;
    temp="";
    // iterates through string and replaces characters using the mapping rules
    for (std::string::iterator it=str.begin(); it!=str.end(); ++it)
    {
      if (str[j]==r1 or str[j]==r2)
      {
        // append replaced string to temp if equal
        temp+=my_map[*it];
      }
      else
      {
        // otherwise add to temp
        temp+=str[j];
      }
    j++;
    }
      counter++;
      // set str to temp so the string can be iterated through again
      str=temp;
    }
    m_finalRule=str;
    //  returns the final rule to lsystem where it will be translated to vector positions
    return m_finalRule;
};

//----------------------------------------------------------------------------------------------------------------------
// Destructor for stringManipulator
StringManipulator::~StringManipulator()
{;}





