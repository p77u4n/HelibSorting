/*Author : Tuan Cau Rao*/
#include "./common_function.h"
#include <fstream>
#include <unistd.h>

int main(){
    cout << "Encrypted is beginning" << endl;
    long p = 2;
    long r = 1;
    long d = 1;
    long c = 2;
    long k = 128;
    long w = 64;
    long L = 1000;
    long num = 10;
    long s = 32;
    long m = 771; // voi m  = 771 ta duoc so 32 bit
    FHEcontext context(m,p,r);
    long n = pow(p,r);

    long iter,iter1;

    AddPrimesBySize(context,5,false);
    buildModChain(context,L,c);
    FHESecKey secretKey(context);
    const FHEPubKey& publicKey = secretKey;
    secretKey.GenSecKey(w);
    addSome1DMatrices(secretKey);

    ZZX GZZX = context.alMod.getFactorsOverZZ()[0];
    EncryptedArray ea(context,GZZX);

    long nslots = ea.size();

    cout << "DONE Configuration" << endl;

    /* ----------- Open File for Read Input -----*/

    fstream infile("input_array.txt", fstream::in);

    std::string line;
    vector<long> valIn;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        long a;
        if (!(iss >> a)) { break; } // error
        valIn.push_back(a);
        // process pair (a,b)
    }
    /*---------------- Encrypting Process----------------*/

    long numVal = valIn.size();

    vector<Ctxt> vecCipher;

    for(iter = 0 ; iter < numVal ; iter++){
        Ctxt nC(publicKey);
        ea.encrypt(nC,publicKey,convertToCoeArray(valIn[iter],nslots));
        vecCipher.push_back(nC);
    }

    
    /* ----------- Open File for Writing -------*/
    fstream sendFile("sendF_encrypter.txt",fstream::out|fstream::trunc);
    assert(sendFile.is_open());


        // Output ContextBase
    writeContextBase(sendFile,context);
    sendFile << context << endl;
    
    // Output PublicKey
    sendFile << publicKey << endl;
    // Output Ciphertext
    sendFile << numVal << endl;
    for(iter = 0 ; iter < numVal ; iter++){
        sendFile << vecCipher[iter] << endl;
    }

    sendFile.close();

    /* ---------- Open File for Secret Writing ------ */

    fstream secretFile("secF_encrypter.txt",fstream::out|fstream::trunc);
    assert(secretFile.is_open());

    // Output ContextBase
    writeContextBase(secretFile,context);
    secretFile << context << endl;

    // Output ScretKey 
    secretFile << secretKey << endl;

    secretFile.close();

    cout << "DONE see sendF_encrypter.txt for sending,and secF_encrypter for decrypting " << endl;
}
