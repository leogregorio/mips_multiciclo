/*

UNIVERSIDADE FEDERAL DE JUIZ DE FORA
INSTITUTO DE CIÊNCIAS EXATAS
DEPARTAMENTO DE CIÊNCIA DA COMPUTAÇÃO

DCC070 - Organização de Computadores
TVC 2 - Trabalho de Implementaçao MIPS Multiciclo
Professor: Marcelo Lobosco

AUTOR:
-LEONARDO GREGÓRIO DE ANDRADE 201865501B

*/
#include<iostream>
#include<bitset>
#include<sstream>
#include<fstream>
#include<string>

#include"Controle.h"
#include"funcoesAux.h"
#include"execucao.h"
#include"interface.h"

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
    int N = numeroInstrucoes(entrada);
    b32* listaInstrucoes = new b32[N];
    criaListaInstrucoes(entrada,listaInstrucoes);

    
    //salvando a lista de instrucoes na memoria a partir da posicao 0
    for(int i=0; i<N; i++)
        memoria[i] = listaInstrucoes[i];
    delete[] listaInstrucoes;

    int flag = 1;
    while(flag)
    {
        flag = menuPrincipal(controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut,saida);
    }



    return 0;
}