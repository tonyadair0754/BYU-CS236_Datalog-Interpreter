#include "interpreter.h"

void Interpreter::interpret()
{
  evaluateSchemes();
  evaluateFacts();
  evaluateRules();
  evaluateQueries();
}