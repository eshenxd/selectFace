#include "header.h"
#include "getFiles.h"
#include "detect_face.h"
#include "facePoint.h"
#include "faceAlign.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[]){

	string foldpath="../config/foldpath.ini";
	string filepath;
	vector<string>filename;

	/*
	   ��ȡ�ļ���·��
	*/
	ifstream fp;

	fp.open(foldpath, ios::in);

	fp >> filepath;

	fp.close();


	/*
	   ��ȡ�ļ�����ͼƬ�ļ�
	*/

	getFiles(filepath, filename);

	
	int* pos = new int[4];
	char fname[20];
	char ext[20];

	int* position = new int[10];

	for (unsigned int ix = 0; ix < filename.size(); ix++){

		/*
		    ��ȡͼƬ����
		*/
		_splitpath(filename[ix].c_str(), NULL, NULL, fname, ext);

		string saveName = fname;

		saveName = saveName + ext;

		/*
		    �������
		*/

		IplImage* image = cvLoadImage(filename[ix].c_str(), 1);

		FaceDetector fd(image);

		fd.runFaceDetector();

		int faceNum = fd.getDetectFaceCount();

		if (faceNum == 1){

			fd.getDetectFacePos(0, pos);

			CvRect rect = {pos[0],pos[1],pos[2]-pos[0],pos[3]-pos[1]};

			/*
			   facepoints
			*/

			FacePoints fp(fd.getImageGray());

			fp.runFacePoints(1, pos);

			

			position = fp.getFacePoints();

			/*
			   facealign
			*/
			FaceAlign fa(fd.getImageGray(),pos,position);

			fa.runFaceAlign();

			saveName = "../image/" + saveName;

			const char* saveFile = saveName.c_str();

			cvSaveImage(saveFile, fa.getAlignimg());

			position = NULL;
		}
	}
	



	delete[] position;
	delete[] pos;


	return 0;
}