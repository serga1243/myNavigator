// ���� ���� c++, � �� ��
#define C

// ����� � ����� ����� �������
// ��� ������� �� ����� ��������� :
#define MYNAVIGATOR_RELEASE
//#define MYNAVIGATOR_DEBUG

#define startLat 							4510.4065414
#define startLon 							8123.0149724
#define startAlt 							-13.017


#define TRANSFORM_COORDS							// ���� ���������� ��������������� ��� ���������� � ���������
#define WRITE_COORDS_IN_FLASH						// ���������� �� ���������� �� ���� ������?
#define MSG_MAX_LEN 						1024	// ������������ ����� ���������
#define PREVIOS_POS_LEN 					6		// ����� �������� ���������� ��������
#define LOG_INFO_ARR_LEN 					1		// ����� ������� �����

// ����� ���������� �����������? :
#define INCLUDE_LAT
#define INCLUDE_LON
#define INCLUDE_ALT
#define INCLUDE_VEL

// ��������� ������� ������� :
//#define KALMAN_FILTERING

// ��������� ���������� ������� :
#define MEDIAN_FILTERING

// ��������� ������������� ������� :
//#define MIN_QUAD_FILTERING

// ��������� �����-���� ������� :
//#define ALPHA_BETA_FILTERING

// ��������� ������ �� ����-������ ��� ������ ������
#define WRITE_IN_FLASH_START_ADRESS 		FLASH_USER_START_ADDR

// �������� ������ �� ����-������ ��� ������ ������
#define WRITE_IN_FLASH_END_ADRESS 			FLASH_USER_END_ADDR

#define GEO_DECART_TRANSFORM_METHOD 		1

// ������������ �������� ������� � �/� :
#define OBJECT_MAX_SPEED 8000.0

// ��������� ���������� ������� :
#define MEDIAN_FILTERING_BUFFER_LENGTH 		6

#ifdef MYNAVIGATOR_RELEASE

#undef WRITE_COORDS_IN_FLASH

#endif
