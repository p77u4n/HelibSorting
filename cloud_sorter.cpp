/*Author : Tuan Cau Rao*/

#include "sorting.h"
#include "common_function.h"

int main(){

    /*---------------READ SEND FILE ------------------*/
    
    fstream receiveFile("sendF_encrypter.txt",fstream::in);
    unsigned long m,p,r;
    vector<long> gens,orgs;
    cout << "Begin Reading Context :|" << endl;
    readContextBase(receiveFile,m,p,r,gens,orgs);
    FHEcontext context(m,p,r,gens,orgs);
    receiveFile >> context;
    
    cout << "Read Context Done :)" << endl;
    FHEPubKey publicKey(context);
    cout << "Begin Reading Pubkey :|" << endl;

    receiveFile >> publicKey;

    cout << "Read Public Key Done :)" << endl;
    

    EncryptedArray ea(context);
    long nslots = ea.size();
    
    long numVal,iter,iter1;
    receiveFile >> numVal;

    vector<Ctxt> vectorCipher;

    for(iter = 0 ; iter < numVal ; iter++){
        Ctxt cN(publicKey);
        receiveFile >> cN;
        vectorCipher.push_back(cN);
    }
    receiveFile.close();

    /*------------- SORTING PROCESS -----------------*/

    cout << " Begin Sorting" << endl;
    cout << "   0 - Direct-Sort : slow time - but using sorter modulus chain" << endl;
    cout << "   1 - Bitonic-Sort : faster tiem - but using too long modulus chain" << endl;

    int choice;
    vector<Ctxt> resultCipher;
    cin >> choice;
    clock_t tStart = clock();
    if(choice == 0){
        resultCipher =  directSorting(vectorCipher,nslots,ea); 
    }else{
        /* -------------------- DECLARE SOME USEFUL CIPHERTEX -------------------*/

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

        bitonicSorting(vectorCipher,nslots,ea,cZeroOne,cOneZero,cOne,0,vectorCipher.size(),cTrue);
        resultCipher = vectorCipher;
    }

    long lowestdepth = resultCipher[0].findBaseLevel();


    /*----------------- OUPUT result.txt ---------------------------------*/
    fstream sendFile("result.txt",fstream::out|fstream::trunc);
    sendFile << numVal << endl;
    for(iter = 0 ; iter < numVal ; iter++){
        sendFile << resultCipher[iter] << endl; 
        if(resultCipher[iter].findBaseLevel() < lowestdepth){
            lowestdepth = resultCipher[iter].findBaseLevel();
        }
    }
    sendFile.close();
    printf("Time taken : %.2fs\n",(double)(clock() - tStart)/CLOCKS_PER_SEC);
    
}

long FindModulusChainLength(long numVal){
    long length;

    return length;
}
