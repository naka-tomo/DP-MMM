#pragma once
/*******************************************************************

MultinomialDPM.h		developed by naka_t	2011.02.01

	�������z Dirichlet Process Mixture

�@�����J�p�Ƀv���O�����𐮗�				naka_t	2010.02.01

  Copyright (C) 2011  naka_t <naka_t@apple.ee.uec.ac.jp>
 *******************************************************************/
#include "MultinomialTable.h"

#define MAX_TABLES 100				// �e�[�u���̍ő吔�i�v�Z�p�o�b�t�@�m�ۂɕK�v�j
#define DP_CONCPARA_PRIOR_A 0.1		// Concentration parameter�̃K���}���O���z�̃p�����^
#define DP_CONCPARA_PRIOR_B 0.1		// Concentration parameter�̃K���}���O���z�̃p�����^

class CMultinomialDPM
{
public:
	CMultinomialDPM(void);
	~CMultinomialDPM(void);

	void SetData( double **data , int numData , int dim );	// �f�[�^�̃Z�b�g
	void Updata();											// �p�����[�^�X�V
	void SaveModel( const char *dir );						// ���f���̕ۑ�

protected:
	void Release();								// ���������
	int Sampling( double *data );				// �q������e�[�u���̃T���v�����O
	double SamplingGamma( double oldGamma );	// concentrate paramter �̃T���v�����O
	void DeleteEmptyTables();					// ��̃e�[�u�����폜
	void DeleteTable( int t );					// ����̃e�[�u�����폜
	double Rand();								// 0�`1�̗�������

	int m_numData;								// �f�[�^��
	int m_dimData;								// �f�[�^�̎���
	double **m_data;							// �f�[�^
	int *m_tableID;								// �f�[�^���������e�[�u��
	double m_gamma;
	std::vector<CMultinomialTable> m_tables;	// �f�[�^������e�[�u��
	std::vector<int> m_numTables;				// �e��ł̃e�[�u���̐�
	double m_Pz[MAX_TABLES];					// �v�Z�p�o�b�t�@

};

