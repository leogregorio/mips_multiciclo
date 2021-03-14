#ifndef CONTROLE_H
#define CONTROLE_H

#include<iostream>
#include<sstream>
#include<fstream>
#include<bitset>

using namespace std;

class Controle
{
public:
    bool RegDst;
    bool RegWrite;
    bool ALUSrcA;
    bool MemRead;
    bool MemWrite;
    bool MemtoReg;
    bool IorD;
    bool IRWrite;
    bool PCWrite;
    bool PCWriteCond;
    int ALUSrcB;
    int ALUOp;
    int PCSource;



    Controle()
    {
        PCWrite = 0;
        IorD = 0;
        MemtoReg = 0;
        IRWrite = 0;
        PCWriteCond = 0;
        PCWrite = 0;
        MemWrite = 0;
        MemRead = 0;
        RegDst = 0;
        ALUSrcA = 0;
        ALUSrcB = 0;
        ALUOp = 0;
        PCSource = 0;
    }

    void displayEstadoControle()
    {
        cout << "\n- Controle:"
        << endl << "- PCWrite: "        << PCWrite
        << endl << "- IorD: "         << IorD 
        << endl << "- MemtoReg: "     << MemtoReg
        << endl << "- IRWrite: "      << IRWrite
        << endl << "- PCWriteCond: "  << PCWriteCond
        << endl << "- PCWrite: "      << PCWrite
        << endl << "- MemWrite: "     << MemWrite
        << endl << "- MemRead: "      << MemRead
        << endl << "- RegDst: "       << RegDst
        << endl << "- ALUSrcA: "      << ALUSrcA
        << endl << "- ALUSrcB: "      << bitset<2>(ALUSrcB)
        << endl << "- ALUOp: "        << bitset<2>(ALUOp)
        << endl << "- PCSource: "     << bitset<2>(PCSource) << endl;
    }

    void salvaEstadoControle(ofstream& saida)
    {
        saida << "\n- Controle:"
        << endl << "- PCWrite: "        << PCWrite
        << endl << "- IorD: "         << IorD 
        << endl << "- MemtoReg: "     << MemtoReg
        << endl << "- IRWrite: "      << IRWrite
        << endl << "- PCWriteCond: "  << PCWriteCond
        << endl << "- PCWrite: "      << PCWrite
        << endl << "- MemWrite: "     << MemWrite
        << endl << "- MemRead: "      << MemRead
        << endl << "- RegDst: "       << RegDst
        << endl << "- ALUSrcA: "      << ALUSrcA
        << endl << "- ALUSrcB: "      << bitset<2>(ALUSrcB)
        << endl << "- ALUOp: "        << bitset<2>(ALUOp)
        << endl << "- PCSource: "     << bitset<2>(PCSource) << endl;
    }

    void resetarControle()
    {
        PCWrite = 0;
        IorD = 0;
        MemtoReg = 0;
        IRWrite = 0;
        PCWriteCond = 0;
        PCWrite = 0;
        MemWrite = 0;
        MemRead = 0;
        RegDst = 0;
        ALUSrcA = 0;
        ALUSrcB = 0;
        ALUOp = 0;
        PCSource = 0;
    }
};







#endif