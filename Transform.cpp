// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    mat3 ret;
    float radians = degrees*(pi/180);
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.  
    vec3 normAxis = normalize(axis);
    mat3 term1 = glm::mat3(cos(radians),0,0,0,cos(radians),0,0,0,cos(radians));

    mat3 term2 = mat3((1-cos(radians))*normAxis.x*normAxis.x, (1-cos(radians))*normAxis.x*normAxis.y, (1-cos(radians))*normAxis.x*normAxis.z,
    (1-cos(radians))*normAxis.x*normAxis.y, (1-cos(radians))*normAxis.y*normAxis.y, (1-cos(radians))*normAxis.y*normAxis.z,
    (1-cos(radians))*normAxis.x*normAxis.z, (1-cos(radians))*normAxis.y*normAxis.z, (1-cos(radians))*normAxis.z*normAxis.z);

    mat3 term3 = mat3(0, sin(radians)*normAxis.z, sin(radians)*-normAxis.y, sin(radians)*-normAxis.z, 0, sin(radians)*normAxis.x, sin(radians)*normAxis.y, sin(radians)*-normAxis.x, 0);

    ret = term1 + term2 + term3;
    return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.  
    mat3 toRotateBye = rotate(degrees, -up);
    /*float radians = degrees*(pi/180);
    mat3 toRotateBye = glm::mat3(cos(radians), 0, -sin(radians),0, 1, 0, sin(radians), 0, cos(radians));
    */
    vec3 u = toRotateBye[0];
    vec3 v = toRotateBye[1];
    vec3 w = toRotateBye[2];
    //eye = eye * toRotateBye;
    //up = up * toRotateBye;
    eye = vec3(glm::dot(eye, u), glm::dot(eye, v), glm::dot(eye, w));
    up = vec3(glm::dot(up, u), glm::dot(up, v), glm::dot(up, w));
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    vec3 rotator = normalize(eye);
    vec3 newAxis = cross(rotator, -up);
    mat3 toRotateBye = rotate(degrees, newAxis);
    //mat3 toRotateBye = glm::mat3(1, 0, 0, 0, cos(radians), sin(radians), 0, -sin(radians), cos(radians));
    vec3 u = toRotateBye[0];
    vec3 v = toRotateBye[1];
    vec3 w = toRotateBye[2];
    eye = vec3(glm::dot(eye, u), glm::dot(eye, v), glm::dot(eye, w));
    up = vec3(glm::dot(up, u), glm::dot(up, v), glm::dot(up, w));
    // Likely the same as in HW 1.  
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    vec3 w = glm::normalize(eye);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::cross(w,u);
    vec3 translate = vec3(-eye.x, -eye.y, -eye.z);
    mat3 rotate(u.x, v.x, w.x, u.y, v.y, w.y, u.z, v.z, w.z);
    rotate = transpose(rotate);
    mat4 ret(u.x,v.x,w.x,0,u.y, v.y, w.y, 0, u.z, v.z, w.z, 0, glm::dot(rotate[0],translate), glm::dot(rotate[1],translate), glm::dot(rotate[2],translate), 1);
    return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    float toTan = (fovy/2)*(pi/180);
    float d = 1/tan(toTan);
    float a = -(zFar+zNear)/(zFar-zNear);
    float b = -(2*zFar*zNear)/(zFar-zNear);
    ret = mat4(d/aspect, 0, 0, 0, 0, d, 0, 0, 0, 0, a, -1, 0, 0, b, 0);
    //ret = transpose(ret);
    //ret = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret;
    ret = mat4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
    //ret = transpose(ret);
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret;
    ret = mat4(1,0,0,0,0,1,0,0,0,0,1,0,tx,ty,tz,1);
    //ret = transpose(ret);
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
