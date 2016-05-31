//
// Created by lukas on 5/30/2016.
//

#include "DuckMovement.h"

DuckMovement::DuckMovement() {
    genRandControlPoints(NUM_OF_CON_POINTS);
    currentTime = 0;
    lastGeneratedCord = vec2(0,0);
    int knotNumber = controlPoitns.size() +4;
    float startValue = 0.f;
    float knotDT = 1.0f / (knotNumber-7);
    knotVector.resize(knotNumber);
    knotVector[0] = 0;
    knotVector[1] = 0;
    knotVector[2] = 0;
    knotVector[knotNumber-3] = 1;
    knotVector[knotNumber-2] = 1;
    knotVector[knotNumber-1] = 1;
    for(int i=3;i<knotNumber-3;i++)
    {

        knotVector[i] = startValue;
        startValue += knotDT;
    }

}

vec2 DuckMovement::getCoords(float frameDT) {
    currentTime += frameDT;

    if(currentTime>LAP_TIME) {
        currentTime -= LAP_TIME;
        genRandControlPoints(NUM_OF_CON_POINTS);
    }
    vec2 currentCords = calculateNewCord(knotVector,(currentTime/LAP_TIME),3);
    vec2 nextCords = calculateNewCord(knotVector,((currentTime+frameDT)/LAP_TIME),3);
    movementDirection = normalize(nextCords - lastGeneratedCord);
    lastGeneratedCord = currentCords;

    return currentCords;
}

void DuckMovement::genRandControlPoints(int numOfControlPoints) {
    controlPoitns.clear();
    controlPoitns.push_back(vec2(0,0));
    for(int i=0;i<numOfControlPoints;i++)
    {
        float xCord,yCord;
        bool findNewX = true;
        while(findNewX)
        {
            float tmpX = getRandomNumber();
            if(abs(tmpX-lastGenX)<0.2f){
                findNewX = false;
                xCord = tmpX;
            }
        }
        bool findNewY = true;
        while(findNewY)
        {
            float tmpY = getRandomNumber();
            if(abs(tmpY-lastGenY)<0.5f){
                findNewY = false;
                yCord = tmpY;
            }
        }
        controlPoitns.push_back(vec2(xCord,yCord));
    }
    controlPoitns.push_back(vec2(0,0));
}


float DuckMovement::getRandomNumber(){
    float LO = -0.8f;
    float HI = 0.8f;

    return (LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO))));
}

vec2 DuckMovement::calculateNewCord(const vector<float> &knotVector, float t, int n) {
    float x,y;
    x = y = 0;
    for(unsigned int i = 0; i < controlPoitns.size(); i++){
        float bsplineBasisValue = bSplineRecurrsion(t, n, i, knotVector);

        x += controlPoitns[i].x * bsplineBasisValue;
        y += controlPoitns[i].y * bsplineBasisValue;
    }
    return vec2(x,y);
}

float DuckMovement::bSplineRecurrsion(float t, int n, int i, const vector<float> &knotVector) {
    if (n == 0){
        if(t >= knotVector[i] && t < knotVector[i+1])
            return 1;
        else
            return 0;
    }
    float leftRecursion = (t - knotVector[i]);
    float leftDenominator = (knotVector[i+n] - knotVector[i]);
    if(leftRecursion == 0 || leftDenominator == 0)
        leftRecursion = 1;
    else
        leftRecursion /= leftDenominator;

    float rightRecursion = knotVector[i+1+n] - t;
    float rightDenominator = (knotVector[i+1+n] - knotVector[i+1]);
    if(rightRecursion == 0 || rightDenominator == 0)
        rightRecursion = 1;
    else
        rightRecursion /= rightDenominator;

    leftRecursion *= bSplineRecurrsion(t, n-1, i, knotVector);
    rightRecursion *= bSplineRecurrsion(t, n-1, i+1, knotVector);

    return leftRecursion + rightRecursion;
}

vec2 DuckMovement::getMovementDirection() {
    return movementDirection;
}
