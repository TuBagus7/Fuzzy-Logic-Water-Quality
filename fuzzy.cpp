#include "fuzzy.h"

float x1,x2,x3;
float a_pred1,a_pred2,a_pred3,a_pred4,a_pred5,a_pred6,a_pred7,a_pred8,a_pred9,a_pred10,a_pred11,a_pred12,a_pred13,a_pred14,a_pred15,a_pred16,a_pred17,a_pred18,a_pred19,a_pred20,a_pred21,a_pred22,a_pred23,a_pred24,a_pred25,a_pred26,a_pred27;
float z1,z2,z3,z4,z5,z6,z7,z8,z9,z10,z11,z12,z13,z14,z15,z16,z17,z18,z19,z20,z21,z22,z23,z24,z25,z26,z27;
float zTerbobot,shDingin,shSedang,shPanas,phAsam,phNormal,phBasa,tbRendah,tbSedang,tbTinggi;


float sh_Dingin(float x1)
{
    float a,b,miu;
    a = 0, b = 10;
    if(x1<=a)
    {
        miu=1;
    }
    else if(x1>a && x1<b)
    {
        miu=(b-x1)/(b-a);
    }
    else
    {
        miu=0;
    }
    return miu;
}

float sh_Sedang(float x1)
{
    float a,b,c,miu;
    a = 10, b = 30, c = 40;
    if (x1 <= a || x1 >= c)
    {
        miu = 0;
    }
    else if (x1 > a && x1 < b)
    {
        miu = (x1 - a) / (b - a);
    }
    else if (x1 > b && x1 < c)
    {
        miu = (c - x1) / (c - b);
    }
    else if (x1 == b)
    {
        miu = 1;
    }
    return miu;
}
float sh_Panas(float x1)
{
    float a,b,miu;
    a = 10, b = 40;
    if(x1<=a)
    {
        miu=0;
    }
    else if(x1>a && x1<b)
    {
        miu=(x1-a)/(b-a);
    }
    else
    {
        miu=1;
    }
    return miu;
}



//================ Fuzzyfication Nilai pH air ===========================
float ph_Asam(float x2)
{
    float a,b,miu;
    a = 0, b = 7;
    if(x2<=a)
    {
        miu=1;
    }
    else if(x2>a && x2<b)
    {
        miu=(b-x2)/(b-a);
    }
    else
    {
        miu=0;
    }
    return miu;
}

float ph_Normal(float x2){
    float a,b,c,miu;
    a = 5, b = 7, c = 9;
    if (x2 <= a || x2 >= c)
    {
        miu = 0;
    }
    else if (x2 > a && x2 < b)
    {
        miu = (x2 - a) / (b - a);
    }
    else if (x2 > b && x2 < c)
    {
        miu = (c - x2) / (c - b);
    }
    else if (x2 == b)
    {
        miu = 1;
    }
    return miu;
}

float ph_Basa(float x2)
{
    float a,b,miu;
    a = 7, b = 14;
    if(x2<=a)
    {
        miu=0;
    }
    else if(x2>a && x2<b)
    {
        miu=(x2-a)/(b-a);
    }
    else
    {
        miu=1;
    }
    return miu;
}

//================ Fuzzyfication Nilai Kekeruhan Air ===========================
float tb_Rendah(float x3)
{
    float a,b,miu;
    a = 0, b = 30;
    if(x3<=a)
    {
        miu=1;
    }
    else if(x3>a && x3<b)
    {
        miu=(b-x3)/(b-a);
    }
    else
    {
        miu=0;
    }
    return miu;
}

float tb_Sedang(float x3)
{
    float a,b,c,miu;
    a = 15, b = 30, c = 45;
    if (x3 <= a || x3 >= c)
    {
        miu = 0;
    }
    else if (x3 > a && x3 < b)
    {
        miu = (x3 - a) / (b - a);
    }
    else if (x3 > b && x3 < c)
    {
        miu = (c - x3) / (c - b);
    }
    else if (x3 == b)
    {
        miu = 1;
    }
    return miu;
}


float tb_Tinggi(float x3){
   float a,b,miu;
    a = 30, b = 150;
    if(x3<=a)
    {
        miu=0;
    }
    else if(x3>a && x3<b)
    {
        miu=(x3-a)/(b-a);
    }
    else
    {
        miu=1;
    }
    return miu;
}
//================ NILAI A_PREDIKAT Fuzzyfication Persenstase banjir ===========================
// float pb(float m1, float m2, float m3)
// {
//     float a1,a2,a_pred;
//     if (m1>m2){
//         a1 = m2;
//     }else{
//         a1 = m1;
//     }

//     if (m1>m3){
//         a2 = m3;
//     }else{
//         a2 = m1;
//     }

//     if (a1>a2){
//         a_pred = a2;
//     }else{
//         a_pred = a1;
//     }

//     return a_pred;
// }

float pb(float m1, float m2, float m3) {
    return min(m1, min(m2, m3));
}

//================ NILAI Z , Fuzzyfication Nilai Perentase Kebersihan Air ===========================
float layak(float a_pred)
{
    float a=0,b=30,z;
    z = b-(a_pred*(b-a));

    return z;
}
float sedang(float a_pred)
{
    float a=31,b=70,z;
    z = b-(a_pred*(b-a));

    return z;
}
float buruk(float a_pred)
{
    float a=71,b=90,z;
    z = b-(a_pred*(b-a));

    return z;
}

// float layak(float a_pred) {
//     float a = 0, b = 30;
//     return a + a_pred * (b - a);  // Naik
// }

// float sedang(float a_pred) {
//     float a = 31, b = 70;
//     return a + a_pred * (b - a);  // Naik
// }

// float buruk(float a_pred) {
//     float a = 71, b = 90;
//     return b - a_pred * (b - a);  // Turun
// }


