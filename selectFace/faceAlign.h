#pragma once
#include "header.h"


#define AlignWidth_A_USTC 140
#define AlignHeight_A_USTC 160

struct LastPos  //��Ž�������������ؼ���λ������
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
	FaceAlign(IplImage*,int*,int*);//ע�������������Ҷ�ͼ��
	~FaceAlign();

	int runFaceAlign();
	
	IplImage* getAlignimg();//��ȡ����������ͼƬ

	int* getFaceAlignPos();//��ȡ�����������ؼ���λ������

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
	//���10ά�����ŵ���10�������ؼ��������
	//��˳��ֱ�Ϊ�����ۣ��Ǽ⣬�������
};