#pragma once
#include "header.h"


#define AlignWidth_A_USTC 140
#define AlignHeight_A_USTC 160

struct LastPos  //存放矫正过后的人脸关键点位置坐标
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
};

class FaceAlign
{
public:
	FaceAlign(IplImage*,int*,int*);//注意这里必须输入灰度图像
	~FaceAlign();

	int runFaceAlign();
	
	IplImage* getAlignimg();//获取对齐后的人脸图片

	int* getFaceAlignPos();//获取对齐后的人脸关键点位置坐标

protected:
private:
	int width,height;

	int* rect;
	int* pos;

	IplImage* grayimg;
	IplImage* rotaimg;
	IplImage* alignimg;

	LastPos* Pos;

	CvMat* map;

	int* FinalPos;
	//这个10维数组存放的是10个人脸关键点的坐标
	//按顺序分别为左右眼，鼻尖，左右嘴角
};