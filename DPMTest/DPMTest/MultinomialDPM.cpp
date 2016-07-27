#include "StdAfx.h"
#include <string.h>
#include <float.h>
#include <math.h>
#include <windows.h>
#include "randlib/randlib.h"
#include "MultinomialDPM.h"
#include "utility.h"

CMultinomialDPM::CMultinomialDPM(void)
{
	m_numData = 0;
	m_dimData = 0;
	m_data = NULL;
	m_tableID = NULL;
	m_gamma = DP_CONCPARA_PRIOR_A / DP_CONCPARA_PRIOR_B;
}

CMultinomialDPM::~CMultinomialDPM(void)
{
	Release();
}

void CMultinomialDPM::Release()
{
	delete [] m_tableID;
	m_tableID = NULL;
}

void CMultinomialDPM::SetData( double **data , int numData , int dim )
{
	Release();

	m_data = data;
	m_dimData = dim;
	m_numData = numData;
	m_tableID = new int[numData];
	for(int i=0 ; i<numData ; i++ ) m_tableID[i] = -1;

	m_tables.clear();
	m_tables.push_back( CMultinomialTable(dim) );			// ��̃e�[�u����ǉ�

	m_numTables.clear();
	m_numTables.reserve(1000);
}


void CMultinomialDPM::Updata()
{
	for(int d=0 ; d<m_numData ; d++ )
	{
		int t = m_tableID[d];

		if( t>=0 ) m_tables[t].DeleteData( m_data[d] );		// �f�[�^���폜
		DeleteEmptyTables();								// ��̃e�[�u�����폜
		t = Sampling( m_data[d] );							// �T���v�����O
		m_tableID[d] = t;
		m_tables[t].AddData( m_data[d] );					// �f�[�^�ǉ�

		// �Ō�̋�̃e�[�u���ɍ������ꍇ
		// �V���ɋ�̃e�[�u����ǉ�
		if( t==m_tables.size()-1 )
		{
			m_tables.push_back( CMultinomialTable(m_dimData) );
		}

		DeleteEmptyTables();					// ��̃e�[�u�����폜
	}

	m_gamma = SamplingGamma( m_gamma );
	m_numTables.push_back( (int)m_tables.size() -1 );
}

int CMultinomialDPM::Sampling( double *data )
{
	double *Pz = m_Pz;
	int numTable = (int)m_tables.size();
	int newTable = -1;
	double max = -DBL_MAX;

	// �e�[�u���̍Ō�͋�̃e�[�u��
	if( numTable == 1 ) return 0;
	
	// �����̃e�[�u���F�ΐ��ޓx
	for(int t=0 ; t<numTable ; t++ )
	{
		Pz[t] = m_tables[t].CalcLogLikilihood( data );
	}

	// �ő�l��T��
	for(int t=0 ; t<numTable ; t++ ) if( max < Pz[t] ) max = Pz[t];

	// �l���������Ȃ肷���邽�߁A�ő�l�ň���
	// �e�e�[�u���̐l�C��������
	// �T���v�����O�̂��߂ɗݐϊm���ɂ���
	Pz[0] = exp(Pz[0] - max) * m_tables[0].GetNumData();
	for(int t=1 ; t<numTable-1 ; t++ ) Pz[t] = Pz[t-1] + exp(Pz[t] - max) * m_tables[t].GetNumData(); 

	// �V���ȃe�[�u���𐶐�����m��
	Pz[numTable-1] = Pz[numTable-2] + exp(Pz[numTable-1] - max) * m_gamma;

	// �T���v�����O���邽�߂̗����𔭐�
	double rand = Rand() * Pz[numTable-1];

	// �v�Z�����m���ɏ]���ĐV���ȃe�[�u����I��
	for(newTable=0 ; newTable<numTable-1 ; newTable++ )
	{
		if( Pz[newTable] >= rand ) break;
	}

	return newTable;
}

void CMultinomialDPM::DeleteEmptyTables()
{
	int numTable = (int)m_tables.size() - 1;

	// ��̃e�[�u����T��
	for(int t=0 ; t<numTable ; t++ )
	{
		if( m_tables[t].GetNumData() == 0 )
		{
			DeleteTable( t );
			break;
		}
	}

}

void CMultinomialDPM::DeleteTable( int t )
{
	m_tables.erase( m_tables.begin() + t );

	// �������e�[�u����id���Â炷
	for(int d=0 ; d<m_numData ; d++ )
	{
		if( m_tableID[d] > t )
		{
			m_tableID[d]--;
		}
	}
}


void CMultinomialDPM::SaveModel( const char *dir )
{
	char dirname[256];
	char filename[256];
	FILE *fpPz, *fpTable;

	strcpy( dirname , dir );
	int len = (int)strlen( dir );
	if( len==0 || dir[len-1] != '\\' || dir[len-1] != '/' ) strcat( dirname , "\\" );
	
	CreateDirectory( dirname , NULL );

	if( m_numTables.size() )
	{
		sprintf( filename , "%sNumTables.txt" , dirname );
		SaveArray( m_numTables , (int)m_numTables.size() , filename );
	}

	// �e�N���X�ɑ�����m����ۑ�
	sprintf( filename , "%sLogLiklihood.txt" , dirname );
	fpPz = fopen( filename , "w" );
	sprintf( filename , "%sClusteringResult.txt" , dirname );
	fpTable = fopen( filename , "w" );
	for(int d=0 ; d<m_numData ; d++ )
	{
		double max = -DBL_MAX;
		int maxIdx = -1;
		for(int t=0 ; t<m_tables.size() ; t++ )
		{
			double lik = m_tables[t].CalcLogLikilihood( m_data[d] );
			fprintf( fpPz , "%lf	" ,  lik );
			if( max < lik )
			{
				max = lik;
				maxIdx = t;
			}
		}
		fprintf( fpPz , "\n" );
		fprintf( fpTable , "%d\n" , maxIdx );
	}
	fclose(fpPz);
	fclose(fpTable);
}

double CMultinomialDPM::Rand()
{
	unsigned int val;
	rand_s(&val);
	return (double)val/UINT_MAX;
}

double CMultinomialDPM::SamplingGamma( double oldGamma )
{
	for(int i=0 ; i<20 ; i++ )
	{
		float gammaB = 0;	// �K���}�֐��X�P�[���p�����[�^
		float gammaA = 0;	// �K���}�֐��`��p�����[�^
		int numTable = (int)m_tables.size() - 1;

		// �x�[�^���z����T���v������
		float w = genbet( (float)oldGamma+1 , (float)m_numData );

		// ��l�̕��z���T���v�����O
		int s = (Rand() * (oldGamma + numTable)) < numTable ? 1 : 0;
		gammaA = (float)(DP_CONCPARA_PRIOR_A + numTable - s);
		gammaB = (float)(DP_CONCPARA_PRIOR_B - log(w));

		// �X�V
		oldGamma = (double)gengam( gammaB , gammaA );
	}

	return oldGamma;
}
