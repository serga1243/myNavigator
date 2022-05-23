#include "kalmanFilter.h"

static const double c_a[9] = { 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.5, 1.0, 1.0 };
static const double dv[9] = { 1.0, 1.0, 0.5, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0 };
static const signed char Q[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
static const signed char d_a[3] = { 1, 0, 0 };
static const signed char iv[3] = { 1, 0, 0 };

static double b_a[9];
static double p_prd[9];
static double b_B[3];
static double x_prd[3];
static double b;
static double d;
static double d1;
static double d2;
static double d3;
static double d4;
static int x_prd_tmp;
static double B;
static double a;
static unsigned short i;
static unsigned short i1;
static unsigned short j;

void kalmanFilter(struct KalmanFilterCoordinate* kalmanFilterCoordinate, struct Coordinate* coordinate, unsigned char* logInfo)
{
    B = 0;
    a = 0;
    *logInfo = 4;
    for (i = 0; i < 3; i++) {
        d = 0.0;
        for (i1 = 0; i1 < 3; i1++) {
            x_prd_tmp = i + 3 * i1;
            d += c_a[x_prd_tmp] * kalmanFilterCoordinate->x_est[i1];
            b_a[x_prd_tmp] =
                (c_a[i] * kalmanFilterCoordinate->p_est[3 * i1] + c_a[i + 3] * kalmanFilterCoordinate->p_est[3 * i1 + 1]) +
                c_a[i + 6] * kalmanFilterCoordinate->p_est[3 * i1 + 2];
        }
        x_prd[i] = d;
        d1 = b_a[i];
        d2 = b_a[i + 3];
        b = b_a[i + 6];
        d3 = 0.0;
        for (i1 = 0; i1 < 3; i1++) {
            x_prd_tmp = i + 3 * i1;
            d4 = ((d1 * dv[3 * i1] + d2 * dv[3 * i1 + 1]) + b * dv[3 * i1 + 2]) +
                (double)Q[x_prd_tmp];
            p_prd[x_prd_tmp] = d4;
            d3 += (double)d_a[i1] * d4;
        }
        b_B[i] = d3;
        B += d3 * (double)iv[i];
        a += (double)d_a[i] * d;
    }
    *logInfo = 3;
    B += kalmanFilterCoordinate->R;
    b = coordinate->decodedPos.value - a;
    d = b_B[0] / B;
    kalmanFilterCoordinate->x_est[0] = x_prd[0] + d * b;
    d1 = b_B[1] / B;
    kalmanFilterCoordinate->x_est[1] = x_prd[1] + d1 * b;
    d2 = b_B[2] / B;
    kalmanFilterCoordinate->x_est[2] = x_prd[2] + d2 * b;
    *logInfo = 2;
    for (i = 0; i < 3; i++) {
        x_prd_tmp = d_a[i];
        b_a[3 * i] = d * (double)x_prd_tmp;
        b_a[3 * i + 1] = d1 * (double)x_prd_tmp;
        b_a[3 * i + 2] = d2 * (double)x_prd_tmp;
    }
    for (i = 0; i < 3; i++) {
        d = b_a[i];
        d1 = b_a[i + 3];
        d2 = b_a[i + 6];
        for (i1 = 0; i1 < 3; i1++) {
            x_prd_tmp = i + 3 * i1;
            kalmanFilterCoordinate->p_est[x_prd_tmp] =
                p_prd[x_prd_tmp] - ((d * p_prd[3 * i1] + d1 * p_prd[3 * i1 + 1]) +
                    d2 * p_prd[3 * i1 + 2]);
        }
    }
    *logInfo = 1;

    coordinate->filteredPos.value = kalmanFilterCoordinate->x_est[0];
    *logInfo = 0;

    return;
}



void kalmanFilterInit(struct KalmanFilterCoordinate* kalmanFilterCoordinate, const double startCoordinate, const double R)
{


    // Стандартный фильтр Калмана
    for (j = 0; j < 9; j++)
    {
        kalmanFilterCoordinate->p_est[j] = 0;
    }
    kalmanFilterCoordinate->x_est[0] = startCoordinate;
    for (j = 1; j < 3; j++)
    {
        kalmanFilterCoordinate->x_est[j] = 0;
    }
    kalmanFilterCoordinate->R = R;


    return;
}