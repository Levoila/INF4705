#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>

#include "helper.h"

using namespace std;

//Strassen algorithm from https://martin-thoma.com/strassen-algorithm-in-python-java-cpp/
int leafsize = 1;

void sum(vector< vector<int> > &A, 
         vector< vector<int> > &B, 
         vector< vector<int> > &C, int tam) {
    int i, j;
 
    for (i = 0; i < tam; i++) {
        for (j = 0; j < tam; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void subtract(vector< vector<int> > &A, 
              vector< vector<int> > &B, 
              vector< vector<int> > &C, int tam) {
    int i, j;
 
    for (i = 0; i < tam; i++) {
        for (j = 0; j < tam; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }   
}

void ikjalgorithm(vector< vector<int> > A, 
                                   vector< vector<int> > B,
                                   vector< vector<int> > &C, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void strassenR(vector< vector<int> > &A, 
              vector< vector<int> > &B, 
              vector< vector<int> > &C, int tam) {
    if (tam <= leafsize) {
        ikjalgorithm(A, B, C, tam);
        return;
    }
 
    // other cases are treated here:
    else {
        int newTam = tam/2;
        vector<int> inner (newTam);
        vector< vector<int> > 
            a11(newTam,inner), a12(newTam,inner), a21(newTam,inner), a22(newTam,inner),
            b11(newTam,inner), b12(newTam,inner), b21(newTam,inner), b22(newTam,inner),
              c11(newTam,inner), c12(newTam,inner), c21(newTam,inner), c22(newTam,inner),
            p1(newTam,inner), p2(newTam,inner), p3(newTam,inner), p4(newTam,inner), 
            p5(newTam,inner), p6(newTam,inner), p7(newTam,inner),
            aResult(newTam,inner), bResult(newTam,inner);
 
        int i, j;
 
        //dividing the matrices in 4 sub-matrices:
        for (i = 0; i < newTam; i++) {
            for (j = 0; j < newTam; j++) {
                a11[i][j] = A[i][j];
                a12[i][j] = A[i][j + newTam];
                a21[i][j] = A[i + newTam][j];
                a22[i][j] = A[i + newTam][j + newTam];
 
                b11[i][j] = B[i][j];
                b12[i][j] = B[i][j + newTam];
                b21[i][j] = B[i + newTam][j];
                b22[i][j] = B[i + newTam][j + newTam];
            }
        }
 
        // Calculating p1 to p7:
 
        sum(a11, a22, aResult, newTam); // a11 + a22
        sum(b11, b22, bResult, newTam); // b11 + b22
        strassenR(aResult, bResult, p1, newTam); // p1 = (a11+a22) * (b11+b22)
 
        sum(a21, a22, aResult, newTam); // a21 + a22
        strassenR(aResult, b11, p2, newTam); // p2 = (a21+a22) * (b11)
 
        subtract(b12, b22, bResult, newTam); // b12 - b22
        strassenR(a11, bResult, p3, newTam); // p3 = (a11) * (b12 - b22)
 
        subtract(b21, b11, bResult, newTam); // b21 - b11
        strassenR(a22, bResult, p4, newTam); // p4 = (a22) * (b21 - b11)
 
        sum(a11, a12, aResult, newTam); // a11 + a12
        strassenR(aResult, b22, p5, newTam); // p5 = (a11+a12) * (b22)   
 
        subtract(a21, a11, aResult, newTam); // a21 - a11
        sum(b11, b12, bResult, newTam); // b11 + b12
        strassenR(aResult, bResult, p6, newTam); // p6 = (a21-a11) * (b11+b12)
 
        subtract(a12, a22, aResult, newTam); // a12 - a22
        sum(b21, b22, bResult, newTam); // b21 + b22
        strassenR(aResult, bResult, p7, newTam); // p7 = (a12-a22) * (b21+b22)
 
        // calculating c21, c21, c11 e c22:
 
        sum(p3, p5, c12, newTam); // c12 = p3 + p5
        sum(p2, p4, c21, newTam); // c21 = p2 + p4
 
        sum(p1, p4, aResult, newTam); // p1 + p4
        sum(aResult, p7, bResult, newTam); // p1 + p4 + p7
        subtract(bResult, p5, c11, newTam); // c11 = p1 + p4 - p5 + p7
 
        sum(p1, p3, aResult, newTam); // p1 + p3
        sum(aResult, p6, bResult, newTam); // p1 + p3 + p6
        subtract(bResult, p2, c22, newTam); // c22 = p1 + p3 - p2 + p6
 
        // Grouping the results obtained in a single matrix:
        for (i = 0; i < newTam ; i++) {
            for (j = 0 ; j < newTam ; j++) {
                C[i][j] = c11[i][j];
                C[i][j + newTam] = c12[i][j];
                C[i + newTam][j] = c21[i][j];
                C[i + newTam][j + newTam] = c22[i][j];
            }
        }
    }
}

unsigned int nextPowerOfTwo(int n) {
    return pow(2, int(ceil(log2(n))));
}

void strassen(vector< vector<int> > &A, 
              vector< vector<int> > &B, 
              vector< vector<int> > &C, unsigned int n) {
    //unsigned int n = tam;
    unsigned int m = nextPowerOfTwo(n);
    vector<int> inner(m);
    vector< vector<int> > APrep(m, inner), BPrep(m, inner), CPrep(m, inner);

    for(unsigned int i=0; i<n; i++) {
        for (unsigned int j=0; j<n; j++) {
            APrep[i][j] = A[i][j];
            BPrep[i][j] = B[i][j];
        }
    }

    strassenR(APrep, BPrep, CPrep, m);
    for(unsigned int i=0; i<n; i++) {
        for (unsigned int j=0; j<n; j++) {
            C[i][j] = CPrep[i][j];
        }
    }
}

int main(int argc, char** argv)
{
	Inputs inputs = readArgs(argc, argv);
	
	if (inputs.mat1.size() == 0 || inputs.mat2.size() == 0) {
		cout << "No matrices." << endl;
		return 0;
	}
	
	if (inputs.threshold != 0) {
		leafsize = inputs.threshold;
	}
	
	unsigned size = inputs.mat1.size();
	auto res = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));
	
	auto start = chrono::high_resolution_clock::now();
	strassen(inputs.mat1, inputs.mat2, res, size);
	auto end = chrono::high_resolution_clock::now();
	
	chrono::duration<double> elapsed = end - start;
	cout << fixed << setprecision(8) << elapsed.count() << endl;
	
	if (inputs.print) {
		printMatrix(res);
	}
	
	return 0;
}
