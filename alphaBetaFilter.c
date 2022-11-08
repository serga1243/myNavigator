#include "alphaBetaFilter.h"

static double Zeks;
static double L;
static double r;

void alphaBetaFilter(struct MyNavigator* myNavigator, struct AlphaBetaFilterCoordinate* alphaBetaFilterCoordinate, struct Coordinate* coordinate)
{
    Zeks = 0;

    L = alphaBetaFilterCoordinate->Sp * alphaBetaFilterCoordinate->Tob * alphaBetaFilterCoordinate->Tob * alphaBetaFilterCoordinate->Sn;
    r = 0.25 * (4.0 + L - sqrt(L * (8.0 + L)));
    alphaBetaFilterCoordinate->Predel = 1.0 - r * r;

    switch (++alphaBetaFilterCoordinate->Step)
    {
    case 1:
        //alphaBetaFilterCoordinate->Vz = 100.0;
        alphaBetaFilterCoordinate->Az = 0.0;
        alphaBetaFilterCoordinate->fZ = coordinate->decodedPos.value;
        break;

    case 2:
        //alphaBetaFilterCoordinate->Vz = (coordinate->decodedPos.value - alphaBetaFilterCoordinate->fZ) / alphaBetaFilterCoordinate->Tob;
        alphaBetaFilterCoordinate->Az = 0.0;
        alphaBetaFilterCoordinate->fZ = coordinate->decodedPos.value;
        break;

    default:
        alphaBetaFilterCoordinate->Ka = 2.0 * (2.0 * (double)alphaBetaFilterCoordinate->Step - 1.0)
            / (double)(alphaBetaFilterCoordinate->Step * (double)alphaBetaFilterCoordinate->Step + (double)alphaBetaFilterCoordinate->Step);
        if (alphaBetaFilterCoordinate->Ka < alphaBetaFilterCoordinate->Predel) alphaBetaFilterCoordinate->Ka = alphaBetaFilterCoordinate->Predel;

        alphaBetaFilterCoordinate->Kb = 2.0 * (2.0 - alphaBetaFilterCoordinate->Ka) - 4.0 * sqrt(1.0 - alphaBetaFilterCoordinate->Ka);
        alphaBetaFilterCoordinate->Kg = alphaBetaFilterCoordinate->Kb * alphaBetaFilterCoordinate->Kb / (2.0 * alphaBetaFilterCoordinate->Ka);

        Zeks = alphaBetaFilterCoordinate->fZ
            + myNavigator->coordinates.vel.decodedPos.value
            * alphaBetaFilterCoordinate->Tob
            + alphaBetaFilterCoordinate->Az
            * alphaBetaFilterCoordinate->Tob
            * alphaBetaFilterCoordinate->Tob * 0.5;
        alphaBetaFilterCoordinate->fZ = Zeks + alphaBetaFilterCoordinate->Ka * (coordinate->decodedPos.value - Zeks);
        //alphaBetaFilterCoordinate->Vz = alphaBetaFilterCoordinate->Vz
        //    + alphaBetaFilterCoordinate->Kb / alphaBetaFilterCoordinate->Tob * (coordinate->decodedPos.value - Zeks);//���������� ��������
        alphaBetaFilterCoordinate->Az = alphaBetaFilterCoordinate->Az
            + (2 * alphaBetaFilterCoordinate->Kg
                / (alphaBetaFilterCoordinate->Tob * alphaBetaFilterCoordinate->Tob)) * (coordinate->decodedPos.value - Zeks); //���������� ���������
        break;
    }

    coordinate->filteredPos.value = alphaBetaFilterCoordinate->fZ;
    alphaBetaFilterCoordinate->Tob = 0;

    return;
}

void alphaBetaFilterInit(struct AlphaBetaFilterCoordinate* alphaBetaFilterCoordinate, const double T, const double Sp, const double Sn)
{
    //double Sp = 0.1;//��������� ����������
    //double Sn = 3.0;//��������� ����
    alphaBetaFilterCoordinate->Sp = Sp;
    alphaBetaFilterCoordinate->Sn = 1.0 / Sn;
    alphaBetaFilterCoordinate->Tob = T;
    L = Sp * alphaBetaFilterCoordinate->Tob * alphaBetaFilterCoordinate->Tob / Sn;
    r = (4.0 + L - sqrt(8.0 * L + L * L)) / 4.0;
    alphaBetaFilterCoordinate->Step = 0;
    alphaBetaFilterCoordinate->Predel = 1.0 - r * r;

    alphaBetaFilterCoordinate->Ka = 0.0;
    alphaBetaFilterCoordinate->Kb = 0.0;
    alphaBetaFilterCoordinate->Kg = 0.0;

    alphaBetaFilterCoordinate->Az = 0.0;
    alphaBetaFilterCoordinate->Vz = 0.0;
    alphaBetaFilterCoordinate->fZ = 0.0;

    return;
}
