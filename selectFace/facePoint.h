#pragma once
#ifndef GETFACEPOINT_H

#define GETFACEPOINT_H

#include "../include/asmfitting.h"
#include "../include/asmbuilding.h"
#include "../include/asmlibrary.h"

#pragma comment(lib, "../lib/asmlibrary.lib")

#define MAX_FACE_NUM_POINT_P_USTC  1 //һ֡������������
#define  NUMUSTC_P_USTC 30
#define SMALL_LEN_P_USTC 320

//��չʹ��
#define PointWidth_P_USTC 300
#define PointHeight_P_USTC 300

struct new_Rect 
{

	int left;
	int top;
	int right;
	int bottom;
};//����λ��

struct _Rect_Point
{

	int left_eye_x;
	int left_eye_y;

	int right_eye_x;
	int right_eye_y;

	int nose_x;
	int nose_y;

	int left_mouth_x;
	int left_mouth_y;

	int right_mouth_x;
	int right_mouth_y;
};//�ؼ���λ��

class FacePoints
{
public:
	FacePoints(IplImage*);/*���ﴫ���Ӧ����һģ��õ��ĻҶ�ͼ��*/
	~FacePoints();

	int runFacePoints(int , int* rect);

	/*
	   ��������һ��10ά���飬��ʾ5���������
	   ˳�����������ۣ��Ǽ⣬�������
	*/
	int* getFacePoints();

protected:
private:
	asm_shape *shapes, *detshapes ;

	asmfitting fit_asm;

	_Rect_Point* facePoint;
	new_Rect* facepos;

	int width,height;

	IplImage* gray_img;

	int* pos;

	bool init_asm();

	void free_shape_memeory(asm_shape** shapes);
};

#endif