#include "decart2geo.h"

static double Dfloor;



#if GEO_DECART_TRANSFORM_METHOD == 1

#define TRANSFORM_ACCURANCE 	1.0E-4
static double B;
static double L;
static double D;
static double La;
static double r;
static double c;
static double p;
static double s2;
static double d;
static int8_t itN;

static const double halfPi   = 1.5707963267948966192313;
static const double singlePi = 3.1415926535897932384626;
static const double doublePi = 6.2831853071795864769253;
static const double radianPi = 57.295779513082323000000;

extern const double gnss_a[];
extern const double gnss_alpha[];
extern const double gnss_e_pow2[];
extern const double gnss_a_e_pow2[];

static uint8_t gnssType;



void decart2geo(struct MyNavigator* myNavigator, double* lat, double* lon, double* alt)
{
	if 		(strncmp((const char *)myNavigator->msgData.idData, "GP", 2) == 0) { gnssType = GPS;     }
	else if (strncmp((const char *)myNavigator->msgData.idData, "GL", 2) == 0) { gnssType = GLONASS; }
	else if (strncmp((const char *)myNavigator->msgData.idData, "GA", 2) == 0) { gnssType = GALILEO; }
	else if (strncmp((const char *)myNavigator->msgData.idData, "BD", 2) == 0) { gnssType = BEIDOU;  }
	else    																   { gnssType = GPS;     }

	register double X = *lat;
	register double Y = *lon;
	register double Z = *alt;

	register double sinB;

	D = sqrt(X * X + Y * Y);

	if (D == 0.0)
	{
		B = halfPi * Z / abs(Z);
		sinB = sin(B);

		*lat = degrees2dm(radianPi * B);
		*lon = 0.0;
		*alt = Z * sinB - gnss_a[gnssType] * sqrt(1.0 - gnss_e_pow2[gnssType] * sinB * sinB);

		return;
	}
	else
	{

		La = abs(asin(Y / D));

		if 		(Y  <  0.0 && X  >  0.0) { L = doublePi - La; }
		else if (Y  <  0.0 && X  <  0.0) { L = singlePi + La; }
		else if (Y  >  0.0 && X  <  0.0) { L = singlePi - La; }
		else if (Y  >  0.0 && X  >  0.0) { L = La;		 	  }
		else if (Y  == 0.0 && X  >  0.0) { L = 0.0; 	 	  }
		else if (Y  == 0.0 && X  <  0.0) { L = singlePi;	  }

		if (Z == 0.0)
		{
			*lat = 0.0;
			*lon = degrees2dm(radianPi * L);
			*alt = D - gnss_a[gnssType];

			return;
		}
		else
		{
			r = sqrt(X * X + Y * Y + Z * Z);
			c = asin(Z / r);
			p = gnss_a_e_pow2[gnssType] / r;

			register double s1;
			register double b;

			itN = 10;
			while (1)
			{
				s1 = 0.0;
				b = c + s1;
				s2 = asin(p * sin(b + b) / sqrt(1.0 - gnss_e_pow2[gnssType] * pow(sin(b), 2.0)));
				d = abs(s2 - s1);

				if (d < TRANSFORM_ACCURANCE || itN-- < 0)
				{
					sinB = sin(B);

					*lat = degrees2dm(radianPi * b);
					*lon = degrees2dm(radianPi * L);
					*alt = D * cos(B) + Z * sinB - gnss_a[gnssType] * sqrt(1.0 - gnss_e_pow2[gnssType] * sinB * sinB);

					return;
				}
				else { s1 = s2; }
			}
		}

	}

	return;
}

#elif GEO_DECART_TRANSFORM_METHOD == 2

static double n;
static double beta;
static double sin_beta;
static double sin_beta_pow2;
static double sin_beta_pow4;
static double sin_B0;
static double B0;
static double z0;
static double z0_pow2;
static double sin_pow2;
static double sin_pow4;
static double sin_pow6;

void decart2geo(struct MyNavigator* myNavigator, double* lat, double* lon, double* alt)
{
	// �������������� ��������� :
	n = (double)((int32_t)(*lon * 0.000001));

	beta = *lat * 1.5704606412372143E-07;
	sin_beta = sin(beta);
	sin_beta_pow2 = sin_beta * sin_beta;
	sin_beta_pow4 = sin_beta_pow2 * sin_beta_pow2;

	B0 = beta + sin(2.0 * beta) * (
		0.00252588685 - 0.00001491860 * sin_beta_pow2 +
		0.00000011904 * sin_beta_pow4);
	z0 = (*lon - (10.0 * n + 5.0) * 100000.0) * 1.5678293950765453E-07 / cos(B0);

	z0_pow2 = pow(z0, 2.0);
	sin_B0 = sin(B0);
	sin_pow2 = pow(sin_B0, 2.0);
	sin_pow4 = pow(sin_B0, 4.0);
	sin_pow6 = pow(sin_B0, 6.0);

	*lat = -z0_pow2 * sin(2.0 * B0) * (0.251684631 -
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
					0.00328 * sin_pow6))));

	*lon = z0 * (1.0 -
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

	// ������ :
	*lat += B0;
	*lat *= radianPi;

	// ������� :
	*lon += 0.10471975511965977 * (n - 0.5);
	*lon *= radianPi;

	// �� ���������� �������� � �������-���������� ������ :
	*lat = degrees2dm(*lat);
	*lon = degrees2dm(*lon);

	return;
}

#endif

inline double degrees2dm(double D)
{
	Dfloor = floor(D);
	return Dfloor * 100.0 + (D - Dfloor) * 60.0;
}
