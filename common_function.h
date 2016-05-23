/* Author : Pham Tran Tuan - 20112449 - Bach Khoa Ha Noi (Ha Noi University of Science and Tech)
 * Code Ref : Source Code tac gia co tham khao (code references) : 
 *          + Test_general.cpp - link ref : https://github.com/shaih/HElib/blob/master/src/Test_General.cpp/ */

#ifndef COMMON_FUNC_H
#define COMMON_FUNC_H

#include <NTL/ZZ.h>
#include "./HElib/src/FHE.h"
#include <time.h>
#include "./HElib/src/EncryptedArray.h"
#include "./HElib/src/PAlgebra.h"
#include <NTL/lzz_pXFactoring.h>
#include <cstring> 
#include <cassert>
#include <cstdio>
#include <NTL/vec_ZZ.h>
#include <NTL/ZZ_pX.h>
#include "./HElib/src/powerful.h"
#include <math.h>
#include <algorithm>
#include <vector>

inline Ctxt addMemo(Ctxt& B,Ctxt& A,EncryptedArray& ea,long nslots);
inline void OR(Ctxt& B,Ctxt& A,Ctxt cOne);
inline Ctxt addOrdiVector(vector<Ctxt>& vArray,EncryptedArray& ea,Ctxt cOne,long nslots);
inline Ctxt mulOrdiVector(vector<Ctxt>& vArray,EncryptedArray& ea,long nslots);
inline Ctxt addMemoVector(vector<Ctxt>& vArray,EncryptedArray& ea,long nslots);
inline Ctxt isEqual(Ctxt B,Ctxt A,Ctxt cOne,EncryptedArray ea,long nslots);
inline Ctxt isEqual2(Ctxt B,Ctxt A,Ctxt cOne,EncryptedArray ea,long nslots);
inline Ctxt isEqual3(Ctxt &B,Ctxt& A,Ctxt& cOne,EncryptedArray& ea,long nslots);
inline Ctxt compare2(Ctxt& B,Ctxt& A,EncryptedArray &ea,Ctxt &cZeroOne,Ctxt &cOneZero,Ctxt &cOne,long nslots);
inline void NOT(Ctxt& B,Ctxt &cOne);
inline Ctxt compare(Ctxt &B,Ctxt &A,EncryptedArray &ea,Ctxt &cZeroOne,Ctxt &cOneZero,Ctxt &cOne,long nslots);
vector<long> convertToCoeArray(long val,long nslots);
void printV(vector<long> &vLong,long nslots);
long convertToL(vector<long> &vLong,long d);
long findL(int choice,long numVal,long elementBitLength);


inline Ctxt addMemo(Ctxt& B,Ctxt& A,EncryptedArray& ea,long nslots){
    Ctxt cTerm(A.getPubKey());
    //cTerm = A;
    long iter;
    for(iter = 0 ; iter < nslots ; iter++){
        cTerm = A;
        B.addCtxt(A);
        cTerm.multiplyBy(B);
        A.addCtxt(cTerm);
        ea.shift(A,-1); 
    }
    return B;
}
inline void OR(Ctxt& B,Ctxt& A,Ctxt cOne){
    Ctxt cTerm(A.getPubKey());
    cTerm = A;
    cTerm.multiplyBy(B);
    B.addCtxt(A);
    cTerm.multiplyBy(cOne);
    B.addCtxt(cTerm);
}

inline Ctxt addOrdiVector(vector<Ctxt>& vArray,EncryptedArray& ea,long nslots){
    Ctxt result(vArray[0].getPubKey());
    if(vArray.size() == 1){
        return vArray[0];
    }else{
        long iter;
        long size = vArray.size();
        long mid = size/2;
        vector<Ctxt> leftArray;
        vector<Ctxt> rightArray;
        for(iter = 0 ; iter < mid ; iter++){
            leftArray.push_back(vArray[iter]);
        }

        for(iter = mid ; iter < size ; iter++){
            rightArray.push_back(vArray[iter]);
        }
        Ctxt resultLeft = addOrdiVector(leftArray,ea,nslots);
        Ctxt resultRight = addOrdiVector(rightArray,ea,nslots);
         result = resultRight;
        result.addCtxt(resultLeft);
        return result;
    }
}
inline Ctxt mulOrdiVector(vector<Ctxt>& vArray,EncryptedArray& ea,long nslots){
    Ctxt result(vArray[0].getPubKey());
    if(vArray.size() == 1){
        return vArray[0];
    }else{
        long iter;
        long size = vArray.size();
        long mid = size/2;
        vector<Ctxt> leftArray;
        vector<Ctxt> rightArray;
        for(iter = 0 ; iter < mid ; iter++){
            leftArray.push_back(vArray[iter]);
        }

        for(iter = mid ; iter < size ; iter++){
            rightArray.push_back(vArray[iter]);
        }
        Ctxt resultLeft = mulOrdiVector(leftArray,ea,nslots);
        Ctxt resultRight = mulOrdiVector(rightArray,ea,nslots);
        result = resultLeft;
        result.multiplyBy(resultRight);
        return result;
    }
}

inline Ctxt orVector(vector<Ctxt>& vArray,EncryptedArray& ea,Ctxt& cOne,long nslots){
    Ctxt result(vArray[0].getPubKey());
    if(vArray.size() == 1){
        return vArray[0];
    }else{
        long iter;
        long size = vArray.size();
        long mid = size/2;
        vector<Ctxt> leftArray;
        vector<Ctxt> rightArray;
        for(iter = 0 ; iter < mid ; iter++){
            leftArray.push_back(vArray[iter]);
        }

        for(iter = mid ; iter < size ; iter++){
            rightArray.push_back(vArray[iter]);
        }

        Ctxt resultLeft = orVector(leftArray,ea,cOne,nslots);
        Ctxt resultRight = orVector(rightArray,ea,cOne,nslots);
        result = resultLeft;
        OR(result,resultRight,cOne);
        return result;
         
    }

}
inline Ctxt addMemoVector(vector<Ctxt>& vArray,EncryptedArray& ea,long nslots){
    
    if(vArray.size() == 2){
        return addMemo(vArray[0],vArray[1],ea,nslots);
    }else if(vArray.size() == 1){
        return vArray[0];
    }else{
        long iter;
        long size = vArray.size();
        long mid = size/2;
        vector<Ctxt> leftArray;
        vector<Ctxt> rightArray;
        for(iter = 0 ; iter < mid ; iter++){
            leftArray.push_back(vArray[iter]);
        }

        for(iter = mid ; iter < size ; iter++){
            rightArray.push_back(vArray[iter]);
        }
        Ctxt resultLeft = addMemoVector(leftArray,ea,nslots);
        Ctxt resultRight = addMemoVector(rightArray,ea,nslots);
        return addMemo(resultLeft,resultRight,ea,nslots);
    }
}
inline Ctxt isEqual(Ctxt B,Ctxt A,Ctxt cOne,EncryptedArray ea,long nslots){
    Ctxt result(A.getPubKey());
    result = A;
    result.addCtxt(cOne);
    A.multiplyBy(B);
    B.addCtxt(cOne);
    result.multiplyBy(B);
    result.addCtxt(A);
    vector<Ctxt> shiftArray;
    shiftArray.push_back(result);
    long iter;
    for(iter = 0 ; iter < nslots - 1 ; iter++){
        Ctxt nC(result.getPubKey());
        ea.shift(result,1); 
        nC = result;
        shiftArray.push_back(nC);
    }
    Ctxt resultReturn = mulOrdiVector(shiftArray,ea,nslots);
    return resultReturn;
} 
inline Ctxt isEqual2(Ctxt B,Ctxt A,Ctxt cOne,EncryptedArray ea,long nslots){
    Ctxt result(A.getPubKey());
    result = A;
    result.addCtxt(cOne);
    A.multiplyBy(B);
    B.addCtxt(cOne);
    result.multiplyBy(B);
    result.addCtxt(A);
    vector<Ctxt> shiftArray;
    shiftArray.push_back(result);
    long iter;
    for(iter = 0 ; iter < nslots - 1 ; iter++){
        Ctxt nC(result.getPubKey());
        ea.rotate(result,1); 
        nC = result;
        shiftArray.push_back(nC);
    }
    Ctxt resultReturn = mulOrdiVector(shiftArray,ea,nslots);
    return resultReturn;
} 


inline void NOT(Ctxt& B,Ctxt &cOne){
    B.addCtxt(cOne);
}

inline Ctxt compare(Ctxt &B,Ctxt &A,EncryptedArray &ea,Ctxt &cZeroOne,Ctxt &cOneZero,Ctxt &cOne,long nslots){
    Ctxt cAB(A.getPubKey());
    cAB = A;
    cAB.multiplyBy(B);
    Ctxt cX(A.getPubKey());
    cX = A;
    NOT(cX,cOne);
    Ctxt nB(B.getPubKey());
    nB = B;
    NOT(nB,cOne);
    
    cX.multiplyBy(nB);
    //KHong can sua xor thanh or
    cX.addCtxt(cAB);

    Ctxt anB(A.getPubKey());
    anB = A;
    anB.multiplyBy(nB);
    //return anB;
    //result = cOneZero;
    int iter;
    //return cX;
    for(iter = 1 ; iter < nslots ; iter++){
        ea.shift(cX,1);
        // KHong can su thanh or
        cX.addCtxt(cZeroOne);
        anB.multiplyBy(cX);
    }
    Ctxt result(A.getPubKey());
    result = anB;
    for(iter = 0 ; iter < nslots - 1 ; iter++){
        ea.shift(anB,1);
        OR(result,anB,cOne);
    } 

    cout << "Done Compare" << endl;
    result.multiplyBy(cOneZero);
    return result;
}
inline Ctxt isEqual3(Ctxt &B,Ctxt& A,Ctxt& cOne,EncryptedArray& ea,long nslots){
    B.addCtxt(A);
    B.addCtxt(cOne);
    long iter;
    vector<Ctxt> shiftArray;
    shiftArray.push_back(B);
    for(iter = 0 ; iter < nslots - 1 ; iter++){
        Ctxt nC(B.getPubKey());
        nC = shiftArray[shiftArray.size() - 1];
        ea.shift(nC,-1);
        shiftArray.push_back(nC);
    }
    Ctxt result = addOrdiVector(shiftArray,ea,nslots);
    return result;
} 

inline Ctxt compare2(Ctxt& B,Ctxt& A,EncryptedArray &ea,Ctxt &cZeroOne,Ctxt &cOneZero,Ctxt &cOne,long nslots){
    Ctxt cAB(A.getPubKey());
    cAB = A;
    cAB.multiplyBy(B);
    Ctxt cX(A.getPubKey());
    cX = A;
    NOT(cX,cOne);
    Ctxt nB(B.getPubKey());
    nB = B;
    NOT(nB,cOne);
    
    cX.multiplyBy(nB);
    //KHong can sua xor thanh or
    cX.addCtxt(cAB);

    Ctxt anB(A.getPubKey());
    anB = A;
    anB.multiplyBy(nB);
    //return anB;
    //result = cOneZero;
    int iter;
    //return cX;
    vector<Ctxt> cXShiftladder;
    cXShiftladder.push_back(anB);
    ea.shift(cX,1);
    cX.addCtxt(cZeroOne);
    cXShiftladder.push_back(cX);
    for(iter = 1 ; iter < nslots - 1; iter++){
        Ctxt nC(cX.getPubKey());
        nC = cXShiftladder[cXShiftladder.size() - 1];
        ea.shift(nC,1);
        nC.addCtxt(cZeroOne);
        cXShiftladder.push_back(nC);
    }
    
    Ctxt result(A.getPubKey());
    result = mulOrdiVector(cXShiftladder,ea,nslots);
    vector<Ctxt> orArray;
    orArray.push_back(result);
    
    for(iter = 0 ; iter < nslots - 1 ; iter++){
        Ctxt nC(orArray[0].getPubKey());
        nC = orArray[orArray.size() - 1];
        ea.shift(nC,1);
        orArray.push_back(nC);
    } 
    Ctxt returnResult(orArray[0].getPubKey());
    returnResult = orVector(orArray,ea,cOne,nslots);
    cout << "Done Compare" << endl;
    returnResult.multiplyBy(cOneZero);
    return returnResult;
}

vector<long> convertToCoeArray(long val,long nslots){
    
    vector<long> result(nslots,0);
    long quotient;
    int i = nslots - 1;
    if(val == 0){
        return result;
    }
    while(val > 0){
        quotient = val%2;
         result[i] = quotient;
        val = (val - quotient)/2;
        i--;
    }

    return result;
}

void printV(vector<long> &vLong,long nslots){
    int iter;
    cout << "[";
    for(iter = 0 ; iter < nslots ; iter++){
        cout << vLong[iter] << " ";
    }
    cout << "]" << endl;
}
long convertToL(vector<long> &vLong,long d){
    int iter;
    long result = 0;
    long x = 1;
    for(iter = d-1 ;iter >= 0 ; iter--){
        result = result + vLong[iter]*x;
        x = x*2;
    }
    return result;
}
/* Find L depends on the size of input array,calculated by Pham
 * Tran Tuan*/
long findL(int choice,long numVal,long elementBitLength){
    long result;
    if(choice == 0){
        long c = 4;
        result = 8 + 3*elementBitLength + (2*elementBitLength + 1)*log2(numVal)*c + log2(elementBitLength); 
    }else{
        long c = 1;
        result = c*numVal*(log2(numVal))*(log2(numVal))*3*log2(elementBitLength);
    } 
    return result;
}



#endif
