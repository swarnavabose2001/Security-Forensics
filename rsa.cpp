#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;


int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int modInverse(int e, int phi) {
    int d = 0, x1 = 0, x2 = 1, y1 = 1, tempPhi = phi;
    while (e > 0) {
        int temp1 = tempPhi / e;
        int temp2 = tempPhi - temp1 * e;
        tempPhi = e;
        e = temp2;

        int x = x2 - temp1 * x1;
        int y = d - temp1 * y1;

        x2 = x1;
        x1 = x;
        d = y1;
        y1 = y;
    }
    return (d + phi) % phi;
}


long long powerMod(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod; 
    while (exp > 0) {
        if (exp % 2 == 1) 
            result = (result * base) % mod;
        exp = exp >> 1;  
        base = (base * base) % mod; 
    }
    return result;
}

int main() {
    int p, q;
    cout << "Enter two prime numbers (p and q): ";
    cin >> p >> q;

  
    long long n = (long long)p * q;
    long long phi = (long long)(p - 1) * (q - 1);

    int e;
    do {
        e = rand() % (phi - 2) + 2; 
    } while (gcd(e, phi) != 1);


    int d = modInverse(e, phi);

    cout << "\nPublic Key (e, n): (" << e << ", " << n << ")";
    cout << "\nPrivate Key (d, n): (" << d << ", " << n << ")\n";

    string msg;
    cout << "Enter message (as a string) to encrypt: ";
    cin.ignore();
    getline(cin, msg);

 
    vector<long long> encryptedMsg;
    cout << "Encrypted Message: ";
    for (char ch : msg) {
        long long encryptedChar = powerMod((long long)ch, e, n);
        encryptedMsg.push_back(encryptedChar);
        cout << encryptedChar << " ";
    }
    cout << endl;

    cout << "Decrypted Message: ";
    for (long long encChar : encryptedMsg) {
        char decryptedChar = (char)powerMod(encChar, d, n);
        cout << decryptedChar;
    }
    cout << endl;

    return 0;
}



