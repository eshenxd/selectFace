#include "header.h"
#include "facePoint.h"

using namespace std;



bool FacePoints::init_asm()
{
	if (fit_asm.Read("Point.amf") == false)//my68-1d.amf添加在工程的文件夹中
	{
		return false;
	}
	else
		return true;
}

void FacePoints::free_shape_memeory(asm_shape** shapes)
{
	delete [](*shapes);
}


FacePoints::FacePoints(IplImage* img)
{
	
	pos=new int[10];

	width=img->width;

	height=img->height;

	gray_img=cvCreateImage(cvSize(width,height),8,1);

	if(img->nChannels!=1)
	{
		cout<<"ERROE：Please input a gray image file!"<<endl;
	}
	else
	{
		cvCopy(img,gray_img);

		facePoint=new _Rect_Point[MAX_FACE_NUM_POINT_P_USTC];

		facepos=new new_Rect[MAX_FACE_NUM_POINT_P_USTC];

		if(!init_asm())cout<<"lost asm files"<<endl;

		detshapes=new asm_shape[MAX_FACE_NUM_POINT_P_USTC];

		shapes=new asm_shape[MAX_FACE_NUM_POINT_P_USTC];
	}
}

FacePoints::~FacePoints()
{
	delete[] pos;

	if(gray_img)
		cvReleaseImage(&gray_img);

	if(facePoint)
		delete[] facePoint;

	if(facepos)
		delete facepos;

	delete[] shapes;

	free_shape_memeory(&detshapes);
}

int FacePoints::runFacePoints(int facesnum,int rect[MAX_FACE_NUM_POINT_P_USTC*4])
{
	int count=0;

	int nFaces=facesnum;

	for(int i=0;i<nFaces;i++)
	{
		detshapes[i].Resize(2);

		detshapes[i][0].x=(float)rect[count++];
		detshapes[i][0].y=(float)rect[count++];
		detshapes[i][1].x=(float)rect[count++];
		detshapes[i][1].y=(float)rect[count++];
	}
	
	for(int i=0;i<nFaces;i++)
	{
		InitShapeFromDetBox(shapes[i], detshapes[i], fit_asm.GetMappingDetShape(), fit_asm.GetMeanFaceWidth());
	}
	

	fit_asm.Fitting2(shapes, nFaces, gray_img,NUMUSTC_P_USTC);

	for(int i=0;i<nFaces;i++)
	{
		facePoint[i].left_eye_x=cvRound((shapes[i])[31].x);
		facePoint[i].left_eye_y=cvRound((shapes[i])[31].y);

		facePoint[i].right_eye_x=cvRound((shapes[i])[36].x);
		facePoint[i].right_eye_y=cvRound((shapes[i])[36].y);


		facePoint[i].nose_x=cvRound((shapes[i])[67].x);
		facePoint[i].nose_y=cvRound((shapes[i])[67].y);

		facePoint[i].left_mouth_x=cvRound((shapes[i])[48].x);
		facePoint[i].left_mouth_y=cvRound((shapes[i])[48].y);

		facePoint[i].right_mouth_x=cvRound((shapes[i])[54].x);
		facePoint[i].right_mouth_y=cvRound((shapes[i])[54].y);
	}

	return 0;
}

int* FacePoints::getFacePoints()
{
	int faceIdx=0;

	pos[0]=facePoint[faceIdx].left_eye_x;
	pos[1]=facePoint[faceIdx].left_eye_y;

	pos[2]=facePoint[faceIdx].right_eye_x;
	pos[3]=facePoint[faceIdx].right_eye_y;

	pos[4]=facePoint[faceIdx].nose_x;
	pos[5]=facePoint[faceIdx].nose_y;

	pos[6]=facePoint[faceIdx].left_mouth_x;
	pos[7]=facePoint[faceIdx].left_mouth_y;

	pos[8]=facePoint[faceIdx].right_mouth_x;
	pos[9]=facePoint[faceIdx].right_mouth_y;

	return pos;
}