#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static float motion_mat[64] = {
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 1,
};

static const float std_weight_position = 1.0f / 20 ;
static const float std_weight_velocity = 1.0f / 160;

void init(float x, float y, float a, float h)
{
    float mean[8] = { x, y, a, h };
    float covariance[64] = {0};
    float dt = 1.0;
    covariance[0 * 8 + 0] = 2 * std_weight_position * h;
    covariance[1 * 8 + 1] = 2 * std_weight_position * h;
    covariance[2 * 8 + 2] = 0.01;
    covariance[3 * 8 + 3] = 2 * std_weight_position * h;
    covariance[4 * 8 + 4] = 10* std_weight_velocity * h;
    covariance[5 * 8 + 5] = 10* std_weight_velocity * h;
    covariance[6 * 8 + 6] = 0.00001;
    covariance[7 * 8 + 7] = 10* std_weight_velocity * h;
    motion_mat[0 * 8 + 4] = dt;
    motion_mat[1 * 8 + 5] = dt;
    motion_mat[2 * 8 + 6] = dt;
    motion_mat[3 * 8 + 7] = dt;
}

void predict(float mean[8], float covariance[64])
{
    float motion_cov[64] = {0};
    motion_cov[0 * 8 + 0] = pow(std_weight_position * mean[3], 2);
    motion_cov[1 * 8 + 1] = pow(std_weight_position * mean[3], 2);
    motion_cov[2 * 8 + 2] = pow(0.01, 2);
    motion_cov[3 * 8 + 3] = pow(std_weight_position * mean[3], 2);
    motion_cov[4 * 8 + 4] = pow(std_weight_velocity * mean[3], 2);
    motion_cov[5 * 8 + 5] = pow(std_weight_velocity * mean[3], 2);
    motion_cov[6 * 8 + 6] = pow(0.00001, 2);
    motion_cov[7 * 8 + 7] = pow(std_weight_velocity * mean[3], 2);

    // mean = motion_mat * mean
    // covariance = motion_mat * covariance * motion_mat' + motion_cov
    // todo...
}







