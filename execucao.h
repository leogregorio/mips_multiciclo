#ifndef EXECUCAO_H
#define EXECUCAO_H


#include<iostream>
#include<bitset>

#include"Controle.h"
#include"funcoesAux.h"

using namespace std;

#define b32 bitset<32>

//Função para mostrar o estado do sistema
void mostrarEstado(int instrucaoAtual, int clock, int PC, b32 A, b32 B, Controle controle)
{
    cout << "Instrucao: " << mostraInstrucao(instrucaoAtual) << endl;
    cout << "Clock: " << clock << endl;
    cout << "PC: " << PC << endl;
    cout << "Registrador A: " << A.to_ulong() << endl;
    cout << "Registrador B: " << B.to_ulong() << endl;
    controle.displayEstadoControle();
}



int executaInstrucao(b32 instrucao, Controle controle, int& PC,
                    b32 registradores[], b32 memoria[],
                    b32& A, b32& B,b32 regInstrucoes,b32 regDados,b32 ALUOut)
{


    int instrucaoAtual = -1; // Instrucao desconhecida
    int clock = 0;  // clock reseta toda execução
    


    cout << "****************************************\n";
    cout << "INICIO DA EXECUCAO\n";
    mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
    cout << "\n****************************************\n";


// INSTRUCTION FETCH

    clock ++;


    memoria[PC] = instrucao;
    regInstrucoes = memoria[PC];


        //primeira metade
    if(!controle.IorD) //a partir do PC
        if(controle.IRWrite)
            regInstrucoes = memoria[PC];


    controle.MemRead = 1;
    controle.MemWrite = 1; // selecionar que vai ler da memoria
    controle.IorD = 0; // selecionar valor que vem do PC
    controle.ALUSrcA = 0; // A = PC
    controle.ALUSrcB = 1; //B = 4
    controle.ALUOp =  0; //SOMA
    controle.PCWrite = 1;
    controle.PCSource = 0;

    if(controle.PCSource == 00 && controle.PCWrite == 01)
        ALUOut = (ALUresult(0,controle,PC,4)); //PC = PC + 4
    PC = ALUOut.to_ulong();




    cout << "****************************************\n";
    cout << "INSTRUCTION FETCH\n";
    mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
    cout << "\n****************************************\n";

// DECODE

    clock++; 
    
    controle.ALUSrcA = 0; //PC para ALU
    controle.ALUSrcB = 3; //

    //campos da instrucao para fins de implementacao
    bitset<6> opcode = getOp(regInstrucoes);
    bitset<5> rs = getRs(regInstrucoes);
    bitset<5> rt = getRt(regInstrucoes);
    bitset<6> funct = getFunct(regInstrucoes);
    bitset<16> offset = getOffset(regInstrucoes);

    //decodificacao da instrucao
    instrucaoAtual = decodificaInstrucao(opcode,funct);




    A = registradores[rs.to_ulong()];
    B = registradores[rt.to_ulong()];

    b32 extensaoSinal = extSinal(getOffset(regInstrucoes));

    ALUOut = ALUresult(00,controle,PC,(extensaoSinal.to_ulong()<<2)); //endereço de desvio já na ALUOut caso seja branch



    cout << "****************************************\n";
    cout << "DECODIFICACAO\n";
    mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
    cout << "\n****************************************\n";

// ETAPA 3: EXECUCAO

    clock++;
// ############# PONTO DE DIVISAO #######################
    if( instrucaoAtual >= 0 && instrucaoAtual <= 5 ) //TIPO R
    {
        controle.ALUSrcA = 1;
        controle.ALUSrcB = 0;
        controle.ALUOp = 2;
        
        ALUOut = ALUresult(instrucaoAtual,controle,A.to_ulong(),B.to_ulong());


        cout << "\n****************************************\n";
        cout << "EXECUCAO R\n";
        mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
        cout << "\n****************************************\n";




        // ETAPA 4: COMPLETAGEM DE INSTRUCAO R
        clock++;

        if( instrucaoAtual >= 0 && instrucaoAtual <= 5 ) 
        {
            controle.RegDst = 1;
            controle.RegWrite = 1;
            controle.MemtoReg = 0;

            bitset<5> rd = getRd(instrucao);
            //Escrever no registrador rd
            registradores[rd.to_ulong()] = ALUOut;        
        }


        cout << "\n****************************************\n";
        cout << "COMPLETAGEM DE INSTRUCAO R\n";
        mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
        cout << "\n****************************************\n";
    }
    else if( instrucaoAtual >= 6 && instrucaoAtual <= 10) //TIPO I
    {
        //ETAPA 3
        controle.ALUSrcA = 1; //primeiro input da ALU é A
        controle.ALUSrcB = 2; //segundo input da ALU é unidade de extensao de sinal
        controle.ALUOp = 0; //ALU realiza soma

        b32 extendido = extSinal(offset);

        //ALUOut guardando o endereço de branch pra ser usado se for o caso
        ALUOut = ALUresult(instrucaoAtual,controle,A.to_ulong(),extendido.to_ulong());

        
        
        cout << "\n****************************************\n";
        cout << "COMPUTAR ENDEREÇO DE MEMORIA \n";
        mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
        cout << "\n****************************************\n";



        //ETAPA 4: ACESSO A MEMORIA
        
        if(instrucaoAtual == 7)      //load word
        {
            controle.MemRead = 1;
            controle.IorD = 1; //forçar endereço de memória a vir do ALU

            regDados = memoria[ALUOut.to_ulong()];

            cout << "\n****************************************\n";
            cout << "ACESSO A MEMORIA \n";
            mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
            cout << "\n****************************************\n";

            //ETAPA 5 - COMPLETAR A LEITURA

            controle.MemtoReg = 1; //escrever o resultado da memoria no registrador
            controle.RegWrite = 1; //causar uma escrita
            controle.RegDst = 0; //escolher o campo rt como numero do resgistrador

            registradores[rt.to_ulong()] = regDados;



            cout << "\n****************************************\n";
            cout << "COMPLETAR LEITURA \n";
            mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
            cout << "\n****************************************\n";
                                    
            //    ~FIM DA LEITURA~     //

        }
        else if(instrucaoAtual == 8) // store word
        {

            //etapa 4 
            
            controle.MemWrite = 1;
            controle.IorD = 1;

            memoria[ALUOut.to_ulong()] = B;

            cout << "\n****************************************\n";
            cout << "ACESSO A MEMORIA \n";
            mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
            cout << "\n****************************************\n";

        }
        else if(instrucaoAtual == 6) //addi
        {
            controle.RegWrite = 1; //causar uma escrita
            controle.RegDst = 0; //escolher o campo rt como numero do resgistrador
            controle.MemtoReg = 0; //registrador escrito com dado da ALU

            registradores[rt.to_ulong()] = ALUOut;


            cout << "\n****************************************\n";
            cout << "ESCREVE RESULTADO NO REGISTRADOR \n";
            mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
            cout << "\n****************************************\n";
                                    
            //    ~FIM DA LEITURA~     //
        }
        else if(instrucaoAtual == 9) //beq
        {
            controle.ALUSrcA = 1; 
            controle.ALUSrcB = 0; 
            controle.ALUOp = 1; // subtração
            controle.PCWrite = 1;   
            controle.PCSource = 1; // PC vem de ALUOut
            controle.PCWriteCond = 1;   // atualizar PC se o output Zero estiver em alta
            
            //Porta Zero que indica se os valores passados para a ALU são iguais (1->iguais)
            bool Zero =  ALUresult(instrucaoAtual,controle,A.to_ulong(),B.to_ulong());

            if(Zero && controle.PCWriteCond)//caso de igualdade entre registradores A e B (branch tomado)
            {
                PC = ALUOut.to_ulong();


            }

        cout << "\n****************************************\n";
        cout << "COMPLETANDO OPERACAO DE DESVIO \n";
        mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
        cout << "\n****************************************\n";


            // FIM DA EXECUCAO
              
        }
        else                         //bne
        {
            controle.ALUSrcA = 1; 
            controle.ALUSrcB = 0; 
            controle.ALUOp = 1; // subtração
            controle.PCWrite = 1;   
            controle.PCSource = 1; // PC vem de ALUOut
            controle.PCWriteCond = 1;   // atualizar PC se o output Zero estiver em alta
            
            //porta Zero que indica se os valores passados para a ALU são diferentes (1->diferentes)
            bool Zero =  !(ALUresult(instrucaoAtual,controle,A.to_ulong(),B.to_ulong()));

            if((Zero) && controle.PCWriteCond)//caso de desigualdade entre registradores A e B (branch tomado)
            {
                PC = ALUOut.to_ulong(); //PC se torna endereço com desvio
            }

            cout << "\n****************************************\n";
            cout << "COMPLETANDO OPERACAO DE DESVIO \n";
            mostrarEstado(instrucaoAtual, clock, PC, A, B, controle);
            cout << "\n****************************************\n";


            // FIM DA EXECUCAO
              
        }

    }
    else if(instrucaoAtual >= 11 && instrucaoAtual <= 13) // TIPO J
    {
        controle.PCSource = 1; //direcionar o endereco de jump para PC
        controle.PCWrite = 1;  //escrever o endereço de jump no PC

        if(instrucaoAtual == 11)//j
        {
            bitset<26> jump = getJump(instrucao); //obtem o numero de 26 bits correspondente ao jump
            b32 endJump = enderecoJump(jump, b32(PC)); //calcula o endereço alvo do jump
            PC = endJump.to_ulong(); // PC novo é o endereço indicado
        }
        else if(instrucaoAtual == 12)//jr
        {
            PC = A.to_ulong(); // PC novo é o endereço indicado
        }
        else //jal
        {           
            controle.RegWrite = 1;
            registradores[31] = PC + 8;

            bitset<26> jump = getJump(instrucao); //obtem o numero de 26 bits correspondente ao jump
            b32 endJump = enderecoJump(jump, b32(PC)); //calcula o endereço alvo do jump
            PC = endJump.to_ulong(); // PC novo é o endereço indicado

        }
    }

    return PC;
}




#endif