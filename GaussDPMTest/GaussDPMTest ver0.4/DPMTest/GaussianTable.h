#pragma once
/*******************************************************************

GaussianTable.h.h		developed by naka_t	2011.02.23

	CRP�ł̃e�[�u���N���X

  Copyright (C) 2011  naka_t <naka_t@apple.ee.uec.ac.jp>
 *******************************************************************/
#include <vector>

//OpenCV�w�b�_
#include <cv.h>
#include <cxcore.h> 
#include <highgui.h>

//OpenCV���C�u����
#pragma comment (lib, "cv210.lib") 
#pragma comment( lib, "cxcore210.lib")
#pragma comment( lib, "highgui210.lib")


// �K�E�X���z�̃p�����^�\����
struct GaussianParams
{
	GaussianParams(int dim);
	GaussianParams(){ dim = 0; };
	
	void Create( int dim_ );				// �������m��
	void Init( double **data , int num );	// �^����ꂽ�f�[�^���珉���l������
	GaussianParams Clone();					// �V���ȃ������̈���m�ۂ��R�s�[

	// �K�E�X���z�̃p�����^
	cv::Mat X;
	cv::Mat C;
	cv::Mat m;
	cv::Mat S;
	int r;
	int nu;
	int N;
	int dim;
};


class CGaussianTable
{
public:
	CGaussianTable(GaussianParams &init);
	~CGaussianTable(void);

	void AddData( double *data );				// �e�[�u���Ƀf�[�^��ǉ�
	void DeleteData( double *data );			// �e�[�u������f�[�^���폜
	int GetNumData(){ return m_param.N; }		// �e�[�u���ɍ����Ă���l�̐l�����擾
	double CalcLogLikilihood( double *data );	// �f�[�^�����̃e�[�u���ɍ���ΐ��ޓx���v�Z

protected:
	GaussianParams m_param;		// �e�[�u���̃p�����^
	GaussianParams m_init;		// �����l

	double CalcLogPx( double *addData=NULL);
	void AddData( GaussianParams &p , double *data );
};
