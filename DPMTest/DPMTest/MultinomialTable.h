#pragma once
/*******************************************************************

MultinomialTable.h.h		developed by naka_t	2011.02.01

	CRP�ł̃e�[�u���N���X

�@�����J�p�Ƀv���O�����𐮗�				naka_t	2010.02.01

  Copyright (C) 2011  naka_t <naka_t@apple.ee.uec.ac.jp>
 *******************************************************************/
#include <vector>
#define DPM_ALPHA	50.0	// �f�B���N�����O���z�̃p�����[�^


class CMultinomialTable
{
public:
	CMultinomialTable(int dim);
	~CMultinomialTable(void);

	void AddData( double *data );
	void DeleteData( double *data );
	int GetNumData(){ return m_numData; }
	double CalcLogLikilihood( double *data );

protected:
	std::vector<double> m_alpha;
	int m_numData;
	int m_dim;

	// �f�B�N�����O���z�̒萔�������̌v�Z
	double CalcLogZ( double *alpha , double *addData=NULL);
};
