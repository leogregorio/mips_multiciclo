#include<iostream>
#include<bitset>

#include"Controle.h"
#include"funcoesAux.h"
#include"execucao.h"

#define  b32 bitset<32>
using namespace std;





int main()
{
    b32 memoria[256];
    b32 registradores[32];
    Controle controle;

    // Registradores para multiciclo
    b32 regInstrucoes;
    b32 regDados;
    b32 A;
    b32 B;
    b32 ALUOut;

    int PC = 100;
    int clock = 0;
    int instrucaoAtual = -1; // instrução desconhecida


    string testeInstrucao = "00100000000000010000000011111111";//32bits
    b32 instrucao = b32(converteLinha(testeInstrucao));

    PC = executaInstrucao(instrucao, controle, PC, registradores, memoria, A, B, regInstrucoes, regDados, ALUOut);

    cout << "REGISTRADORES: " << endl;
    for(int i=0; i<32; i++)
        cout << i << ": " << registradores[i] << endl;
    cout << endl;

    return 0;
}