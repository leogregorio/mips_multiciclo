#ifndef EXECUCAO_H
#define EXECUCAO_H


#include<iostream>
#include<bitset>
#include<string>
#include<sstream>
#include<fstream>

#include"Controle.h"
#include"funcoesAux.h"
#include"interface.h"

using namespace std;

#define b32 bitset<32>


void mostraEstado(int instrucaoAtual, int clock, int PC, Controle controle,b32 registradores[]);
void salvaEstado(int instrucaoAtual, int clock, int PC, Controle controle, b32 registradores[],b32 memoria[], ofstream& saida);



//funcoes para manipilacao de arquivo e criacao de lista de instrucoes

int numeroInstrucoes(ifstream& entrada)
{
    string aux;
    int cont = 0;
    while(!entrada.eof())
    {
        getline(entrada,aux);
        cont++;
    }
    return cont;
}

void criaListaInstrucoes(ifstream& entrada,b32 lista[])
{
    //retornar ao inicio do arquivo
    entrada.clear();
    entrada.seekg(0);
    //leitura do arquivo
    string linha;
    getline(entrada,linha);

    int i = 0;
    lista[0] = stringToBitset(linha);
    
    i++;
    
    while(!entrada.eof())
    {
        getline(entrada,linha);
        lista[i] = stringToBitset(linha);
        i++;
    }
    
}


// execução de uma instrução individual
int executaInstrucao(b32 instrucao, Controle& controle, int& PC,
                    b32 registradores[], b32 memoria[],
                    b32& A, b32& B,b32 regInstrucoes,b32 regDados,b32& ALUOut,
                    ofstream& saida)
{

    int instrucaoAtual = -1; // Instrucao desconhecida
    int clock = 0;  // clock reseta toda execução
    


    cout << "**************************************** " << endl;
    cout << "Etapa: INICIO DA EXECUCAO " << endl;
    saida << "Etapa: INICIO DA EXECUCAO " << endl;
    mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
    salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
    cout << "\n**************************************** " << endl;


// BUSCA DA INSTRUCAO

    


    memoria[PC/4] = instrucao;
    regInstrucoes = memoria[PC/4];

    controle.MemRead = 1; // ler a instrução da memória
    controle.MemWrite = 1; // selecionar que vai ler da memoria
    controle.IorD = 0; // selecionar valor que vem do PC
    controle.ALUSrcA = 0; // A = PC
    controle.ALUSrcB = 1; // B = 4
    controle.ALUOp =  0; //SOMA
    controle.PCWrite = 1; // escrever o endereço incrementado no PC
    controle.PCSource = 0; // 

    if(controle.PCSource == 00 && controle.PCWrite == 01)
        ALUOut = (ALUresult(0,controle,PC,4)); //PC = PC + 4
    PC = ALUOut.to_ulong();



    clock ++; //termina ciclo de clock
    cout << "**************************************** " << endl;
    cout << "Etapa: BUSCA DA INSTRUCAO " << endl;
    saida << "Etapa: BUSCA DA INSTRUCAO " << endl;
    mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
    salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
    cout << "\n**************************************** " << endl;

    

// DECODIFICAÇÃO DA INSTRUCAO

    
    controle.ALUSrcA = 0; //PC para ALU
    controle.ALUSrcB = 3; //offset depois da extensao de sinal para ALU

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

    ALUOut =  PC + (getOffset(instrucao)<<2).to_ulong(); //endereço de desvio já na ALUOut caso seja branch



    clock ++; //termina ciclo de clock
    cout << "**************************************** " << endl;
    cout << "Etapa: DECODIFICACAO " << endl;
    saida << "Etapa: DECODIFICACAO " << endl;
    mostraEstado(instrucaoAtual, clock, PC, controle,registradores); 
    salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
    cout << "\n**************************************** " << endl;


    

// ETAPA 3: EXECUCAO

// ############# PONTO DE DIVISAO #######################
    if( instrucaoAtual >= 0 && instrucaoAtual <= 4  ) //TIPO R
    {
        controle.ALUSrcA = 1; //Registrador A para ALU
        controle.ALUSrcB = 0; //Registrador B para ALU
        controle.ALUOp = 2;   // campo funct vai ser usado para determinar operação
        
        ALUOut = ALUresult(instrucaoAtual,controle,A.to_ulong(),B.to_ulong()); //ALUOut guarda resultado da operação

        clock ++; //termina ciclo de clock

        cout << "\n**************************************** " << endl;
        cout << "Etapa: EXECUCAO INSTRUCAO TIPO-R " << endl;
        saida << "Etapa: EXECUCAO INSTRUCAO TIPO-R" << endl;
        mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
        salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
        cout << "\n**************************************** " << endl;




        // ETAPA 4: COMPLETAGEM DE INSTRUCAO R

        controle.RegDst = 1; // campo rd vai ser usado como destino
        controle.RegWrite = 1; // escrita no registrador
        controle.MemtoReg = 0; // escrita não vem da memoria

        bitset<5> rd = getRd(instrucao);
        registradores[rd.to_ulong()] = ALUOut;  //escrever no registrador indicado por rd      
    



        clock ++; //termina ciclo de clock
        cout << "\n**************************************** " << endl;
        cout << "Etapa: CONCLUSAO DA INSTRUCAO TIPO-R " << endl;
        saida << "Etapa: CONCLUSAO INSTRUCAO TIPO-R " << endl;
        mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
        salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
        cout << "\n**************************************** " << endl;

        

    }
    else if(instrucaoAtual == 5) //shift left logical
    {
        controle.RegDst = 1;
        controle.MemRead = 0;
        controle.MemtoReg = 0;

        bitset<5> shamt = getShamt(instrucao);
        ALUOut = (A.to_ulong() << shamt.to_ulong());

        clock++;

        cout << "\n**************************************** " << endl;
        cout << "Etapa: EXECUCAO INSTRUCAO TIPO-R " << endl;
        saida << "Etapa: EXECUCAO INSTRUCAO TIPO-R " << endl;
        mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
        salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
        cout << "\n**************************************** " << endl;



        // ETAPA 4: COMPLETAGEM DE INSTRUCAO R

        controle.RegDst = 1; // campo rd vai ser usado como destino
        controle.RegWrite = 1; // escrita no registrador
        controle.MemtoReg = 0; // escrita não vem da memoria

        bitset<5> rd = getRd(instrucao);
        registradores[rd.to_ulong()] = ALUOut;  //escrever no registrador indicado por rd      
    

        clock ++; //termina ciclo de clock
        cout << "\n**************************************** " << endl;
        cout << "Etapa: CONCLUSAO DA INSTRUCAO TIPO-R " << endl;
        saida << "Etapa: CONCLUSAO DA INSTRUCAO TIPO-R " << endl;
        mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
        salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
        cout << "\n**************************************** " << endl;

        

    }
    else if( instrucaoAtual >= 6 && instrucaoAtual <= 10) //TIPO I
    {
        //ETAPA 3
        controle.ALUSrcA = 1; //primeiro input da ALU é A
        controle.ALUSrcB = 2; //segundo input da ALU é unidade de extensao de sinal
        controle.ALUOp = 0; //ALU realiza soma

        b32 extendido = (extSinal(offset)); //extensao de sinal do campo offset

        //ALUOut guardando o endereço de branch pra ser usado se for o caso
        //ALUOut = PC + 4 + extendido.to_ulong();

        
        clock++; //final do ciclo de clock    

        cout << "\n**************************************** " << endl;
        cout << "Etapa: COMPUTAR ENDERECO DE MEMORIA  " << extendido << endl ;
        saida << "Etapa: COMPUTAR ENDERECO DE MEMORIA  " << extendido << endl ;
        mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
        salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
        cout << "\n**************************************** " << endl;

        

        //ETAPA 4: ACESSO A MEMORIA
        
        if(instrucaoAtual == 7)      //load word
        {
            controle.MemRead = 1; //ler da memoria 
            controle.IorD = 1; //forçar endereço de memória a vir do ALU


            ALUOut = b32(extendido.to_ulong() + rs.to_ulong());
            regDados = memoria[ALUOut.to_ulong()]; //registrador de dados recebe dados da memoria

            clock++; //final do ciclo de clock

            cout << "\n**************************************** " << endl;
            cout << "Etapa: ACESSO A MEMORIA  " << endl;
            saida << "Etapa: ACESSO A MEMORIA  " << endl;
            mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
            salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
            cout << "\n**************************************** " << endl;

            

            //ETAPA 5 - COMPLETAR A LEITURA

            controle.MemtoReg = 1; //escrever o resultado da memoria no registrador
            controle.RegWrite = 1; //causar uma escrita
            controle.RegDst = 0; //escolher o campo rt como numero do resgistrador

            registradores[rt.to_ulong()] = regDados;



            clock++; //final do ciclo de clock

            cout << "\n**************************************** " << endl;
            cout << "Etapa: COMPLETAR LEITURA  " << endl;
            saida<< "Etapa: COMPLETAR LEITURA  " << endl;
            mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
            salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
            cout << "\n**************************************** " << endl;

            
                                    

        }
        else if(instrucaoAtual == 8) // store word
        {

            //etapa 4 
            
            controle.MemWrite = 1; //escrever na memoria
            controle.IorD = 1;  //forçar endereço de memoria a vir da ALU

            ALUOut = b32(extendido.to_ulong() + rs.to_ulong());

            memoria[ALUOut.to_ulong()] = B;            

                  
            clock++; //final do ciclo de clock

            cout << "\n**************************************** " << endl;
            cout << "Etapa: ACESSO A MEMORIA  " << endl;
            saida << "Etapa: ACESSO A MEMORIA  " << endl;
            mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
            salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
            cout << "\n**************************************** " << endl;

            

        }
        else if(instrucaoAtual == 6) //addi
        {
            controle.RegWrite = 1; //causar uma escrit
            controle.RegDst = 0; //escolher o campo rt como numero do resgistrador 
            controle.MemtoReg = 0; //registrador escrito com dado da ALU

            registradores[rt.to_ulong()] = extSinal(getOffset(instrucao));



            clock++; //final do ciclo de cloc

            cout << "\n**************************************** " << endl;
            cout << "Etapa: ESCREVE RESULTADO NO REGISTRADOR  " << endl;
            saida << "Etapa: ESCREVE RESULTADO NO REGISTRADOR  " << endl;
            mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
            salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
            cout << "\n**************************************** " << endl;

        
                                    
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
            bool Zero =  !(ALUresult(instrucaoAtual,controle,A.to_ulong(),B.to_ulong()));

            if(Zero && controle.PCWriteCond)//caso de igualdade entre registradores A e B (branch tomado)
            {
                PC = ALUOut.to_ulong();
            }

            clock++; //final do ciclo de clock

            cout << "\n**************************************** " << endl;
            cout << "Etapa: COMPLETANDO OPERACAO DE DESVIO  " << endl;
            saida << "Etapa: COMPLETANDO OPERACAO DE DESVIO  " << endl;
            mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
            salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
            cout << "Zero: " << Zero << endl;
            cout << "\n**************************************** " << endl;

            

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
            bool Zero =  ALUresult(instrucaoAtual,controle,A.to_ulong(),B.to_ulong());

            if((Zero) && controle.PCWriteCond)//caso de desigualdade entre registradores A e B (branch tomado)
            {
                PC = ALUOut.to_ulong(); //PC se torna endereço com desvio
            }


            clock++; //final do ciclo de clock

            cout << "\n**************************************** " << endl;
            cout << "Etapa: COMPLETANDO OPERACAO DE DESVIO  " << endl;
            saida << "Etapa: COMPLETANDO OPERACAO DE DESVIO  " << endl;
            mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
            salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
            cout << "\n**************************************** " << endl;

            

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
            PC = (registradores[(getRs(instrucao)).to_ulong()].to_ulong())<<2; // PC novo é o endereço indicado
        }
        else //jal
        {           
            controle.RegWrite = 1;
            registradores[31] = PC + 8;

            bitset<26> jump = getJump(instrucao); //obtem o numero de 26 bits correspondente ao jump
            b32 endJump = enderecoJump(jump, b32(PC)); //calcula o endereço alvo do jump
            PC = endJump.to_ulong(); // PC novo é o endereço indicado

        }

        
            clock++; //final do ciclo de clock

            cout << "\n**************************************** " << endl;
            cout << "Etapa: COMPLETANDO OPERACAO DE JUMP  " << endl;
            saida << "Etapa: COMPLETANDO OPERACAO DE JUMP  " << endl;
            mostraEstado(instrucaoAtual, clock, PC, controle,registradores);
            salvaEstado(instrucaoAtual,clock,PC,controle,registradores,memoria,saida);
            cout << "\n**************************************** " << endl;

            
    }

    return PC;
}


//executa todas as instruções até encontrar a instrução 0


#endif