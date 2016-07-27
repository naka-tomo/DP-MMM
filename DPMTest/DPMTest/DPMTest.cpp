
/*******************************************************************

DPMTest.cpp		developed by naka_t	2011.02.01

	DPM�N���X�̎g�p��

  Copyright (C) 2011  naka_t <naka_t@apple.ee.uec.ac.jp>
 *******************************************************************/
#include "stdafx.h"
#include "MultinomialDPM.h"
#include "utility.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CMultinomialDPM dpm;	// DPM�N���X
	int num, dim;			// �f�[�^���Ǝ���

	// �f�[�^�̓ǂݍ���
	double **data = LoadMatrix<double>( dim , num , "sample.txt" );

	// �f�[�^��n��
	dpm.SetData( data , num , dim );

	// �p�����^�̍X�V
	for(int i=0 ; i<100 ; i++ )
	{
		dpm.Updata();
	}

	// �w�K���ʕۑ�
	dpm.SaveModel( "result" );

	Free( data );

	return 0;
}

