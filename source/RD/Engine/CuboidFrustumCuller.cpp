/**
 *  @file CuboidFrustumCuller.cpp
 *  @details Version 20200513.
 */
#include <Engine.hpp>


RD::Engine::CuboidFrustumCuller::CuboidFrustumCuller(const glm::mat4& cameraProjectionViewMatrix){
    const GLfloat *pMat = glm::value_ptr(cameraProjectionViewMatrix);
    cullInfo[0][0] = pMat[3] + pMat[1];
    cullInfo[0][1] = pMat[7] + pMat[5];
    cullInfo[0][2] = pMat[11] + pMat[9];
    cullInfo[0][3] = -(pMat[15] + pMat[13]);
    cullInfo[1][0] = pMat[3] - pMat[1];
    cullInfo[1][1] = pMat[7] - pMat[5];
    cullInfo[1][2] = pMat[11] - pMat[9];
    cullInfo[1][3] = pMat[13] - pMat[15];
    cullInfo[2][0] = pMat[3] + pMat[0];
    cullInfo[2][1] = pMat[7] + pMat[4];
    cullInfo[2][2] = pMat[11] + pMat[8];
    cullInfo[2][3] = -(pMat[15] + pMat[12]);
    cullInfo[3][0] = pMat[3] - pMat[0];
    cullInfo[3][1] = pMat[7] - pMat[4];
    cullInfo[3][2] = pMat[11] - pMat[8];
    cullInfo[3][3] = pMat[12] - pMat[15];
    cullInfo[4][0] = pMat[3] - pMat[2];
    cullInfo[4][1] = pMat[7] - pMat[6];
    cullInfo[4][2] = pMat[11] - pMat[10];
    cullInfo[4][3] = pMat[14] - pMat[15];
    cullInfo[5][0] = pMat[3];
    cullInfo[5][1] = pMat[7];
    cullInfo[5][2] = pMat[11];
    cullInfo[5][3] = -pMat[15];
}

RD::Engine::CuboidFrustumCuller::~CuboidFrustumCuller(){}

bool RD::Engine::CuboidFrustumCuller::IsVisible(glm::vec3 blockLowestPosition, glm::vec3 blockDimension){
    // Check all six planes
    for(unsigned plane = 0; plane != 6; plane++){
        if(cullInfo[plane][0] * (cullInfo[plane][0] > 0.0f ? blockLowestPosition[0] + blockDimension.x : blockLowestPosition[0]) + cullInfo[plane][1] * (cullInfo[plane][1] > 0.0f ? blockLowestPosition[1] + blockDimension.y : blockLowestPosition[1]) + cullInfo[plane][2] * (cullInfo[plane][2] > 0.0f ? blockLowestPosition[2] + blockDimension.z : blockLowestPosition[2]) < cullInfo[plane][3]){
            return false;
        }
    }
    return true;
}

