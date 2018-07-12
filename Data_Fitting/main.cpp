#include <stdio.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string.h>
using namespace std;
const double PRECISION = 1E-10;

#define ARR_INDEX(row, col, dim)  ((row) * (dim) + (col))


bool IsPrecisionZero(double value);
bool IsPrecisionZero(double value)
{
    if(std::fabs(value) < PRECISION)
    {
        return true;
    }

    return false;
}

class GuassEquation
{
public:
    GuassEquation(int M);
    GuassEquation(int M, double *A, double *b);
    virtual ~GuassEquation();

    void Init(int M, double *A, double *b);
    double& operator()(int row, int col);
    const double& operator()(int row, int col) const;

    bool Resolve(std::vector<double>& xValue);
#ifdef _DEBUG
    void DebugDump();
#endif
protected:
    int SelectPivotalElement(int column);
    void SwapRow(int row1, int row2);
    void SimplePivotalRow(int row, int startColumn);
    void RowElimination(int rowS, int rowE, int startColumn);

protected:
    double *m_matrixA;
    int m_DIM;
    double *m_bVal;
};
GuassEquation::GuassEquation(int M)
{
    m_matrixA = NULL;
    m_bVal = NULL;
    Init(M, NULL, NULL);
}

GuassEquation::GuassEquation(int M, double *A, double *b)
{
    m_matrixA = NULL;
    m_bVal = NULL;
    Init(M, A, b);
}

GuassEquation::~GuassEquation()
{
    if(m_matrixA)
    {
        delete[] m_matrixA;
        m_matrixA = NULL;
    }
    if(m_bVal)
    {
        delete[] m_bVal;
        m_bVal = NULL;
    }
}

void GuassEquation::Init(int M, double *A, double *b)
{
    assert((m_matrixA == NULL) && (m_bVal == NULL));

    m_DIM = M;
    m_matrixA = new double[m_DIM * m_DIM];
    if(m_matrixA)
    {
        m_bVal = new double[m_DIM];
        if(m_bVal)
        {
            if(A)
            {
                memcpy(m_matrixA, A, sizeof(double) * m_DIM * m_DIM);
            }
            if(b)
            {
                memcpy(m_bVal, b, sizeof(double) * m_DIM);
            }
        }
        else
        {
            delete[] m_matrixA;
            m_matrixA = NULL;
        }
    }
}

double& GuassEquation::operator()(int row, int col)
{
    assert((row < m_DIM) && (col < m_DIM));

    double (*rowPtr)[] = (double (*)[])m_matrixA;
    return *(m_matrixA + row * m_DIM + col);
}

const double& GuassEquation::operator()(int row, int col) const
{
    assert((row < m_DIM) && (col < m_DIM));

    return *(m_matrixA + row * m_DIM + col);
}

/*������Ԫ�ĸ�˹��ȥ���ⷽ���飬���Ľ���matrixA�ĶԽ�����*/
bool GuassEquation::Resolve(std::vector<double>& xValue)
{
    assert(xValue.size() == m_DIM);

    /*��Ԫ���õ���������*/
    for(int i = 0; i < m_DIM - 1; i++)
    {
        /*����ѡ��Ԫ*/
        int pivotRow = SelectPivotalElement(i);
        if(pivotRow != i)/*����б�Ҫ��������*/
        {
            SwapRow(i, pivotRow);
        }
        if(IsPrecisionZero(m_matrixA[i * m_DIM + i])) /*��Ԫ��0? ������Ψһ��*/
        {
            return false;
        }
        /*��ϵ����һ������ʹ�е�һ��ϵ����1.0*/
        SimplePivotalRow(i, i);
        /*���н�����Ԫ*/
        for(int j = i + 1; j < m_DIM; j++)
        {
            RowElimination(i, j, i);
        }
    }
    /*�ش����*/
    m_matrixA[(m_DIM - 1) * m_DIM + m_DIM - 1] = m_bVal[m_DIM - 1] / m_matrixA[(m_DIM - 1) * m_DIM + m_DIM - 1];
    for(int i = m_DIM - 2; i >= 0; i--)
    {
        double totalCof = 0.0;
        for(int j = i + 1; j < m_DIM; j++)
        {
            totalCof += m_matrixA[i * m_DIM + j] * m_matrixA[j * m_DIM + j];
        }
        m_matrixA[i * m_DIM + i] = (m_bVal[i] - totalCof) / m_matrixA[i * m_DIM + i];
    }

    /*���Խ���Ԫ�صĽ�������������*/
    for(int i = 0; i < m_DIM; i++)
    {
        xValue[i] = m_matrixA[i * m_DIM + i];
    }

    return true;
}

#ifdef _DEBUG
void GuassEquation::DebugDump()
{
    for(int i = 0; i < m_DIM; i++)
    {
        for(int j = 0; j < m_DIM; j++)
        {
            std::cout << m_matrixA[i * m_DIM + j] << " ";
        }
        std::cout << "| " << m_bVal[i] << std::endl;
    }
}
#endif

/*M��ά�ȣ�columnȡֵ��Χ��0��M-1*/
int GuassEquation::SelectPivotalElement(int column)
{
    assert(column < m_DIM);

    int row = column;
    double maxU = std::fabs(m_matrixA[column * m_DIM + column]);

    for(int i = column + 1; i < m_DIM; i++)
    {
        if(std::fabs(m_matrixA[i * m_DIM + column]) > maxU)
        {
            maxU = std::fabs(m_matrixA[i * m_DIM + column]);
            row = i;
        }
    }

    return row;
}

void GuassEquation::SwapRow(int row1, int row2)
{
    assert((row1 < m_DIM) && (row2 < m_DIM));

    double temp;
    for(int i = 0; i < m_DIM; i++)
    {
        temp = m_matrixA[row1 * m_DIM + i];
        m_matrixA[row1 * m_DIM + i] = m_matrixA[row2 * m_DIM + i];
        m_matrixA[row2 * m_DIM + i] = temp;
    }
    temp = m_bVal[row1];
    m_bVal[row1] = m_bVal[row2];
    m_bVal[row2] = temp;
}

void GuassEquation::SimplePivotalRow(int row, int startColumn)
{
    assert((row < m_DIM) && (startColumn < m_DIM));

    double simple = m_matrixA[row * m_DIM + startColumn];

    for(int i = startColumn; i < m_DIM; i++)
    {
        m_matrixA[row * m_DIM + i] /= simple;
    }
    m_bVal[row] /= simple;
}

void GuassEquation::RowElimination(int rowS, int rowE, int startColumn)
{
    assert((rowS < m_DIM) && (rowE < m_DIM) && (startColumn < m_DIM));

    double simple = m_matrixA[rowE * m_DIM + startColumn];

    for(int i = startColumn; i < m_DIM; i++)
    {
        m_matrixA[rowE * m_DIM + i] -= m_matrixA[rowS * m_DIM + i] * simple;
    }
    m_bVal[rowE] -= m_bVal[rowS] * simple;
}


typedef std::vector<double> CDoubleArray;
bool CalculateCurveParameter(CDoubleArray *X,CDoubleArray *Y,long M,long N,CDoubleArray *A)
{
    //X,Y --  X,Y���������
    //M   --  �����������
    //N   --  ������Ŀ
    //A   --  �������

    long i,j,k;
    double Z,D1,D2,C,P,G,Q;
    CDoubleArray B,T,S;
    B.resize(N);
    T.resize(N);
    S.resize(N);
    if(M>N)M=N;
    for(i=0;i<M;i++)
        (*A)[i]=0;
    Z=0;
    B[0]=1;
    D1=N;
    P=0;
    C=0;
    for(i=0;i<N;i++)
    {
        P=P+(*X)[i]-Z;
        C=C+(*Y)[i];
    }
    C=C/D1;
    P=P/D1;
    (*A)[0]=C*B[0];
    if(M>1)
    {
        T[1]=1;
        T[0]=-P;
        D2=0;
        C=0;
        G=0;
        for(i=0;i<N;i++)
        {
            Q=(*X)[i]-Z-P;
            D2=D2+Q*Q;
            C=(*Y)[i]*Q+C;
            G=((*X)[i]-Z)*Q*Q+G;
        }
        C=C/D2;
        P=G/D2;
        Q=D2/D1;
        D1=D2;
        (*A)[1]=C*T[1];
        (*A)[0]=C*T[0]+(*A)[0];
    }
    for(j=2;j<M;j++)
    {
        S[j]=T[j-1];
        S[j-1]=-P*T[j-1]+T[j-2];
        if(j>=3)
        {
            for(k=j-2;k>=1;k--)
                S[k]=-P*T[k]+T[k-1]-Q*B[k];
        }
        S[0]=-P*T[0]-Q*B[0];
        D2=0;
        C=0;
        G=0;
        for(i=0;i<N;i++)
        {
            Q=S[j];
            for(k=j-1;k>=0;k--)
                Q=Q*((*X)[i]-Z)+S[k];
            D2=D2+Q*Q;
            C=(*Y)[i]*Q+C;
            G=((*X)[i]-Z)*Q*Q+G;
        }
        C=C/D2;
        P=G/D2;
        Q=D2/D1;
        D1=D2;
        (*A)[j]=C*S[j];
        T[j]=S[j];
        for(k=j-1;k>=0;k--)
        {
            (*A)[k]=C*S[k]+(*A)[k];
            B[k]=T[k];
            T[k]=S[k];
        }
    }
    return true;
}

bool LeastSquare(const std::vector<double>& x_value, const std::vector<double>& y_value,
                 int M, std::vector<double>& a_value)
{
    assert(x_value.size() == y_value.size());
    assert(a_value.size() == M);

    double *matrix = new double[M * M];
    double *b= new double[M];

    std::vector<double> x_m(x_value.size(), 1.0);
    std::vector<double> y_i(y_value.size(), 0.0);
    for(int i = 0; i < M; i++)
    {
        matrix[ARR_INDEX(0, i, M)] = std::accumulate(x_m.begin(), x_m.end(), 0.0);
        for(int j = 0; j < static_cast<int>(y_value.size()); j++)
        {
            y_i[j] = x_m[j] * y_value[j];
        }
        b[i] = std::accumulate(y_i.begin(), y_i.end(), 0.0);
        for(int k = 0; k < static_cast<int>(x_m.size()); k++)
        {
            x_m[k] *= x_value[k];
        }
    }
    for(int row = 1; row < M; row++)
    {
        for(int i = 0; i < M - 1; i++)
        {
            matrix[ARR_INDEX(row, i, M)] = matrix[ARR_INDEX(row - 1, i + 1, M)];
        }
        matrix[ARR_INDEX(row, M - 1, M)] = std::accumulate(x_m.begin(), x_m.end(), 0.0);
        for(int k = 0; k < static_cast<int>(x_m.size()); k++)
        {
            x_m[k] *= x_value[k];
        }
    }

    GuassEquation equation(M, matrix, b);
    delete[] matrix;
    delete[] b;

    return equation.Resolve(a_value);
}
const int N = 12;
double x_value[N] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
double y_value[N] = { 8.41, 9.94, 11.58, 13.02, 14.33, 15.92, 17.54, 19.22, 20.49, 22.01, 23.53, 24.47 };
/*
const int N = 6;
double x_value[N] = { 0.5, 1.0, 1.5, 2.0, 2.5, 3.0 };
double y_value[N] = { 1.75, 2.45, 3.81, 4.80, 7.00, 8.60 };
*/

int main(int argc, char* argv[])
{
    std::vector<double> X,Y;
    std::vector<double> A(2);

    X.assign(x_value, x_value + N);
    Y.assign(y_value, y_value + N);

    CalculateCurveParameter(&X, &Y, 2, N, &A);

    CDoubleArray B(2);
    LeastSquare(X, Y, 2, B);

	return 0;
}
