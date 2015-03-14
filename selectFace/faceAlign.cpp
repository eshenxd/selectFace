#include "faceAlign.h"


FaceAlign::FaceAlign(IplImage* img,int* rect_C,int* pos_C)
{
	width=img->width;

	height=img->height;

	rect=rect_C;

	pos=pos_C;

	FinalPos=new int[10];

	grayimg=cvCreateImage(cvSize(width,height),8,1);

	rotaimg=cvCreateImage(cvSize(width,height),8,1);

	alignimg=cvCreateImage(cvSize(AlignWidth_A_USTC,AlignHeight_A_USTC),8,1);

	cvCopy(img,grayimg);

	Pos=new LastPos[1];

	map=cvCreateMat(2,3,CV_32FC1);

}

FaceAlign::~FaceAlign()
{
	if(grayimg)
		cvReleaseImage(&grayimg);

	if(rotaimg)
		cvReleaseImage(&rotaimg);

	if(alignimg)
		cvReleaseImage(&alignimg);

	if(Pos)
		delete[] Pos;

	if(map)
		cvReleaseMat(&map);
}

int FaceAlign::runFaceAlign()
{
	CvPoint2D32f  left_point;
	CvPoint2D32f  right_point;

	left_point.x=(float)pos[0];
	left_point.y=(float)pos[1];
	right_point.x=(float)pos[2];
	right_point.y=(float)pos[3];

	CvPoint rota_point= cvPoint(cvRound((left_point.x + right_point.x)/2), 
		cvRound((left_point.y + right_point.y)/2));

	float xDis, yDis;
	xDis = right_point.x - left_point.x;
	yDis = right_point.y - left_point.y;

	float m_angle = cvFastArctan(yDis, xDis);

	cv2DRotationMatrix(cvPoint2D32f(rota_point.x, rota_point.y), m_angle, 1, map); // ÏàËÆ±ä»»
	cvWarpAffine(grayimg, rotaimg, map);


	//Ðý×ªµã
	//int last[10];
	float x=rota_point.x;
	float y=rota_point.y;

	float mouth_center_x=(float)((pos[6]+pos[8])/2);
	float mouth_center_y=(float)((pos[7]+pos[9])/2);

	float dis_eye=sqrt((float)(((pos[0]-pos[2])*(pos[0]-pos[2]))+((pos[1]-pos[3])*(pos[1]-pos[3]))));

	float dis_nose=sqrt((float)(((x-pos[4])*(x-pos[4]))+((y-pos[5])*(y-pos[5]))));

	float dis_mouth_center=sqrt((float)((x-mouth_center_x)*(x-mouth_center_x)+(y-mouth_center_y)*(y-mouth_center_y)));

	float dis_mouth=sqrt((float)(((pos[6]-pos[8])*(pos[6]-pos[8]))+((pos[7]-pos[9])*(pos[7]-pos[9]))));

	float zoom=abs(pos[2]-pos[0])/(AlignWidth_A_USTC/2.0);
	CvRect facerect;
	facerect.x=cvRound(pos[0]-(AlignWidth_A_USTC/4.0)*zoom);
	facerect.y=cvRound(pos[1]-(AlignWidth_A_USTC/4.0)*zoom);
	facerect.width=cvRound(AlignWidth_A_USTC*zoom);
	facerect.height=cvRound(AlignHeight_A_USTC*zoom);


	cvSetImageROI(rotaimg,facerect);
	cvResize(rotaimg,alignimg);
	cvResetImageROI(rotaimg);

	//×óÑÛ
	Pos[0].left_eye_x=cvRound(AlignWidth_A_USTC/4);
	Pos[0].left_eye_y=cvRound(AlignWidth_A_USTC/4);
	//ÓÒÑÛ
	Pos[0].right_eye_x=3*cvRound(AlignWidth_A_USTC/4);
	Pos[0].right_eye_y=cvRound(AlignWidth_A_USTC/4);
	//±Ç×Ó
	Pos[0].nose_x=2*cvRound(AlignWidth_A_USTC/4);
	Pos[0].nose_y=cvRound(AlignWidth_A_USTC/4)+cvRound(dis_nose/zoom);
	//×ó×ì½Ç
	Pos[0].left_mouth_x=2*cvRound(AlignWidth_A_USTC/4)-cvRound(dis_mouth/(2.0*zoom));
	Pos[0].left_mouth_y=cvRound(AlignWidth_A_USTC/4)+cvRound(dis_mouth_center/zoom);
	//ÓÒ×ì½Ç
	Pos[0].right_mouth_x=2*cvRound(AlignWidth_A_USTC/4)+cvRound(dis_mouth/(2.0*zoom));
	Pos[0].right_mouth_y=cvRound(AlignWidth_A_USTC/4)+cvRound(dis_mouth_center/zoom);

	return 0;
}

int* FaceAlign::getFaceAlignPos()
{
	FinalPos[0]=Pos[0].left_eye_x;
	FinalPos[1]=Pos[0].left_eye_y;
	FinalPos[2]=Pos[0].right_eye_x;
	FinalPos[3]=Pos[0].right_eye_y;
	FinalPos[4]=Pos[0].nose_x;
	FinalPos[5]=Pos[0].nose_y;
	FinalPos[6]=Pos[0].left_mouth_x;
	FinalPos[7]=Pos[0].left_mouth_y;
	FinalPos[8]=Pos[0].right_mouth_x;
	FinalPos[9]=Pos[0].right_mouth_y;

	return FinalPos;
}

IplImage* FaceAlign::getAlignimg()
{
	return alignimg;
}