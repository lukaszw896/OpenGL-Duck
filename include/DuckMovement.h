//
// Created by lukas on 5/30/2016.
//

#ifndef DUCK_DUCKMOVEMENT_H
#define DUCK_DUCKMOVEMENT_H
#include <glm/glm.hpp>
#include <vector>
#include <cstdlib>
#define NUM_OF_CON_POINTS 20
#define LAP_TIME 30

using namespace glm;
using namespace std;

class DuckMovement {
public:
    DuckMovement();
    vec2 getCoords(float frameDT);
    vec2 getLastGeneratedCords();
    vec2 getMovementDirection();
private:
    vector<vec2> controlPoitns;
    vec2 lastGeneratedCord;
    vec2 movementDirection;
    float currentTime;
    float lapTime;
    vector<float> knotVector;
    float lastGenX = 0.f;
    float lastGenY = 0.f;

    void genRandControlPoints(int numOfControlPoints);
    vec2 calculateNewCord(const vector<float>& knotVector, float t, int n);
    float bSplineRecurrsion(float t, int n, int i, const vector<float>& knotVector);

    float getRandomNumber();



};


#endif //DUCK_DUCKMOVEMENT_H
