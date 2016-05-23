/*Author : Tuan Cau Rao*/

#include "./common_function.h"


int main(){

    long iter,iter1;

    fstream secretFile("secF_encrypter.txt",fstream::in);
    unsigned long m,p,r;
    vector<long> gens,orgs;
    readContextBase(secretFile,m,p,r,gens,orgs);
    FHEcontext context(m,p,r,gens,orgs);
    secretFile >> context;
    FHESecKey secretKey(context);
    const FHEPubKey& publicKey = secretKey;

    secretFile >> secretKey;

    EncryptedArray ea(context);
    long nslots = ea.size();

    /*---------- DOC FILE result ------------------------*/

    fstream resultFile("result.txt",fstream::in);
    
    vector<Ctxt> vecCipher;
    std::string line;
    long nCipher;
    resultFile >> nCipher;
    cout << "Number of Ciphertext " << nCipher << endl;
    for(iter = 0 ; iter < nCipher ; iter++){
        cout << "read cipher " << iter << endl;
        Ctxt nC(publicKey);
        resultFile >> nC;
        vecCipher.push_back(nC);
    }
    /*
    while (std::getline(resultFile, line))
    {
        cout << "read cipher : " << nCipher << endl;
        std::istringstream iss(line);
        Ctxt a(publicKey);
        if (!(iss >> a)) { break; } // error
        nCipher++;
        vecCipher.push_back(a);

        // process pair (a,b)
    }*/



    resultFile.close();
    
    /*------------------DECRYPTING PROCESS----------------*/
    fstream decryptFile("decryptF.txt",fstream::out|fstream::trunc);
    
    for(iter = 0; iter < nCipher ; iter++){
        vector<long> vVal;
        ea.decrypt(vecCipher[iter],secretKey,vVal);
        decryptFile << convertToL(vVal,nslots) << endl;
    }
    decryptFile.close(); 
}


