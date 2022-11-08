#include "decart2geo.h"

static double SGN;

#if GEO_DECART_TRANSFORM_METHOD == 1

extern const double gnss_a[];
extern const double gnss_e_pow2[];
extern const double gnss_e_pow2_1[];

static uint8_t gnssType;



void geo2decart(struct MyNavigator* myNavigator, double* lat, double* lon, double* alt)
{
	if 		(strncmp((const char *)myNavigator->msgData.idData, "GP", 2) == 0) { gnssType = GPS;     }
	else if (strncmp((const char *)myNavigator->msgData.idData, "GL", 2) == 0) { gnssType = GLONASS; }
	else if (strncmp((const char *)myNavigator->msgData.idData, "GA", 2) == 0) { gnssType = GALILEO; }
	else if (strncmp((const char *)myNavigator->msgData.idData, "BD", 2) == 0) { gnssType = BEIDOU;  }
	else    																   { gnssType = GPS;     }

	register double B = myfabs(*lat);
	B = floor(0.01 * B);
	B = dm2degrees(B, myfabs(*lat) - B * 100.0);
	B *= 0.017453292519943295;

	register double L = myfabs(*lon);
	L = floor(0.01 * L);
	L = dm2degrees(L, myfabs(*lon) - L * 100.0);

	register double sinB = sin(B);
	register double cosB = cos(B);

	register double H = *alt;
	register double N = gnss_a[gnssType] / (sqrt(1 - gnss_e_pow2[gnssType] * sinB * sinB));

	*lat = (N + H) * cosB * cos(L);
	*lon = (N + H) * cosB * sin(L);
	*alt = (gnss_e_pow2_1[gnssType] * N + H) * sinB;

	return;
}

#elif GEO_DECART_TRANSFORM_METHOD == 2

static double B;
static double L;
static double n;
static double l;
static double l_pow2;
static double sin_pow2;
static double sin_pow4;
static double sin_pow6;

void geo2decart(struct MyNavigator* myNavigator, double* lat, double* lon, double* alt)
{
	// �� ��������-���������� ����� � ���������� ������� :
	B = myfabs(*lat);
	B = floor(0.01 * B);
	B = dm2degrees(B, myfabs(*lat) - B * 100.0);
	B *= 0.017453292519943295;

	L = myfabs(*lon);
	L = floor(0.01 * L);
	L = dm2degrees(L, myfabs(*lon) - L * 100.0);

	// �������������� ��������� :
	n = (double)((int32_t)((6.0 + L) * 0.16666666666666667));

	l = (L - (3.0 + 6.0 * (n - 1.0))) * 0.017453292519943295;

	l_pow2 = pow(l, 2.0);
	sin_pow2 = pow(sin(B), 2.0);
	sin_pow4 = pow(sin(B), 4.0);
	sin_pow6 = pow(sin(B), 6.0);

	// �������� :
	*lat = 6367558.4968 * B -
		sin(2.0 * B) * (16002.8900 +
			66.9607 * sin_pow2 +
			0.3515 * sin_pow4 -
			l_pow2 * (1594561.25 +
				5336.535 * sin_pow2 +
				26.790 * sin_pow4 +
				0.149 * sin_pow6 +
				l_pow2 * (672483.4 -
					811219.9 * sin_pow2 +
					5420.0 * sin_pow4 -
					10.6 * sin_pow6 +
					l_pow2 * (278194.0 -
						830174.0 * sin_pow2 +
						572434.0 * sin_pow4 -
						16010.0 * sin_pow6 +
						l_pow2 * (109500.0 -
							574700.0 * sin_pow2 +
							836700.0 * sin_pow4 -
							398600.0 * sin_pow6)))));

	// �������� :
	*lon = (5.0 + 10.0 * n) * 100000.0 +
		l * cos(B) * (6378245.0 +
			21346.1415 * sin_pow2 +
			107.1590 * sin_pow4 +
			0.5977 * sin_pow6 +
			l_pow2 * (1070204.16 -
				2136826.66 * sin_pow2 +
				17.98 * sin_pow4 -
				11.99 * sin_pow6 +
				l_pow2 * (270806.0 -
					1523417.0 * sin_pow2 +
					1327645.0 * sin_pow4 -
					21701.0 * sin_pow6 +
					l_pow2 * (79690.0 -
						866190.0 * sin_pow2 +
						1730360.0 * sin_pow4 -
						945460.0 * sin_pow6))));

	return;
}

#endif

inline double dm2degrees(double D, double M)
{
	return SGN = D >= 0.0 && M >= 0.0 ?
		   (myfabs(D) + myfabs(M) * 0.01666666666666667) :
		   -(myfabs(D) + myfabs(M) * 0.01666666666666667);
}
