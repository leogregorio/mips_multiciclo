#ifndef FUNCOESAUX_H
#define FUNCOESAUX_H


#include<iostream>
#include<bitset>
#include<string>
#include"Controle.h"

using namespace std;

#define b32 bitset<32>




//FUNCOES PARA OBTER OS CAMPOS DA INSTRUCAO PARA FACILITAR IMPLEMENTACAO
bitset<6> getOp(b32 instrucao)
{
    b32 aux = instrucao >> 26;
    return bitset<6>(aux.to_ulong());
}

bitset<5> getRs(b32 instrucao)
{
    bitset<32> aux = instrucao << 6;
    aux = aux >> 27;
    return bitset<5>(aux.to_ulong());
}

bitset<5> getRt(b32 instrucao)
{
    bitset<32> aux = instrucao << 11;
    aux = aux >> 27;
    return bitset<5>(aux.to_ulong());
}

bitset<5> getRd(b32 instrucao)
{
    bitset<32> aux = instrucao << 16;
    aux = aux >> 27;
    return bitset<5>(aux.to_ulong());
}

bitset<16> getOffset(b32 instrucao)
{
    return bitset<16>(instrucao.to_ulong());
}

bitset<6> getFunct(b32 instrucao)
{
    return bitset<6>(instrucao.to_ulong());
}

bitset<26> getJump(b32 instrucao)
{
    return bitset<26>(instrucao.to_ulong());
}

bitset<5> getShamt(b32 instrucao)
{
    bitset<32> aux = instrucao << 16;
    aux = aux >> 27;
    return bitset<5>(aux.to_ulong());
}


bitset<32> enderecoJump(bitset<26> jump, bitset<32> pc_bin)
{
    pc_bin = (pc_bin >> 26);
    bitset<4> pc_4 = bitset<4>(pc_bin.to_ulong());

    string s1 = "00";
    string s2 = pc_4.to_string();
    string s3 = jump.to_string();

    string aux = s2 + s3;
    aux = aux + s1;

    return (bitset<32>(aux));
}



//EXTENSOR DE SINAL
b32 extSinal(bitset<16> offset)
{
  b32 aux = b32(offset.to_ulong());
  if(offset.test(15))
    for(int i=16; i<32; i++)
      aux.set(i);
  return aux;
}

//funcao que transoforma umas string de 0's e 1's em um bitset de 32 bits
b32 stringToBitset(string instrucao)
{
  b32 aux;
  for(int i=0; i<32; i++)
    if(instrucao[i] == '1')
      aux.set(31-i);
  return aux;
}



//funcao que recebe os campos da instrucao em questao 
//e retorna a instrucao a ser realizada de acordo com a codificacao:
/*
    R:
        0:add   
        1:sub, 
        2:and, 
        3:or, 
        4:slt, 
        5:sll, 
    I:
        6:addi, 
        7:lw, 
        8:sw, 
        9:beq, 
        10:bne, 
    J:
        11:j, 
        12:jr, 
        13:jal,
 */

int decodificaInstrucao(bitset<6> opcode, bitset<6> funct)
{   
    /*
    R:
        0:add   
        1:sub, 
        2:and, 
        3:or, 
        4:slt, 
        5:sll, 
    I:
        6:addi, 
        7:lw, 
        8:sw, 
        9:beq, 
        10:bne, 
    J:
        11:j, 
        12:jr, 
        13:jal,
    */
   
int f = funct.to_ulong();
// Caso o OpCode seja [00 0000] -> TIPO R
   if( opcode.none()) 
   {
        int f = funct.to_ulong();

        switch(f)
        {
            case 32:
                return 0; // add
            case 34:
                return 1; // sub
            case 36:
                return 2; // and
            case 37:
                return 3; // or
            case 42:
                return 4; //slt
            case 0:
                return 5; //sll
            case 8:
                return 12;//jr
            default:
                return -1;
        }
   }


    int op = opcode.to_ulong();

switch(op)
{
    case 8:
        return 6; //addi
    case 35:    
        return 7; //lw
    case 43:
        return 8; //sw
    case 4:
        return 9; //beq
    case 5:
        return 10;//bne
    case 2:
        return 11;//jump
    case 3:
        return 13;//jal
    default:
        return -1;

}




}



string mostraInstrucao(int instrucao)
{
    switch(instrucao)
    {
        
    //R:
        case 0: return "add";  
        case 1: return "sub"; 
        case 2: return "and"; 
        case 3: return "or";
        case 4: return "slt"; 
        case 5: return "sll";
    //I:
        case 6: return "addi"; 
        case 7: return "lw"; 
        case 8: return "sw";
        case 9: return "beq";
        case 10: return "bne"; 
    //J:
        case 11: return "j";
        case 12: return "jr";
        case 13: return "jal";
        default:
            return "Desconhecida";
    
    }
}

b32 converteLinha(string instrucao)
{
    b32 aux;
    for(int i=0; i<32; i++)
        if(instrucao[i] == '1')
            aux.set(31-i);
    return aux;
}

int ALUresult(int instrucaoAtual, Controle controle, int A, int B)
{
    if(controle.ALUOp == 0)
        if(instrucaoAtual == 10)
            return !(A - B);
        else
            return A + B;
    
    if(controle.ALUOp == 1)
        return A - B;
    
    if(controle.ALUOp == 2)
    {
        switch (instrucaoAtual)
        {
        case 0:
            return A + B;
        case 1:
            return A - B;
        case 2:
            return A & B;
        case 3:
            return A | B;
        case 4:
            return A < B;
        case 5:
            return A << B;
        default:
            break;
        }
    }
    return -1;
}



//Funções para resetar os elementos de estado

void resetarMemoria(b32 memoria[])
{
    for(int i=0; i<32; i++)
        memoria[i].reset();
    cout << "Memoria resetada." << endl;
}

void resetarRegistradores(b32 registradores[])
{
    for(int i=0; i<32; i++)
        registradores[i].reset();

    cout << "Registradores resetados." << endl;
}


void resetarAuxiliares(b32 A, b32 B, b32 regDados, b32 regInstrucoes)
{
    A = b32(0);
    B = b32(0);
    regDados = b32(0);
    regInstrucoes = b32(0);
    cout << "Auxiliares resetados." << endl;
}

void resetarTudo(b32 memoria[], b32 registradores[], Controle controle, b32 A, b32 B, b32 regDados, b32 regInstrucoes)
{
    resetarRegistradores(registradores);
    resetarMemoria(memoria);
    controle.resetarControle();
    A = b32(0);
    B = b32(0);
    regDados = b32(0);
    regInstrucoes = b32(0);
}





#endif