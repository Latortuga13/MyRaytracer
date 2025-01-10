//#include "variables.h"
// Readfile definitions 

#include <sstream>
#include <stack>

using namespace std;

void rightmultiply(const mat4 & M, stack<mat4> &transfstack); 
void mySize(float width, float height);
bool readvals (stringstream &s, const int numvals, float * values) ;
void readfile (const char * filename) ;
