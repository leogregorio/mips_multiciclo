#ifndef INTERFACE_H
#define INTERFACE_H


#include<iostream>
#include<bitset>
#include<sstream>
#include<fstream>
#include<string>

#include"funcoesAux.h"
#include"execucao.h"
#include"Controle.h"

using namespace std;

int executaInstrucao(b32 instrucao, Controle& controle, int& PC,
                    b32 registradores[], b32 memoria[],
                    b32& A, b32& B,b32 regInstrucoes,b32 regDados,b32& ALUOut,
                    ofstream& saida);

void mostraRegistradoresNaoNulos(b32 registradores[])
{
    cout << "- Conteudo Registradores: (omitindo conteudos nulos)" << endl;
    for(int i=0; i<32; i++)
        if(registradores[i].to_ulong()!=0)
            cout << i << ": " << registradores[i] << endl;
    cout << endl;
}

void mostraRegistradores(b32 registradores[])
{
    cout << "- Conteudo Registradores:" << endl;
    for(int i=0; i<32; i++)
        cout << i << ": " << registradores[i] << endl;
    cout << endl;
}
void salvaRegistradores(b32 registradores[],ofstream& saida)
{
    saida << "- Conteudo Registradores:" << endl;
    for(int i=0; i<32; i++)
        saida << i << ": " << registradores[i] << endl;
    saida << endl;
}

void mostraMemoria(b32 memoria[])
{
    for(int i=0; i<256; i++)
        cout << i << ": " << memoria[i] << endl;
    cout << endl;
}
void salvaMemoria(b32 memoria[],ofstream& saida)
{
    saida << "- Conteudo Memoria:" << endl;
    for(int i=0; i<256; i++)
        saida << i << ": " << memoria[i] << endl;
    saida << endl;
}

//Funções para mostrar estado do sistema
void mostraEstado(int instrucaoAtual, int clock, int PC, Controle controle, b32 registradores[])
{
    cout << "- Instrucao: " << mostraInstrucao(instrucaoAtual) << endl;
    cout << "- Clock: " << clock << endl;
    cout << "- PC: " << PC << endl;
    mostraRegistradoresNaoNulos(registradores);
}


void salvaEstado(int instrucaoAtual, int clock, int PC, Controle controle, b32 registradores[],b32 memoria[], ofstream& saida)
{
    saida << "- Instrucao: " << mostraInstrucao(instrucaoAtual) << endl;
    saida << "- Clock: " << clock << endl;
    saida << "- PC: " << PC << endl;
    salvaRegistradores(registradores,saida);
    salvaMemoria(memoria,saida);
    controle.salvaEstadoControle(saida);
}

void mostraAuxiliares(b32 A, b32 B, b32 regDados, b32 regInstucoes, b32 ALUOut)
{
    cout << "- Conteudo dos registradores auxiliares:" << endl;
    cout << "Registrador A: " << A << endl;
    cout << "Registrador B: " << B << endl;
    cout << "Registrador de Dados: " << regDados << endl;
    cout << "Registrador de Instrucoes: " << regInstucoes << endl;
    cout << "ALUOut: " << ALUOut << endl << endl;
}




// MENUS


//MENUS PARA EXECUCAO DE INSTRUCOES


void menuExecucaoDireta(Controle& controle, int& PC,
                        b32 registradores[], b32 memoria[],
                        b32& A, b32& B,b32 regInstrucoes,b32 regDados,b32& ALUOut,
                        ofstream& saida)
{
    cout << "Executando todas as instrucoes ate encontrar com codigo binario 0" << endl;
    int cont = 0;
    while((memoria[PC/4]).to_ulong() != 0)
    {   
        cout << endl <<"###################################" << endl << "-----EXECUCAO numero " << cont << "-----" << endl << "###################################" << endl;
        cout << memoria[PC/4] << endl;
        cout << PC/4 << endl;
        executaInstrucao(memoria[PC/4],controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut,saida);
        cont++;
    }
}

void menuExecucaoMemoria(Controle& controle, int& PC,
                        b32 registradores[], b32 memoria[],
                        b32& A, b32& B,b32 regInstrucoes,b32 regDados,b32& ALUOut,
                        ofstream& saida)
{
    cout << "Insira a posicao na memoria da instrucao que deseja executar:" << endl;
    int posicao;
    cin >> posicao;

    PC = posicao*4;
    cout << "PC setado para " << PC << endl;
    executaInstrucao(memoria[PC/4],controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut,saida);
}

void menuExecucaoInput(Controle& controle, int& PC,
                        b32 registradores[], b32 memoria[],
                        b32& A, b32& B,b32 regInstrucoes,b32 regDados,b32& ALUOut,
                        ofstream& saida)
{
    string input;

    cout << "Insira o codigo binario da instrucao a ser executada:" << endl;
    cin >> input;

    b32 instrucao = stringToBitset(input);
    memoria[PC/4] = instrucao;

    executaInstrucao(memoria[PC/4],controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut,saida);        

}

int menuExecucao(Controle& controle, int& PC,
                    b32 registradores[], b32 memoria[],
                    b32& A, b32& B,b32 regInstrucoes,b32 regDados,b32& ALUOut,
                    ofstream& saida)
{
    int opcao;

    cout << endl << "- MENU DE EXECUCAO -" << endl;
    cout << "Insira a opcao:" << endl;
    cout << "1) Execucao direta (do valor atual de PC ate instrucao nula)" << endl;
    cout << "2) Executar instrucao na memoria" << endl;
    cout << "3) Executar instrucao inserindo comando binario" << endl;
    cout << "0) Voltar" << endl;

    cin >> opcao;

    switch (opcao)
    {
    case 0:
        return opcao;
    case 1:
        menuExecucaoDireta(controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut,saida);
        break;
    case 2:
        menuExecucaoMemoria(controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut,saida);
        break;
    case 3:
        menuExecucaoInput(controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut,saida);
        break;
//
    }

    return opcao;
}


//MENUS PARA RESETAR
int menuResets(Controle& controle, int& PC,
                b32 registradores[], b32 memoria[],
                b32& A, b32& B,b32 regInstrucoes,b32 regDados,b32& ALUOut,
                ofstream& saida)
{
    int opcao;
    

    cout << "- MENU DE RESETS -" << endl;
    cout << "Insira a opcao:" << endl;
    cout << "1) Resetar PC" << endl;
    cout << "2) Resetar memoria" << endl;
    cout << "3) Resetar banco de registradores" << endl;
    cout << "4) Resetar variaveis de controle" << endl;
    cout << "5) Resetar auxiliares" << endl;
    cout << "6) Resetar tudo" << endl;
    cout << "0) Voltar" << endl;

    cin >> opcao;
    switch(opcao)
    {
        case 0: return opcao;
        case 1: PC = 0;
            break;
        case 2: resetarMemoria(memoria);
            break;
        case 3: resetarRegistradores(registradores);
            break;
        case 4: controle.resetarControle();
            break;
        case 5: resetarAuxiliares(A,B,regDados,regInstrucoes);
            break;
    }

    return opcao;
}




//MENUS PARA DISPLAY
int menuDisplay(Controle& controle, int& PC,
                b32 registradores[], b32 memoria[],
                b32& A, b32& B,b32 regInstrucoes,b32 regDados,b32& ALUOut,
                ofstream& saida)
{
    int opcao; 
    cout << "- MENU DE DISPLAY -" << endl;
    cout << "Insira a opcao:" << endl;
    cout << "1) Mostrar memoria" << endl;
    cout << "2) Mostrar banco de registradores" << endl;
    cout << "3) Mostrar variaveis de controle" << endl;
    cout << "4) Mostrar registradores auxiliares" << endl;
    cout << "0) Voltar" << endl;

    cin >> opcao;

    switch(opcao)
    {
        case 1:
            mostraMemoria(memoria);
            break;
        case 2:
            mostraRegistradores(registradores);
            break;
        case 3:
            controle.displayEstadoControle();
            break;
        case 4:
            mostraAuxiliares(A,B,regDados,regInstrucoes,ALUOut);
            break;
    }

    return opcao;
}








int menuPrincipal(  Controle& controle, int& PC,
                    b32 registradores[], b32 memoria[],
                    b32& A, b32& B,b32 regInstrucoes,b32 regDados,b32& ALUOut,
                    ofstream& saida )
{
    int opcao;
    cout << " - MENU PRINCIPAL -" << endl;
    cout << "Insira a acao a ser executada:" << endl << endl;
    cout << "1) Execucao de instrucoes" << endl;
    cout << "2) Resets" << endl;
    cout << "3) Mostrar estados" << endl;
    cout << "4) Mostrar valor de PC" << endl;
    cout << "0) Finalizar" << endl;

    cin >> opcao;


    switch(opcao)
    {
        case 0: return opcao;
        case 1: menuExecucao(controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut,saida);
            break;
        case 2: menuResets(controle, PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut,saida);
            break;
        case 3: menuDisplay(controle,PC,registradores,memoria,A,B,regInstrucoes,regDados,ALUOut,saida);
            break;
        case 4: cout << "Valor de PC: " << PC << endl << endl;
    }

    return opcao;
}




#endif