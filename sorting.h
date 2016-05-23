/* Author : Pham Tran Tuan - 20112449 - Bach Khoa Ha Noi (Ha Noi University of Sci and Tech)
 * Code Ref : Source Code coder co tham khao (Code References)
 *          + Test_general.cpp - link ref : https://github.com/shaih/HElib/blob/master/src/Test_General.cpp/ 
 *          + http://www.iti.fh-flensburg.de/lang/algorithmen/sortieren/bitonic/bitonicen.htm */

#ifndef SORT_H 
#define SORT_H 

#include "common_function.h"
void test_bitonic_sort(long p,long r,long d, long c,long k,long w,long L,long num);
long maxPow2LT(int n);
void compareAndSwap(vector<Ctxt>& ctxtIns,EncryptedArray& ea,Ctxt& cZeroOne,Ctxt& cOneZero,Ctxt& cOne,long nslots,long i,long j,Ctxt dir);
void bitonicSorting(vector<Ctxt>& ctxtIns,long nslots,EncryptedArray& ea,Ctxt & cZeroOne,Ctxt& cOneZero,Ctxt& cOne,long lo,long n,Ctxt dir);
void bitonicMerge(vector<Ctxt>& ctxtIns,long nslots,EncryptedArray &ea,Ctxt& cZeroOne,Ctxt& cOneZero,Ctxt& cOne,long lo,long n,Ctxt dir);
void test_direct_sorter(long p,long r,long d, long c,long k,long w,long L,long num);
vector<Ctxt> directSorting(vector<Ctxt>& ctxtIns,long nslots,EncryptedArray& ea);


void test_bitonic_sort(long p,long r,long d, long c,long k,long w,long L,long num){
    /*---------------CONFIGURATION-------------------*/
    
    ZZX GZZX;

    long pr = 2,iter;
    long s = 32;
    //long m = FindM(k,L,c,p,d,s,0);
    long m = 771;
    FHEcontext context(m,p,r);
    long n = pow(p,r);
    
    AddPrimesBySize(context,5,false);
    buildModChain(context, L,c);
    FHESecKey secretKey(context);
    const FHEPubKey& publicKey = secretKey;

    secretKey.GenSecKey(w);

    //cout << "Okay" << endl;
    addSome1DMatrices(secretKey);
    
    //cout << "Okay" << endl;
    GZZX = context.alMod.getFactorsOverZZ()[0];
    EncryptedArray ea(context,GZZX); 


    long nslots = ea.size(); 
    //PAlgebra al(m,p);
    //cout << al.getNSlots() << endl;
    cout << "Done Configuration" << endl;

    /* -------------------- DECLARE SOME USEFUL CIPHERTEX -------------------*/

    //vector<vector<long> > onearray[nslots - 1];
    vector<long> zeroOne (nslots,0);
    Ctxt cZeroOne (publicKey);
    zeroOne[0] = 1;
    ea.encrypt(cZeroOne,publicKey,zeroOne);
    
    zeroOne[nslots - 1] = 1;
    zeroOne[0] = 0;
    Ctxt cOneZero (publicKey);
    ea.encrypt(cOneZero,publicKey,zeroOne);

    vector<long> vOne (nslots,1);
    Ctxt cOne(publicKey);
    ea.encrypt(cOne,publicKey,vOne);

    Ctxt cTrue(publicKey);
    cTrue = cOneZero;

    vector<long> vZero (nslots,0);
    Ctxt cFalse(publicKey);
    ea.encrypt(cFalse,publicKey,vZero);


    /*--------------------- TEST SORTING ------------------------*/
    
    cout << "begin test Sorting" << endl;
    vector<vector<long> > vVal;
    vVal.push_back(convertToCoeArray(67,nslots));
    vVal.push_back(convertToCoeArray(22,nslots));
    vVal.push_back(convertToCoeArray(76,nslots));
    vVal.push_back(convertToCoeArray(22,nslots));
    vVal.push_back(convertToCoeArray(33,nslots));

    vector<Ctxt> vCtxt;

    for(iter = 0 ; iter < vVal.size() ; iter++){
        Ctxt nC(publicKey);
        ea.encrypt(nC,publicKey,vVal[iter]);
        vCtxt.push_back(nC);
    }
    bitonicSorting(vCtxt,nslots,ea,cZeroOne,cOneZero,cOne,0,vCtxt.size(),cTrue);
    
    long maxlevel = 0;
    for(iter = 0 ;iter < vVal.size() ; iter++){
        CheckCtxt(vCtxt[iter],"");
        if(vCtxt[iter].findBaseLevel() > maxlevel){
            maxlevel = vCtxt[iter].findBaseLevel();
        }
    }

    for(iter = 0 ; iter < vCtxt.size(); iter++){
        ea.decrypt(vCtxt[iter],secretKey,vVal[iter]);
        cout << convertToL(vVal[iter],nslots) << " ";
    }
    cout << endl; 
    cout << "MAX level " << maxlevel << endl;

    cout << endl;
}



/************************ IMPLEMENT BITONIC SORTING****************************
 * ****************************************************************************/

long maxPow2LT(long n){
    long result = 1;
    while(result < n){
        result = result << 1;
    }
    result = result >> 1;
    return result;
}

void compareAndSwap(vector<Ctxt>& ctxtIns,EncryptedArray& ea,Ctxt& cZeroOne,Ctxt& cOneZero,Ctxt& cOne,long nslots,long i,long j,Ctxt dir){
    cout << "swap " << endl;
    Ctxt cCompare2Val = compare2(ctxtIns[i],ctxtIns[j], ea,cZeroOne,cOneZero,cOne,nslots);
    cCompare2Val.addCtxt(cOne);
    Ctxt cIsEqual2Com = isEqual3(cCompare2Val,dir,cOne,ea,nslots);
    Ctxt cnIsEaqual2Com = cIsEqual2Com;
    NOT(cnIsEaqual2Com,cOne);
    Ctxt cTermi(ctxtIns[i].getPubKey());
    cTermi = ctxtIns[i];
    Ctxt cTermj(ctxtIns[j].getPubKey());
    cTermj = ctxtIns[j];
    
    ctxtIns[i].multiplyBy(cnIsEaqual2Com);
    cTermj.multiplyBy(cIsEqual2Com);
    ctxtIns[i].addCtxt(cTermj);

    ctxtIns[j].multiplyBy(cnIsEaqual2Com);
    cTermi.multiplyBy(cIsEqual2Com);
    ctxtIns[j].addCtxt(cTermi);

}

void bitonicSorting(vector<Ctxt>& ctxtIns,long nslots,EncryptedArray& ea,Ctxt & cZeroOne,Ctxt& cOneZero,Ctxt& cOne,long lo,long n,Ctxt dir){
    
    Ctxt notDir(dir.getPubKey());
    notDir = dir;
    NOT(notDir,cOne);
    if(n > 1){
        long m = n/2;
        bitonicSorting(ctxtIns,nslots,ea,cZeroOne,cOneZero,cOne,lo,m,notDir);
        bitonicSorting(ctxtIns,nslots,ea,cZeroOne,cOneZero,cOne,lo + m,n-m,dir);
        bitonicMerge(ctxtIns,nslots,ea,cZeroOne,cOneZero,cOne,lo,n,dir); 
    }
}

void bitonicMerge(vector<Ctxt>& ctxtIns,long nslots,EncryptedArray &ea,Ctxt& cZeroOne,Ctxt& cOneZero,Ctxt& cOne,long lo,long n,Ctxt dir){
    if(n > 1){
        long iter;
        long m = maxPow2LT(n);
        long boundIter = lo + n - m;
        for(iter = lo ; iter < boundIter ; iter++){
            compareAndSwap(ctxtIns,ea,cZeroOne,cOneZero,cOne,nslots,iter,iter+m,dir);
        }
        bitonicMerge(ctxtIns,nslots,ea,cZeroOne,cOneZero,cOne,lo,m,dir);
        bitonicMerge(ctxtIns,nslots,ea,cZeroOne,cOneZero,cOne,lo+m,n-m,dir);
    }
}


/* ********************************* IMPLEMENTING DIRECT SORT **********************************************/


void test_direct_sorter(long p,long r,long d, long c,long k,long w,long L,long num){
    /*---------------CONFIGURATION-------------------*/
    
    ZZX GZZX;

    long pr = 2,iter;
    long s = 32;
    //long m = FindM(k,L,c,p,d,s,0);
    long m = 771;
    FHEcontext context(m,p,r);
    long n = pow(p,r);
    
    AddPrimesBySize(context,5,false);
    buildModChain(context, L,c);
    FHESecKey secretKey(context);
    const FHEPubKey& publicKey = secretKey;

    secretKey.GenSecKey(w);

    //cout << "Okay" << endl;
    addSome1DMatrices(secretKey);
    
    //cout << "Okay" << endl;
    GZZX = context.alMod.getFactorsOverZZ()[0];
    EncryptedArray ea(context,GZZX); 


    long nslots = ea.size(); 
    //PAlgebra al(m,p);
    //cout << al.getNSlots() << endl;
    cout << "Done Configuration" << endl;
    cout << "--- Maximum Circuit Depth  " << L << endl;
    printf("--- True Plaintext Space : Z[X]/(cyclotomic(%ld),%ld^%ld \n",m,p,r);
    printf("--- Slot Value Space (Input Plaintext Space) : Z[X]/(G,%ld^%ld) voi deg(G) la d = %ld \n",p,r,d);
    printf("--- Number of Bit input la so luong slot %ld \n",nslots);

    /* -------------------- DECLARE SOME USEFUL CIPHERTEX -------------------*/

    //vector<vector<long> > onearray[nslots - 1];
    vector<long> zeroOne (nslots,0);
    Ctxt cZeroOne (publicKey);
    zeroOne[0] = 1;
    ea.encrypt(cZeroOne,publicKey,zeroOne);
    
    zeroOne[nslots - 1] = 1;
    zeroOne[0] = 0;
    Ctxt cOneZero (publicKey);
    ea.encrypt(cOneZero,publicKey,zeroOne);

    vector<long> vOne (nslots,1);
    Ctxt cOne(publicKey);
    ea.encrypt(cOne,publicKey,vOne);


    /*--------------------- TEST SORTING ------------------------*/
    
    cout << "begin test Sorting" << endl;
    vector<vector<long> > vVal;
    vVal.push_back(convertToCoeArray(67,nslots));
    vVal.push_back(convertToCoeArray(22,nslots));
    vVal.push_back(convertToCoeArray(22,nslots));
    vVal.push_back(convertToCoeArray(55,nslots));

    vector<Ctxt> vCtxt;

    for(iter = 0 ; iter < vVal.size() ; iter++){
        Ctxt nC(publicKey);
        ea.encrypt(nC,publicKey,vVal[iter]);
        vCtxt.push_back(nC);
    }
    vector<Ctxt> indexSorted = directSorting(vCtxt,nslots,ea);

    long lowestlevel = indexSorted[0].findBaseLevel();
    
    for(iter = 0 ;iter < indexSorted.size() ; iter++){
        CheckCtxt(indexSorted[iter],"");
        if(indexSorted[iter].findBaseLevel() < lowestlevel){
            lowestlevel = indexSorted[iter].findBaseLevel();
        }

    }

    for(iter = 0 ; iter < indexSorted.size(); iter++){
        ea.decrypt(indexSorted[iter],secretKey,vVal[iter]);
        //cout << "OK" << endl; 
        cout << convertToL(vVal[iter],nslots) << " ";
    }
    cout << endl;
    cout << "LOWEST level " << lowestlevel << endl;

    cout << endl;
}


vector<Ctxt> directSorting(vector<Ctxt>& ctxtIns,long nslots,EncryptedArray& ea){
    cout << "Begin Sorting" << endl;
    long numCtxt = ctxtIns.size(),iter,iter1;
    //Ctxt M[numCtxt][numCtxt];
    //vector<vector<Ctxt> > M(numCtxt);
    Ctxt nC(ctxtIns[0].getPubKey());
    vector<vector<Ctxt> > M(numCtxt);
    vector<vector<Ctxt> > comMatrix(numCtxt);
    
    for(iter = 0 ; iter < numCtxt ; iter++){
        for(iter1 = 0 ; iter1 < numCtxt ; iter1++){
            Ctxt nCC(ctxtIns[0].getPubKey());
            M[iter].push_back(nCC);
        }
    }
    
    
    

    /* -------------------- DECLARE SOME USEFUL CIPHERTEX -------------------*/

    //vector<vector<long> > onearray[nslots - 1];
    vector<long> zeroOne (nslots,0);
    Ctxt cZeroOne (ctxtIns[0].getPubKey());
    zeroOne[0] = 1;
    ea.encrypt(cZeroOne,ctxtIns[0].getPubKey(),zeroOne);
    
    vector<long> oneZero(nslots,0);
    oneZero[nslots - 1] = 1;
    Ctxt cOneZero (ctxtIns[0].getPubKey());
    ea.encrypt(cOneZero,ctxtIns[0].getPubKey(),oneZero);

    vector<long> vOne (nslots,1);
    Ctxt cOne(ctxtIns[0].getPubKey());
    ea.encrypt(cOne,ctxtIns[0].getPubKey(),vOne);

        
    /* ------- CALCULATE M ---------*/
    cout << "Begin Calculating Matrix M" << endl;
    for(iter = 0 ; iter < numCtxt - 1 ; iter++){
        for(iter1 = iter + 1 ; iter1 < numCtxt ; iter1++){
            Ctxt result1(ctxtIns[0].getPubKey());
            Ctxt result2(ctxtIns[0].getPubKey());
            result1 = compare2(ctxtIns[iter],ctxtIns[iter1],ea,cZeroOne,cOneZero,cOne,nslots);
            //cout << "OK" << endl;
            M[iter1][iter] = result1; 
            result2 = result1;
            result2.addCtxt(cOneZero);
            M[iter][iter1] = result2;
            
        } 
    }
    vector<long> zero(nslots,0);
    Ctxt cZero(ctxtIns[0].getPubKey());
    ea.encrypt(cZero,ctxtIns[0].getPubKey(),zero);
    for(iter = 0 ; iter < numCtxt ; iter++){
        M[iter][iter] = cZero;
    }

    cout << "Done Calculating Matrix" << endl;

    cout << "Begin Calculating Index Result" << endl;
    
    vector<Ctxt> indexArray;
    for(iter = 0; iter < numCtxt  ; iter++){
        Ctxt cIndex = addMemoVector(M[iter],ea,nslots);
        //cIndex.addCtxt(cOneZero); 
        indexArray.push_back(cIndex);
        cout << "[Index Result] Done Column" << iter << endl;
    }
    //return indexArray;
    cout << "Done Calculating Index Result" << endl;

    cout << "Begin Calculating (index compare)*val Matrix" << endl;
    vector<Ctxt> sortedArray;
    vector<Ctxt> isEqualCtxt;
    vector<vector<Ctxt> > isEqualResult(numCtxt);
    
    for(iter = 0 ; iter < numCtxt ; iter++){
        //cout << iter << endl;
        vector<long> nVindex = convertToCoeArray(iter,nslots);
        Ctxt nCindex(ctxtIns[0].getPubKey());
        ea.encrypt(nCindex,ctxtIns[0].getPubKey(),nVindex);
        isEqualCtxt.push_back(nCindex);
                
        for(iter1 = 0 ; iter1 < numCtxt ; iter1++){
            cout << iter1 << endl;
            Ctxt cComRe = isEqual2(indexArray[iter1],isEqualCtxt[iter],cOne,ea,nslots);
            cComRe.multiplyBy(ctxtIns[iter1]);
            isEqualResult[iter].push_back(cComRe);
        }
    }

    cout << "Done Calculating (index compare)*val Matrix" << endl;

    cout << "Begin Calculating sorted Array" << endl;
    for(iter = 0 ; iter < numCtxt ; iter++){
        Ctxt rIsequal = addOrdiVector(isEqualResult[iter],ea,nslots);
        sortedArray.push_back(rIsequal);
    }


    cout << "Done Calculating sorted Array" << endl;
    return sortedArray;
    
}

/************************ IMPLEMENT directSorting ****************************/

#endif
