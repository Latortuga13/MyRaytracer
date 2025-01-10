/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN vec3 normal;
EXTERN vec3 cnormal;
EXTERN vec3 tnormal;
EXTERN int depth;
EXTERN float shortestdist;
EXTERN int shortestIndex;
EXTERN mat4 mytranspose;
EXTERN mat4 translations;
EXTERN float myt;
EXTERN float myrt;
EXTERN float mycrt;
EXTERN vec3 reflectnorm;
EXTERN vec3 reflectTnorm;
EXTERN vec3 reflectCnorm;
EXTERN vec3 reflectDiffuse;
EXTERN vec3 reflectSpec;
EXTERN vec3 reflectTDiffuse;
EXTERN vec3 reflectTSpec;
EXTERN vec3 reflectCol;
EXTERN vec3 previousadd;

#ifdef MAINPROGRAM 
vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
vec3 up(0.0,1.0,0.0) ; // Initial up position, also for resets
vec3 center(0.0,0.0,0.0) ; // Center look at point 
int amountinit = 5;
float w = 500, h = 500 ; // width and height 
float fovy;
EXTERN vec3 intersectionPoint;
EXTERN vec3 cintersectionPoint;
EXTERN vec3 tintersectionPoint;
float myw, myh;
float myfovy;

#else 
EXTERN float myw, myh;
EXTERN vec3 eyeinit ; 
EXTERN vec3 up ; 
EXTERN vec3 center ; 
EXTERN vec3 intersectionPoint;
EXTERN int amountinit;
EXTERN float w, h ; 
EXTERN float fovy;
EXTERN float myfovy ; 
#endif 

EXTERN bool useGlu; // Toggle use of "official" opengl/glm transform vs user 
EXTERN mat4 projection, modelview; // The mvp matrices
static enum {view, translate, scale} transop ; // which operation to transform 
EXTERN float sx, sy ; // the scale in x and y 
EXTERN float tx, ty ; // the translation in x and y

// Lighting parameter array, similar to that in the fragment shader
const int numLights = 10 ; 
EXTERN float lightposn [4*numLights] ; // Light Positions
EXTERN float lightcolor[4*numLights] ; // Light Colors
EXTERN float lightransf[4*numLights] ; // Lights transformed by modelview
EXTERN int numused;                     // How many lights are used 

// Materials (read from file) 
// With multiple objects, these are colors for each.
EXTERN float ambient[3]; 
//EXTERN float diffuse[3] ; 
EXTERN vec3 diffuse;
EXTERN vec3 specular;
//EXTERN float specular[3] ; 
EXTERN float emission[3] ; 
EXTERN float directional[6];
EXTERN float point[6];
EXTERN float shininess; 
EXTERN float attenuation[3];

// For multiple objects, read from a file.  

EXTERN struct vert {
  float color[3];
  float x;
  float y;
  float z;
} vertex[100];

EXTERN int tricount;
EXTERN struct tri{
  mat4 transform;
  float color[3];
  float emission[3];
  vec3 diffuse;
  vec3 specular;
  vec3 A;
  vec3 B;
  vec3 C;
  vec3 normal;
} tria[100];

EXTERN int circcount;
EXTERN struct sphere {
  mat4 transform;
  float color[3];
  float emission[3];
  vec3 diffuse;
  vec3 specular;
  float x;
  float y;
  float z;
  float radius;
  vec3 normal;
} circ[100];


