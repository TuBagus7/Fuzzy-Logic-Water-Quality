#ifndef FUZZY_H
#define FUZZY_H
#include <Arduino.h>

extern float x1,x2,x3;
extern float a_pred1,a_pred2,a_pred3,a_pred4,a_pred5,a_pred6,a_pred7,a_pred8,a_pred9,a_pred10,a_pred11,a_pred12,a_pred13,a_pred14,a_pred15,a_pred16,a_pred17,a_pred18,a_pred19,a_pred20,a_pred21,a_pred22,a_pred23,a_pred24,a_pred25,a_pred26,a_pred27;
extern float z1,z2,z3,z4,z5,z6,z7,z8,z9,z10,z11,z12,z13,z14,z15,z16,z17,z18,z19,z20,z21,z22,z23,z24,z25,z26,z27;
extern float zTerbobot,shDingin,shSedang,shPanas,phAsam,phNormal,phBasa,tbRendah,tbSedang,tbTinggi;

extern float sh_Dingin(float x1);
extern float sh_Sedang(float x1);
extern float sh_Panas(float x1);

extern float ph_Asam(float x2);
extern float ph_Normal(float x2);
extern float ph_Basa(float x2);

extern float tb_Rendah(float x3);
extern float tb_Sedang(float x3);
extern float tb_Tinggi(float x3);

extern float pb(float m1, float m2, float m3);

extern float layak(float a_pred);
extern float sedang(float a_pred);
extern float buruk(float a_pred);

extern void fuzzyfikasi();

#endif

/*
        sh_Panas --> sh_Panas
        sh_Sedang --> sh_Sedang
        sh_Dingin --> sh_Dingin


        ph_Basa --> ph_Panas
        ph_Normal --> ph_Normal
        ph_Asam --> ph_Asam


        tb_Tinggi --> tb_Tingg
        tb_Sedang --> tb_Sedang
        tb_Rendah --> tb_Rendah

*/