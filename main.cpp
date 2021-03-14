#include<iostream>
#include<bitset>
#include<sstream>
#include<fstream>
#include<string>

#include"Controle.h"
#include"funcoesAux.h"
#include"execucao.h"

#define  b32 bitset<32>
using namespace std;



int main(int argc, const char* argv[])
{       
    //Manipulacao de arquivo
    ifstream entrada;
    ofstream saida;
    entrada.open(argv[1], ios::in);
    saida.open(argv[2], ios::out | ios::trunc);


    //Estruturas
    b32 memoria[256];
    b32 registradores[32];
    Controle controle;

    // Registradores para multiciclo
    b32 regInstrucoes;
    b32 regDados;
    b32 A;
    b32 B;
    b32 ALUOut;

    int PC = 0;
    int clock = 0;
    int instrucaoAtual = -1; // instrução desconhecida

    //criando lista de instrucoes a partir de arquivo
    b32* listaInstrucoes = new b32[13];
    criaListaInstrucoes(entrada,listaInstrucoes);

    
    //salvando a lista de instrucoes na memoria a partir da posicao 0
    for(int i=0; i<13; i++)
        memoria[i] = listaInstrucoes[i];
    delete[] listaInstrucoes;

    
    //execucao direta
    int cont = 0;

    
    while(cont < 13)
    {   
        cout << endl <<"###################################" << endl << "-----EXECUCAO numero " << cont << "-----" << endl << "###################################" << endl;
        cout << memoria[PC/4] << endl;
        cout << PC/4 << endl;
        executaInstrucao(memoria[PC/4],controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut);
        cont++;
        mostraRegistradores(registradores);
    }
    

    executaInstrucao(stringToBitset("00000001100000000000000010000000"),controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut);
    mostraRegistradores(registradores);

    return 0;
}