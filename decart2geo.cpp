#include "decart2geo.h"

static double x;
static double y;
static double n;
static double beta;
static double sin_beta;
static double B0;
static double z0;
static double z0_pow2;
static double sin_pow2;
static double sin_pow4;
static double sin_pow6;
static double dB;
static double l;

void decart2geo(double* lat, double* lon)
{
	x = *lat;
	y = *lon;

	n = (double)((int)(y * 0.000001));
	//modf(y * 0.000001, &n);

	beta = x / 6367558.4968;
	sin_beta = sin(beta);
	B0 = beta + sin(2.0 * beta * (
		0.00252588685 - 0.00001491860 * pow(sin_beta, 2.0) +
		0.00000011904 * pow(sin_beta, 4.0)));
	z0 = (y - (10.0 * n + 5.0) * 100000.0) / (6378245.0 * cos(B0));

	z0_pow2 = pow(z0, 2.0);
	sin_pow2 = pow(sin(B0), 2.0);
	sin_pow4 = pow(sin(B0), 4.0);
	sin_pow6 = pow(sin(B0), 6.0);

	dB = -z0_pow2 * sin(2.0 * B0 * (0.251684631 -
		0.003369263 * sin_pow2 +
		0.000011276 * sin_pow4 -
		z0_pow2 * (0.10500614 -
			0.04559916 * sin_pow2 +
			0.00228901 * sin_pow4 -
			0.00002987 * sin_pow6 -
			z0_pow2 * (0.042858 -
				0.025318 * sin_pow2 +
				0.014346 * sin_pow4 -
				0.001264 * sin_pow6 -
				z0_pow2 * (0.01672 -
					0.00630 * sin_pow2 +
					0.01188 * sin_pow4 -
					0.00328 * sin_pow6)))));

	l = z0 * (1.0 -
		0.0033467108 * sin_pow2 -
		0.0000056002 * sin_pow4 -
		0.0000000187 * sin_pow6 -
		z0_pow2 * (0.16778975 +
			0.16273586 * sin_pow2 -
			0.00052490 * sin_pow4 -
			0.00000846 * sin_pow6 -
			z0_pow2 * (0.0420025 +
				0.1487407 * sin_pow2 +
				0.0059420 * sin_pow4 -
				0.0000150 * sin_pow6 -
				z0_pow2 * (0.01225 +
					0.09477 * sin_pow2 +
					0.03282 * sin_pow4 -
					0.00034 * sin_pow6 -
					z0_pow2 * (0.0038 +
						0.0524 * sin_pow2 +
						0.0482 * sin_pow4 +
						0.0032 * sin_pow6)))));

	*lat = B0 + dB;
	*lon = 6.0 * (n - 0.5) / 57.29577951 + l;

	return;
}