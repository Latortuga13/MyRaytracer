#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h"
#include "packages/glm.0.9.7.1/build/native/include/glm/glm.hpp"
#include "packages/glm.0.9.7.1/build/native/include/glm/gtc/matrix_transform.hpp"

using namespace std;

#include "variables.h"
#include "readfile.h"


void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    T = T * M; 
}

void mySize(float width, float height){
    w = width;
    h = height;
}

bool readvals(stringstream &s, const int numvals, float* values) 
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        //cout<< values[i] << "\n";
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

void readfile(const char* filename){
    string str, cmd; 
    ifstream in;
    in.open(filename); 
    circcount = 0;
    tricount = 0;
    int vertcount = 0;

    if (in.is_open()) {
        //std::cout << "we reading \n"; 

        // I need to implement a matrix stack to store transforms. 
        stack <mat4> transfstack; 
        transfstack.push(mat4(1.0));  // identity

        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                stringstream s(str);

                s >> cmd; 
                //cout << cmd << endl;
                int i;

                float values[10]; // Position and color for light, colors for others
                                    // Up to 10 params for cameras.  
                bool validinput; // Validity of input 
                //object * obj = &(objects[numobjects]); 
                //std::cout<< cmd << '\n';

                if (cmd == "ambient") {
                    //std::cout << "we in ambient \n";
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            //cout<< ambient[i];
                            //std::cout << "we in ambient \n";
                            ambient[i] = values[i];
                        }
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        diffuse.x = values[0];
                        diffuse.y = values[1];
                        diffuse.z = values[2];
                        //cout << values[0] << values[1] << values[2] << endl;
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        specular.x = values[0];
                        specular.y = values[1];
                        specular.z = values[2];
                    }
                } else if (cmd == "directional") {
                    validinput = readvals(s, 6, values); 
                    if (validinput) {
                        for (i = 0; i < 6; i++) {
                            directional[i] = values[i]; 
                        }
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            emission[i] = values[i]; 
                        }
                    }
                } else if(cmd == "shininess"){
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
                        shininess = values[0]; 
                    }
                }else if (cmd == "point") {
                    validinput = readvals(s,6,values); 
                    if (validinput) { 
                        for (i = 0; i < 6; i++) {
                            point[i] = values[i]; 
                            //cout << point[i] << endl;
                        }
                    } 
                    //cout << point[3] << point[4] << point[5];
                } else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {
                        eye = vec3(values[0], values[1], values[2]);
                        up = /*normalize(*/vec3(values[6], values[7], values[8]);//);
                        center = vec3(values[3], values[4], values[5]);
                        fovy = values[9];
                        myfovy = values[9] * (pi/180.0f);

                    }
                }

                //OBJECTS

                else if (cmd == "sphere") {
                    validinput = readvals(s,4,values); 
                    if (validinput) { 

                            circ[circcount].x = values[0];
                            circ[circcount].y = values[1];
                            circ[circcount].z = values[2];
                            circ[circcount].radius = values[3];
                            circ[circcount].color[0] = ambient[0] ;
                            circ[circcount].color[1] = ambient[1];
                            circ[circcount].color[2] = ambient[2];
                            circ[circcount].emission[0] = emission[0];
                            circ[circcount].emission[1] = emission[1];
                            circ[circcount].emission[2] = emission[2];
                            circ[circcount].diffuse.x = diffuse.x;
                            circ[circcount].diffuse.y = diffuse.y;
                            circ[circcount].diffuse.z = diffuse.z;
                            circ[circcount].specular.x = specular.x;
                            circ[circcount].specular.y = specular.y;
                            circ[circcount].specular.z = specular.z;

                            circ[circcount].transform = transfstack.top(); 
                            circcount++;

                            for(int i = 0; i< 4; i++){
                                for(int j = 0; j< 4; j++){
                                    //cout << circ[circcount].transform[i][j] << endl;
                                }
                            }
                    }
                    
                } else if (cmd == "vertex"){
                    //cout << "i see vertex command \n";
                    validinput = readvals(s,3,values); 
                    //cout << "value: " << values[0] << '\n';
                    //if (validinput) { 
                    vertex[vertcount].x = values[0];
                    vertex[vertcount].y = values[1];
                    vertex[vertcount].z = values[2];
                    vertcount++;
                    //cout << "i add to vertex" << '\n';
                    //}
                } else if (cmd == "tri"){
                    //cout << "i see tri command \n";
                    validinput = readvals(s, 3, values);
                    if (validinput) { 
                        tria[tricount].A = vec3(vertex[(int)values[0]].x, vertex[(int)values[0]].y, vertex[(int)values[0]].z);
                        tria[tricount].B = vec3(vertex[(int)values[1]].x, vertex[(int)values[1]].y, vertex[(int)values[1]].z);
                        tria[tricount].C = vec3(vertex[(int)values[2]].x, vertex[(int)values[2]].y, vertex[(int)values[2]].z);
                        //cout << ambient[0] << endl;
                        tria[tricount].color[0] = ambient[0];
                        tria[tricount].color[1] = ambient[1];
                        tria[tricount].color[2] = ambient[2];

                        tria[tricount].emission[0] = emission[0];
                        tria[tricount].emission[1] = emission[1];
                        tria[tricount].emission[2] = emission[2];

                        tria[tricount].diffuse.x = diffuse.x;
                        tria[tricount].diffuse.y = diffuse.y;
                        tria[tricount].diffuse.z = diffuse.z;

                        tria[tricount].specular.x = specular.x;
                        tria[tricount].specular.y = specular.y;
                        tria[tricount].specular.z = specular.z;
                    }

                    tria[tricount].transform = transfstack.top(); 
                    /*for(int i = 0; i< 4; i++){
                        for(int j = 0; j< 4; j++){
                            cout << tria[tricount].transform[i][j] << endl;
                        }
                    }*/
                   /*obj[numobjects].A = tria[tricount].A;
                   obj[numobjects].B = tria[tricount].B;
                   obj[numobjects].C = tria[tricount].C;
                   obj[numobjects].obj = tri;*/
                   tricount++;
                   //numobjects++;
                   //cout << "i add tri object \n";
                }

                //
                else if(cmd == "size"){
                    validinput = readvals(s, 2, values);
                        //mySize(values[0], values[1]);
                        myw = values[0];
                        myh = values[1];
                }
                else if (cmd == "translate") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {
                        mat4 toAdd = Transform::translate(values[0], values[1], values[2]);
                        rightmultiply(toAdd, transfstack);
                        //transfstack.pop();
                        // YOUR CODE FOR HW 2 HERE.  
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file. 
                        // Also keep in mind what order your matrix is!

                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {
                        mat4 toAdd = Transform::scale(values[0], values[1], values[2]);
                        rightmultiply(toAdd, transfstack);
                        //transfstack.pop();
                        // YOUR CODE FOR HW 2 HERE.  
                        // Think about how the transformation stack is affected
                        // You might want to use helper functions on top of file.  
                        // Also keep in mind what order your matrix is!

                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values); 
                    if (validinput) {
                        vec3 axis = vec3(values[0], values[1], values[2]);
                        mat3 toAdd = Transform::rotate(values[3], axis);
                        mat4 toMultiply = mat4(toAdd[0][0], toAdd[0][1], toAdd[0][2], 0, toAdd[1][0], toAdd[1][1],  toAdd[1][2], 0, toAdd[2][0], toAdd[2][1], toAdd[2][2], 0, 0, 0, 0, 1);
                        rightmultiply(toMultiply, transfstack);
                        // YOUR CODE FOR HW 2 HERE. 
                        // values[0..2] are the axis, values[3] is the angle.  
                        // You may want to normalize the axis (or in Transform::rotate)
                        // See how the stack is affected, as above.  
                        // Note that rotate returns a mat3. 
                        // Also keep in mind what order your matrix is!

                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top()); 
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n"; 
                    } else {
                        transfstack.pop(); 
                    }
                }

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }

            getline (in, str); 
        }
    }
}