#include "decart2geo.h"

static double B;
static double L;
static double n;
static double l;
static double l_pow2;
static double sin_pow2;
static double sin_pow4;
static double sin_pow6;

void geo2decart(double* lat, double* lon)
{
	B = *lat;
	L = *lon;

	n;
	modf((6.0 + L) / 6.0, &n);

	l = (L - (3.0 + 6.0 * (n - 1.0))) / 57.29577951;

	l_pow2 = pow(l, 2.0);
	sin_pow2 = pow(sin(B), 2.0);
	sin_pow4 = pow(sin(B), 4.0);
	sin_pow6 = pow(sin(B), 6.0);

	*lat = 6367558.4968 * B -
		sin(2.0 * B * (16002.8900 +
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
							398600.0 * sin_pow6))))));



	*lon = (5.0 + 10.0 * n) * 100000.0 +
		l * cos(B * (6378245.0 +
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
						945460.0 * sin_pow6)))));

	return;
}