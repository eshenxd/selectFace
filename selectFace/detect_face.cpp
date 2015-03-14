/*****************************************
功能简介：detect faces in a picture
作    者：Ethan
编辑时间：2014年10月22日
*****************************************/

#include "detect_face.h"

using namespace std;

char* cascade_name_faces="face.xml";

FaceDetector::FaceDetector(IplImage* img)
{
	width=img->width;
	height=img->height;

	//double paramtmp[4] = { 1.2, 1.6, 2.0, 3.0 };

	//param = paramtmp;
	//param = { 1.2, 1.6, 2.0, 3.0 };

	scale=2;

	Frame=cvCreateImage(cvSize(width,height),img->depth,img->nChannels);
	cvCopy(img,Frame);

	grayFrame=cvCreateImage(cvSize(width,height),8,1);
	small_img=cvCreateImage(cvSize(cvRound(width/scale),cvRound(height/scale)),8,1);

	storage_faces=cvCreateMemStorage(0);

	cascade_faces = (CvHaarClassifierCascade*)cvLoad( cascade_name_faces, 0, 0, 0 );

	faceCount=0;

	facePos=new _Rect[MAX_FACE_COUNT_D_USTC];

}

FaceDetector::~FaceDetector()
{
	if(Frame)
		cvReleaseImage(&Frame);
	
	if(grayFrame)
		cvReleaseImage(&grayFrame);

	if(small_img)
		cvReleaseImage(&small_img);

	if(facePos)
		delete[] facePos;

	cvReleaseMemStorage(&storage_faces);
	cvReleaseHaarClassifierCascade(&cascade_faces);
}

IplImage* FaceDetector::getImageGray(){

	return grayFrame;
}

int FaceDetector::runFaceDetector()
{
	if(Frame->nChannels==3)
	{
		cvCvtColor(Frame,grayFrame,CV_BGR2GRAY);
	}
	
	else
		cvCopy(Frame,grayFrame);

	if(!cascade_faces)
	{
		cout<<"ERROR: Could not load classifier cascade"<<endl;

		system("pause");
	}
	
	CvSeq* faces = NULL;

	cvClearMemStorage( storage_faces );

	cvResize( grayFrame , small_img , CV_INTER_LINEAR );

	//对图像进行直方图均衡化，也即增强图像的亮度和对比度
	cvEqualizeHist( small_img, small_img);
	
	int i = 0;//初始化迭代次数

	double param[4] = { 1.2, 1.6, 2.0, 3.0 };

	for (int i = 0; i < 4 ; i++){

		faces = cvHaarDetectObjects(small_img, cascade_faces, storage_faces,
			param[i], 2.0, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
			cvSize(20, 20));

		if (faces->total == 1)break;
	}

	//faces=cvHaarDetectObjects(small_img, cascade_faces, storage_faces,
	//	1.1, 2.0, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
	//	cvSize(20, 20) );

	faceCount=faces->total;

	for(int i = 0; i < (faces ? faces->total : 0); i++ )
	{
		CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

		//保存人脸的位置
		facePos[i].left=cvRound(r->x*scale);
		facePos[i].top=cvRound(r->y*scale);
		facePos[i].right=facePos[i].left+cvRound(r->width*scale);
		facePos[i].bottom=facePos[i].top+cvRound(r->height*scale);
	}

	return 0;
}

int FaceDetector::getDetectFaceCount()
{
	return faceCount;
}

int FaceDetector::getDetectFacePos(int faceIdx, int pos[4])
{
	if(faceIdx>=getDetectFaceCount())
	{
		cout<<"There's no enough face!"<<endl;

		return -1;
	}

	pos[0] = facePos[faceIdx].left;
	pos[1] = facePos[faceIdx].top;
	pos[2] = facePos[faceIdx].right;
	pos[3] = facePos[faceIdx].bottom;

	return 0;
}
