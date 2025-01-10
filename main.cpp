#define MAINPROGRAM

#include <iostream>
#include "FreeImage.h"
#include "Transform.h"
#include "variables.h" 
#include "readfile.h"
#include <limits>
#include "C:\Documents\CompSci\cse167\testscenes\testscenes\packages\glm.0.9.7.1\build\native\include\glm\gtc\matrix_inverse.hpp"

#ifndef RAY_H
#define RAY_H

void maxdepth(){
    depth = 5;
}

class ray {
  public:
    ray() {}

    ray(const vec3 origin, const vec3& direction) : orig(origin), dir(direction) {}

    vec3 origin() const  { return orig; }
    vec3 direction() const { return dir; }

    vec3 at(float t) const {
        return orig + t*dir;
    }

  private:
    vec3 orig;
    vec3 dir;
};

#endif

vec3 compSpec(vec3 lightcolor, vec3 mnormal, vec3 halfvec, vec3 myspecular, float myshininess){
    float nDotH = dot(mnormal, halfvec) ; 
    vec3 phong;
        //cout << myshininess << endl;
        if(nDotH >= 0){
           phong = myspecular * lightcolor * pow(nDotH, myshininess) ;   
        }    
        else{
            phong = myspecular * lightcolor * pow(0.0f, myshininess) ; 
        } 
        return phong;
}

vec3 ComputeLight (vec3 direction, vec3 lightcolor, vec3 mnormal,  vec3 halfvec,  vec3 mydiffuse,  vec3 myspecular,  float myshininess) {

        float nDotL = dot(mnormal, direction)  ; 
        vec3 lambert;
        vec3 phong;

        //cout << nDotL << endl;
        //cout << mydiffuse.x << mydiffuse.y << mydiffuse.z <<endl;
        if(nDotL >= 0){
            lambert = mydiffuse * lightcolor * nDotL ;  
        }    
        else{
            lambert = mydiffuse * lightcolor * 0.0f;
        } 

        float nDotH = dot(mnormal, halfvec) ; 
        //cout << myshininess << endl;
        if(nDotH >= 0){
           phong = myspecular * lightcolor * pow(nDotH, myshininess) ;   
        }    
        else{
            phong = myspecular * lightcolor * pow(0.0f, myshininess) ; 
        }  

        vec3 retval = lambert + phong ; 
        //cout << retval.x << " " << retval.y << " " << retval.z << endl;
        //cout << "third: " << lightcolor.x << endl;
        return retval ;            
}      

ray RayThruPixel(vec3 observer, vec3 u, vec3 v, int i, int j){

        float alpha;
        float beta;
        float tanfovxOver2 = (tan(myfovy/2.0f))* (myw/myh);
        float tanfovyOver2 = (tan(myfovy/2.0f));
        float widthOver2 = myw/2.0f;
        float heightOver2 = myh/2.0f;

        alpha = tanfovxOver2 * (((j) - widthOver2+0.5)/widthOver2);
        beta = tanfovyOver2 * ((heightOver2 - (i)+0.5) /heightOver2);
        //cout << "alpha:" << alpha << " beta: " << beta <<'\n';

        vec3 p1 = normalize((alpha * u) + (beta * v) - observer);
        vec3 myray = p1;

        //cout << "p1: " << p1.x << " " << p1.y << " " << p1.z << '\n';
        //cout << "eye: " << eye.x << " " << eye.y << " " << eye.z << '\n';
        ray toReturn = ray(eye, p1);

        return toReturn;

}

float tintersection(tri tria, ray ray){
    int i;
    float alpha, alpha1, alpha2, alpha3, alpha4, alpha5;
    float beta, beta1, beta2, beta3, beta4, beta5;
    float gamma, gamma1, gamma2, gamma3, gamma4, gamma5;

    vec4 initAval = vec4(tria.A, 1);
    vec3 transformedA = vec3(tria.transform * initAval);

    vec4 initBval = vec4(tria.B, 1);
    vec3 transformedB = vec3(tria.transform * initBval);

    vec4 initCval = vec4(tria.C, 1);
    vec3 transformedC = vec3(tria.transform * initCval);

    vec3 n = normalize(cross(transformedC-transformedA, transformedB-transformedA));

    float t = (dot(transformedA, n) - dot(ray.origin(), n))/dot(ray.direction(),n);

    if(dot(ray.direction(),n) == 0){
        //cout << "dist is 0\n";
        t = -1;
        return t;
    }

    vec3 p = ray.direction()*t + ray.origin();
    vec3 pminusa = p-transformedA;
    vec3 bminusa = transformedB-transformedA;
    vec3 cminusa = transformedC-transformedA;

    beta = -(pminusa.x*cminusa.y - pminusa.y*cminusa.x)/(bminusa.y*cminusa.x-bminusa.x*cminusa.y);
    gamma = (-beta*bminusa.x+pminusa.x)/cminusa.x;

    beta2 = -(pminusa.x*cminusa.z - pminusa.z*cminusa.x)/(bminusa.z*cminusa.x-bminusa.x*cminusa.z);
    gamma2 = (-beta2*bminusa.x+pminusa.x)/cminusa.x;

    beta1 = -(pminusa.z*cminusa.y - pminusa.y*cminusa.z)/(bminusa.y*cminusa.z-bminusa.z*cminusa.y);
    gamma1 = (-beta1*bminusa.z+pminusa.z)/cminusa.z;

    beta5 = -(pminusa.z*cminusa.x - pminusa.x*cminusa.z)/(bminusa.x*cminusa.z-bminusa.z*cminusa.x);
    gamma5 = (-beta5*bminusa.z+pminusa.z)/cminusa.z;

    beta3 = -(pminusa.y*cminusa.x - pminusa.x*cminusa.y)/(bminusa.x*cminusa.y-bminusa.y*cminusa.x);
    gamma3 = (-beta3*bminusa.y+pminusa.y)/cminusa.y;

    beta4 = -(pminusa.y*cminusa.z - pminusa.z*cminusa.y)/(bminusa.z*cminusa.y-bminusa.y*cminusa.z);
    gamma4 = (-beta4*bminusa.y+pminusa.y)/cminusa.y;

    /*cout<< "gamma: " << gamma << '\n';
    cout<< "beta: " << beta << '\n' << '\n';*/
    //we lie inside the triangle*/
    if(beta+gamma < 1.00001 && beta>=-0.00001 && gamma>=-0.00001){
        alpha = 1 - beta - gamma;
        vec3 bforNormal = transformedA-transformedB;
        vec3 cforNormal = transformedC-transformedB;
        normal = cross(bforNormal, cforNormal);
        myt = t;
        intersectionPoint = alpha*transformedA+beta*transformedB+gamma*transformedC;
        return t;
    }
    else if(beta1+gamma1 < 1.00001 && beta1>=-0.00001 && gamma1>=-0.00001){
        alpha1 = 1 - beta1 - gamma1;
        vec3 bforNormal = transformedA-transformedB;
        vec3 cforNormal = transformedC-transformedB;
        normal = cross(bforNormal, cforNormal);
        myt = t;
        intersectionPoint = alpha1*transformedA+beta1*transformedB+gamma1*transformedC;
        return t;
    }
    else if(beta2+gamma2 < 1.00001 && beta2>=-0.00001 && gamma2>=-0.00001){
        alpha2 = 1 - beta2 - gamma2;
        vec3 bforNormal = transformedA-transformedB;
        vec3 cforNormal = transformedC-transformedB;
        normal = cross(bforNormal, cforNormal);
        myt = t;
        intersectionPoint = alpha2*transformedA+beta2*transformedB+gamma2*transformedC;
        return t;
    }
    else if(beta3+gamma3 < 1.00001 && beta3>=-0.00001 && gamma3>=-0.00001){
        alpha3 = 1 - beta3 - gamma3;
        vec3 bforNormal = transformedA-transformedB;
        vec3 cforNormal = transformedC-transformedB;
        normal = cross(bforNormal, cforNormal);
        myt = t;
        intersectionPoint = alpha3*transformedA+beta3*transformedB+gamma3*transformedC;
        return t;
    }
    else if(beta4+gamma4 < 1.00001 && beta4>=-0.00001 && gamma4>=-0.00001){
        alpha4 = 1 - beta4 - gamma4;
        vec3 bforNormal = transformedA-transformedB;
        vec3 cforNormal = transformedC-transformedB;
        normal = cross(bforNormal, cforNormal);
        myt = t;
        intersectionPoint = alpha4*transformedA+beta4*transformedB+gamma4*transformedC;
        return t;
    }
    else if(beta5+gamma5 < 1.00001 && beta5>=-0.00001 && gamma5>=-0.00001){
        alpha5 = 1 - beta5 - gamma5;
        vec3 bforNormal = transformedA-transformedB;
        vec3 cforNormal = transformedC-transformedB;
        normal = cross(bforNormal, cforNormal);
        myt = t;
        intersectionPoint = alpha5*transformedA+beta5*transformedB+gamma5*transformedC;
        return t;
    }
    //we dont
    else{
        //cout << "i no color\n";
        //cout<< oindex << '\n' << '\n';
        t = -1;
    }

    return t;
}

float cintersection(ray ray, sphere circ){
    RGBQUAD color;
    vec3 p1 = ray.direction();
    vec3 p0 = ray.origin();

    //cout << "p0: " << p0.x << " " << p0.y << " " << p0.z << endl;

    vec4 p0new = vec4(p0, 1);
    vec4 p1new = vec4(p1, 0);
    
    vec3 inversep0 = vec3(inverse(circ.transform) * p0new);
    vec3 inversep1 = vec3(inverse(circ.transform) * p1new);
    mytranspose = inverseTranspose(circ.transform);

    vec3 circcen = vec3(circ.x, circ.y, circ.z);

    //initialize t to be -1. -1 represents no intersection
    float t = -1;

    float qa = glm::dot(inversep1, inversep1);
    float qb = 2.0f * glm::dot(inversep1, inversep0-circcen);
    float qc =  glm::dot(inversep0-circcen, inversep0-circcen) - (circ.radius*circ.radius);

    //cout << "a:" << qa << " b: " << qb << " c; " << qc << endl;

    float discriminant = (qb*qb) - (4.0f * qa * qc);

    float t0 = (-qb-sqrt(discriminant))/(2.0f*qa);
    float t1 = (-qb+sqrt(discriminant))/(2.0f*qa);

    //cout << discriminant << endl;
    // 2 real roots
    if(discriminant > 0){
        //both positive
        if(t1 > 0 && t0 >0){
            //pick smaller one
            if( t1 > t0){
                t = t0;
                myt = t;
                intersectionPoint =  inversep0+ t*inversep1;
                normal = circcen-intersectionPoint;
                vec4 editnormal = vec4(normal, 1);
                normal = vec3(mytranspose* editnormal);
            }
            else{ //if(t0 > t1){
                t = t1;
                myt = t;
                intersectionPoint =  inversep0+ t*inversep1;
                normal = circcen-intersectionPoint;
                vec4 editnormal = vec4(normal, 1);
                normal = vec3(mytranspose* editnormal);
            }
        } 

        //one positve one negative
        else if(t1 < 0){
            if(t0 > 0){
                t = t0;
                myt = t;
                intersectionPoint =  inversep0+ t*inversep1;
                normal = circcen-intersectionPoint;
                vec4 editnormal = vec4(normal, 1);
                normal = vec3(mytranspose* editnormal);
            }
        }
        else if(t0 < 0){
            if(t1 > 0){
                t = t1;
                myt = t;
                intersectionPoint =  inversep0+ t*inversep1;
                normal = circcen-intersectionPoint;
                vec4 editnormal = vec4(normal, 1);
                normal = vec3(mytranspose* editnormal);
            }
        }
    }
    // both roots same
    else if(discriminant == 0){
        if(t1 >0){
            t = t1;
            myt = t;
            intersectionPoint =  inversep0+ t*inversep1;
            //intersectionPoint = vec3(circ.transform*vec4(intersectionPoint, 1));
            normal = circcen-intersectionPoint;
            vec4 editnormal = vec4(normal, 1);
            normal = vec3(mytranspose* editnormal);
        }
    } 
    else{
        t = -1;
    }

    return t;
}
vec3 nextDir(vec3 curdir, vec3 normal){
    vec3 toreturn;
    toreturn = curdir - 2*dot(curdir,normal)*normal;
    return toreturn;
}



int nextShape(ray myray){
    float shortestDist = FLT_MAX;
    float newshortestDist = 0;
    RGBQUAD color;
    float myval = -1;
    
    //cout << tricount << '\n';
    for(int i = 0; i<tricount; i++){
        //cout << tricount << '\n';
        newshortestDist = tintersection(tria[i], myray);
        if(newshortestDist < shortestDist && newshortestDist >= 0){
            //cout << "intersect with table" <<endl;
            reflectDiffuse = tria[i].diffuse;
            reflectSpec = tria[i].specular;
            reflectnorm = normal;
            myval = 1;
            //cout << "intersect with table" <<endl;
        }
    }
    //cout << circcount << '\n';
    for(int j = 0; j<circcount; j++){

        //vec3 circen = vec3(circ[i].x, circ[i].y, circ[i].z);
        newshortestDist = cintersection(myray, circ[j]);
        //cout << newshortestDist << '\n';
        if(newshortestDist < shortestDist && newshortestDist >= 0){
            //cout << newshortestDist << '\n';
            reflectDiffuse = circ[j].diffuse;
            reflectSpec = circ[j].specular;
            reflectnorm = normal;
            myval = 2;
        }
    }

    return myval;
}

int checkifshadowrayconnects(ray myray, vec3 light){
    float shortestDist = FLT_MAX;
    float newshortestDist = 0;
    RGBQUAD color;
    float myval = -1;
    
    //cout << tricount << '\n';
    for(int i = 0; i<tricount; i++){
        //cout << tricount << '\n';
        newshortestDist = tintersection(tria[i], myray);
        /*if(newshortestDist != -1){
            cout << "t dist" << newshortestDist <<endl;
        }*/
        if(newshortestDist < shortestDist && newshortestDist >= 0){
            //cout << "intersect with table" <<endl;
            myval = 1;
            //cout << "intersect with table" <<endl;
        }
    }
    //cout << circcount << '\n';
    for(int j = 0; j<circcount; j++){

        //vec3 circen = vec3(circ[i].x, circ[i].y, circ[i].z);
        newshortestDist = cintersection(myray, circ[j]);
        //cout << newshortestDist << '\n';
        if(newshortestDist < shortestDist && newshortestDist >= 0){
            //cout << newshortestDist << '\n';
            myval = 1;
        }
    }

    return myval;
}

vec3 applyDirectionalLight(vec3 directionlight, vec3 normal, vec3 diffuse, vec3 myspecular, int torc, ray theray){
    vec3 direction0;
    direction0.x = directional[0];
    direction0.y = directional[1];
    direction0.z = directional[2];

    vec3 mynormal = normalize(normal);
    vec3 eyedirn = normalize(eye-intersectionPoint);

    direction0 = normalize(direction0);
    vec3 half0 = normalize(direction0+eyedirn);
    if(torc == 2){
        //vec4 editnormal = vec4(point1, 1);
        //vec3 editpoint = vec3(transpose(translations)*editnormal);
        vec3 worldorigin = vec3(transpose(translations)*vec4(theray.origin(),1));
        vec3 worldDir = vec3(transpose(translations)*vec4(theray.direction(), 0));
        vec3 newintersect = worldorigin + mycrt*worldDir;
        vec3 rintersect = vec3(inverseTranspose(translations)*vec4(newintersect,1));
        direction0 = normalize(rintersect-direction0);
        eyedirn = (rintersect-worldorigin);
        
        ray shadowray = ray(rintersect+(1.0f/1000.0f *-direction0), -direction0);
        half0 = normalize(direction0+eyedirn);
        if(checkifshadowrayconnects(shadowray, direction0) == 1){
            vec3 nocol = vec3(0,0,0);
            return nocol;
        }
        
    }
    else{
        vec3 newintersect = theray.origin() + myrt*theray.direction();
        eyedirn = normalize(newintersect-eye);
        direction0 = normalize(newintersect-direction0);
        //vec3 newdir = normalize(point1 - newintersect);
        ray shadowray = ray(newintersect + (1.0f/1000.0f *  -direction0) , -direction0);
        half0 = normalize(direction0+eyedirn);
        
        if(checkifshadowrayconnects(shadowray, direction0) == 1){
            //cout << depth <<endl;
            //cout << "i connect triangle" << endl;
            vec3 nocol = vec3(0,0,0);
            return nocol;
        }
        
        newintersect = theray.origin() + myrt*theray.direction();
        eyedirn = normalize(newintersect-eye);
        direction0 = normalize(newintersect);
        //vec3 newdir = normalize(point1 - newintersect);
        shadowray = ray(newintersect + (1.0f/1000.0f *  -direction0) , -direction0);
        half0 = normalize(direction0+eyedirn);
        

    }
    
    vec3 colAdd = ComputeLight(direction0, directionlight, mynormal, half0, diffuse, myspecular, shininess);

    return colAdd;
}

vec3 applyPointLight(vec3 pointlight,  vec3 normal , /*vec3 aintersectionPoint,*/ vec3 diffuse, vec3 myspecular, int torc, ray theray){
    vec3 point1;
    point1.x = point[0];
    point1.y = point[1];
    point1.z = point[2];
    //convert point light to world coords

    //cout << myspecular.x << myspecular.y << myspecular.z << endl;

    //normalize normal
    vec3 mynormal = normalize(normal);


    vec3 eyedirn;
    vec3 direction1;
    vec3 half1;
    ray nextray;
    vec3 nextd;
    vec3 addon;
    int shape;

    //cout << pointlight.x << endl;

    if(torc == 2){
        //vec4 editnormal = vec4(point1, 1);
        //vec3 editpoint = vec3(transpose(translations)*editnormal);
        vec3 worldorigin = vec3(transpose(translations)*vec4(theray.origin(),1));
        vec3 worldDir = vec3(transpose(translations)*vec4(theray.direction(), 0));
        vec3 newintersect = worldorigin + mycrt*worldDir;
        vec3 rintersect = vec3(inverseTranspose(translations)*vec4(newintersect,1));
        direction1 = normalize(rintersect-point1);
        eyedirn = (rintersect-worldorigin);
        
        ray shadowray = ray(rintersect+(1.0f/1000.0f *-direction1), -direction1);
        half1 = normalize(direction1+eyedirn);
        if(checkifshadowrayconnects(shadowray, point1) == 1){
            return vec3(0, 0, 0);
            //return nocol;
        }
    }
    else{
        vec3 newintersect = theray.origin() + myrt*theray.direction();
        eyedirn = normalize(newintersect-eye);
        direction1 = normalize(newintersect-point1);
        //vec3 newdir = normalize(point1 - newintersect);
        ray shadowray = ray(newintersect + (1.0f/1000.0f *  -direction1) , -direction1);
        half1 = normalize(direction1+eyedirn);
        
        if(checkifshadowrayconnects(shadowray, point1) == 1){
            //cout << depth <<endl;,
            return vec3(0,0,0);
            //return nocol;
        }
        
        newintersect = theray.origin() + myrt*theray.direction();
        eyedirn = normalize(newintersect-eye);
        direction1 = normalize(newintersect-point1);
        //vec3 newdir = normalize(point1 - newintersect);
        shadowray = ray(newintersect + (1.0f/1000.0f *  -direction1) , -direction1);
        half1 = normalize(direction1+eyedirn);

    }
    //vec3 temp = myspecular*addon;
    //cout << depth << endl;
    
    vec3 colAdd = ComputeLight(direction1, pointlight, mynormal, half1, diffuse, myspecular, shininess);

    return colAdd;
}

vec3 findlight(vec3 pointlight,  vec3 normal , vec3 diffuse, vec3 myspecular, int torc, ray theray){
    if(depth == 0){
        return vec3(0.0f,0.0f,0.0f); 
    }
    vec3 mynormal = normalize(normal);
    int shape;
    ray nextray;

    if(torc == 2){
        vec3 worldorigin = vec3(transpose(translations)*vec4(theray.origin(),1));
        vec3 worldDir = vec3(transpose(translations)*vec4(theray.direction(), 0));
        vec3 newintersect = worldorigin + mycrt*worldDir;
        vec3 rintersect = vec3(inverseTranspose(translations)*vec4(newintersect,1));

        vec3 curDir = normalize(theray.direction());//rintersect-theray.origin());
        vec3 nextd = nextDir(curDir, mynormal);
        nextray = ray(rintersect+(1/1000.0f* nextd), nextd);
        shape = nextShape(nextray);
    }
    else{
        vec3 newintersect = theray.origin() + myrt*theray.direction();
        vec3 curDir = normalize(theray.direction());//normalize(newintersect-theray.origin());
        vec3 nextd = nextDir(curDir, mynormal);
        nextray = ray(newintersect+(1/1000.0f * nextd),nextd);
        shape = nextShape(nextray);
    }

    vec3 currentcol = applyPointLight(pointlight, reflectnorm, reflectDiffuse, reflectSpec, shape, nextray);

    if(shape == -1){
        return vec3(0.0f,0.0f,0.0f);
    }

    vec3 finalColor = currentcol+myspecular*findlight(pointlight, reflectnorm, reflectDiffuse, reflectSpec, shape, nextray);

    return finalColor;
}

RGBQUAD draw(ray myray){
    float shortestDist = FLT_MAX;
    float newshortestDist;
    //int shortestIndex;
    int tOrc = 0;
    RGBQUAD color;
    depth = 5;
    //cout << tricount << '\n';
    for(int i = 0; i<tricount; i++){
        //cout << tricount << '\n';
        newshortestDist = tintersection(tria[i], myray);
        if(newshortestDist < shortestDist && newshortestDist != -1){
            tOrc = 1;
            shortestDist = newshortestDist;
            tnormal = normal;
            tintersectionPoint = intersectionPoint;
            shortestIndex = i;
            myrt = myt;
            //translations = mytranspose;
        }
    }
    //cout << circcount << '\n';
    for(int i = 0; i<circcount; i++){

        //vec3 circen = vec3(circ[i].x, circ[i].y, circ[i].z);
        newshortestDist = cintersection(myray, circ[i]);
        //cout << newshortestDist << '\n';
        if(newshortestDist < shortestDist && newshortestDist >= 0){
            //cout << newshortestDist << '\n';
            tOrc = 2;
            shortestDist = newshortestDist;
            shortestIndex = i;
            cintersectionPoint = intersectionPoint;
            cnormal = normal;
            mycrt = myt;
            translations = mytranspose;
            //cout << circ[16].color[0] << '\n';
        }
    }

    if(tOrc == 1){
        vec3 directionray2;
        vec3 directionlight;
        vec3 pointr;
        vec3 pointlight;

        pointlight.x = point[3];
        pointlight.y = point[4];
        pointlight.z = point[5];

        directionlight.x = directional[3];
        directionlight.y = directional[4];
        directionlight.z = directional[5];
        //cout << "first: " << pointlight.x << endl;

        //ray lightray = ray(tintersectionPoint, directionray2);

        int itIsAShadow = 0;
        int itIsAPShadow = 0;

            //vec3 othercol = applyDirectionalLight(directionlight, tnormal, tria[shortestIndex].diffuse, tria[shortestIndex].specular, tOrc, myray);
            vec3 colAdd = applyPointLight(pointlight, tnormal, tria[shortestIndex].diffuse, tria[shortestIndex].specular, tOrc, myray) ;
            colAdd = colAdd + /*tria[shortestIndex].specular*/findlight(pointlight, tnormal, tria[shortestIndex].diffuse, tria[shortestIndex].specular, tOrc, myray);
            //cout << "ambient +emission " << tria[shortestIndex].color[0] << endl;
            color.rgbRed = (tria[shortestIndex].color[0] +tria[shortestIndex].emission[0]+ colAdd.x) *255.0f;
            color.rgbGreen = (tria[shortestIndex].color[1] + tria[shortestIndex].emission[1]+colAdd.y)*255.0f;
            color.rgbBlue = (tria[shortestIndex].color[2] +tria[shortestIndex].emission[2]+ colAdd.z)*255.0f;

            shortestdist = shortestIndex;

        return color;
    }
    else if(tOrc == 2){
        //cout << "draw circles" << endl;
        vec3 directionray2;
        vec3 directionlight;
        vec3 pointr;
        vec3 pointlight;

        pointlight.x = point[3];
        pointlight.y = point[4];
        pointlight.z = point[5];
        directionlight.x = directional[3];
        directionlight.y = directional[4];
        directionlight.z = directional[5];

        vec3 othercol = applyDirectionalLight(directionlight, tnormal, tria[shortestIndex].diffuse, tria[shortestIndex].specular, tOrc, myray);

        vec3 colAdd = applyPointLight(pointlight, cnormal, circ[shortestIndex].diffuse, circ[shortestIndex].specular, tOrc, myray);
        colAdd = colAdd + /*circ[shortestIndex].specular*/findlight(pointlight, cnormal, circ[shortestIndex].diffuse, circ[shortestIndex].specular, tOrc, myray);

        vec3 worldorigin = vec3(transpose(translations)*vec4(myray.origin(),1));
        vec3 worldDir = vec3(transpose(translations)*vec4(myray.direction(), 0));
        vec3 newintersect = worldorigin + mycrt*worldDir;
        vec3 rintersect = vec3(inverseTranspose(translations)*vec4(newintersect,1));

        //vec3 colAdd = findlight(rintersect, pointlight, myray, cnormal,circ[shortestIndex].diffuse, circ[shortestIndex].specular);

        color.rgbRed = (circ[shortestIndex].color[0] + circ[shortestIndex].emission[0]+colAdd.x) *255.0f;
        color.rgbGreen = (circ[shortestIndex].color[1] + circ[shortestIndex].emission[1]+colAdd.y)*255.0f;
        color.rgbBlue = (circ[shortestIndex].color[2] + circ[shortestIndex].emission[2]+colAdd.z)*255.0f;
        //cout << "green for both: " << tria[shortestIndex].color[1] + colAdd.x << endl;
        shortestdist = shortestIndex;

        return color;
    }
    else{
        //cout << " i didn't hit an object" <<endl;
        color.rgbRed = 0;
        color.rgbGreen = 0;
        color.rgbBlue = 0;
    }

    return color;
}


main(int argc, char* argv[]){
    FreeImage_Initialise();


    ambient[0] = 0.2f;
    ambient[1] = 0.2f;
    ambient[2] = 0.2f;
    attenuation[0] = 1.0f;
    attenuation[1] = 0.0f;
    attenuation[2] = 0.0f;

    readfile(argv[1]);

    vec3 a = eye - center;

    vec3 observer = glm::normalize(a);
    vec3 u = glm::normalize(glm::cross(up, observer));
    vec3 v = glm::cross(observer,u);

    FIBITMAP *bitmap = FreeImage_Allocate(myw, myh, 24);
    RGBQUAD color ;

    for ( int i =0; i < myh; i++) {
        for ( int j =0; j <myw; j++) {

            ray myray = RayThruPixel(observer, u, v, i, j);
            color = draw(myray);
            

            FreeImage_SetPixelColor(bitmap, j, myh-i, &color);
    }
}

    //sFIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);


    //FreeImage_Save(FIF_PNG, img, "img.png", 0);
    FreeImage_Save(FIF_PNG, bitmap, argv[2], 0);


    //cout<< "image saved!";
    FreeImage_DeInitialise();
}