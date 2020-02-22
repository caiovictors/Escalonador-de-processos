//  CAIO VICTOR DO AMARAL CUNHA SARMENTO
//  UFPB - CENTRO DE INFORMATICA -  2020
//       ENGENHARIA DE COMPUTACAO

#include <stdio.h>
#include <stdlib.h>

void InsertionSort(int num, int *time_a, int *duration)
{
    int i, j, temp, temp2;
    for (i = 1; i < num; i++){
        temp = time_a[i];
        temp2 = duration[i];
        j = i - 1;
        while((temp < time_a[j]) && (j >= 0)){
            time_a[j+1] = time_a[j];
            duration[j+1] = duration[j];
            j = j-1;
        }
        time_a[j+1] = temp;
        duration[j+1] = temp2;
    }
}

struct Queue            //fila pro RR
{
	int capacidade;
	float *dados;
	int first;
	int last;
	int nItens; 
};

void criarQueue( struct Queue *f, int c)                //cria a fila
{ 
	f->capacidade = c;
	f->dados = (float*) malloc (f->capacidade * sizeof(float));
	f->first = 0;
	f->last = -1;
	f->nItens = 0; 
}

void inserir(struct Queue *f, int v)            //insere na fila
{
	if(f->last == f->capacidade-1)
		f->last = -1;

	f->last++;
	f->dados[f->last] = v;                      // incrementa last e insere
	f->nItens++;                                // mais um item inserido

}

int remover( struct Queue *f ) {               // pega o item do comeco da Queue

	int temp = f->dados[f->first++];           // pega o valor e incrementa o first

	if(f->first == f->capacidade)
		f->first = 0;

	f->nItens--;                               // um item retirado
	return temp;
}

void FCFS (int num, int *time_a, int *duration)
{
    float t_retorno = 0, t_resposta = 0, t_espera = 0;
    int time = 0, t_aux[num], dur_aux[num], i = 0, count = 0, cancel = 0, first = 0;

    for(int x = 0; x < num; x++){       //cria copia dos arrays
        t_aux[x] = time_a[x];
        dur_aux[x] = duration[x];
        if(dur_aux[x] <= 0) cancel++;    //cancela programa com duracao 0
    }

    while(1){
        for(int z = 0; z < num; z++){
            if(dur_aux[z] <= 0)     //incrementa contador caso processo tenha duracao igual ou menor que 0
                count ++; 
        }if(count == num){          //testa se todos sao de duracao 0 ou menos
            break;
        }else{
            count = 0;
        }
        if(dur_aux[i] <= 0){        //caso tenha duracao 0 o tempo fica 0 para nao entrar no calculo
            t_aux[i] = 0;
            i++;    
        
        }else if(!first && dur_aux[i] > 0 && time == time_a[i]){    //primeiro processo
            t_retorno += dur_aux[i];
            dur_aux[i] = 0;
            time += t_retorno;
            i++;
            first = 1;
        }else if(time >= t_aux[i]){             //segundo ou + processos
            t_espera += time - t_aux[i];
            time += dur_aux[i];
            t_retorno += time - t_aux[i];
            dur_aux[i] = 0;
            i++;
        }else{
            time++;
        }
    }
    if(cancel == num){                      //caso todos os programas tenham duracao 0
        t_retorno = 0;
        t_espera = 0;
        t_resposta = 0;
    }else{                                  //elimina os programas do duracao 0
        num -= cancel;
    }
    t_retorno = t_retorno/num;
    t_espera = t_espera/num;
    t_resposta = t_espera;

    printf("FCFS %.1f %.1f %.1f\n", t_retorno, t_resposta, t_espera);
}

void SJF (int num, int *time_a, int *duration)
{
    float t_retorno = 0, t_resposta = 0, t_espera = 0;
    int time = 0, menor = 0, index, i = 0, count = 0, first = 0, count2 = 0;
    int t_aux[num], dur_aux[num], cancel = 0;

    for(int x = 0; x < num; x++){       //cria copia dos arrays
        t_aux[x] = time_a[x];
        dur_aux[x] = duration[x];
        if(dur_aux[x] <= 0) cancel++;   //cancela programa com duracao 0
    }

    while(1){
        
        for(int x = 0; x < num; x++){   
            if(dur_aux[x] <= 0)         //incrementa contador caso processo tenha duracao igual ou menor que 0
                count ++; 
        }if(count == num){              //testa se todos sao de duracao 0 ou menos
            break;
        }else{
            count = 0;
        }

        if(dur_aux[i] <= 0){            //caso tenha duracao 0 o tempo fica 0 para nao entrar no calculo
            t_aux[i] = 0;
            i++;    
        }else if(!first && time == time_a[i]){        //primeiro processo
            menor = t_aux[i];
            index = i;

            for(int j = 0; j < num; j++){
                if(t_aux[j] <= menor && dur_aux[j] <= dur_aux[i] && dur_aux[j] > 0){     //pega o processo de menor duracao e tempo de chegada
                    menor = t_aux[j];
                    index = j;
                }
            }
            t_retorno += dur_aux[index];
            time += t_retorno;
            t_aux[index] = 0;
            dur_aux[index] = 0;
            first = 1;
            InsertionSort(num, dur_aux, t_aux);                 //ordena por duracao em ordem crescente apos ja ter executado o primeiro
            
        }else if(first){                                        //segundo ou + processos
            for(int z = 1; z < num; z++){
                
                if(time >= t_aux[z] && dur_aux[z] > 0){
                    t_resposta += time - t_aux[z];
                    time += dur_aux[z];
                    t_retorno += time - t_aux[z];
                    dur_aux[z] = 0;
                    t_aux[z] = 0;
                    count2 = 0;
                    break;
                }else{
                    count2++;
                }if(count2 == num){
                    time++;
                    count2 = 0;
                }
            }
        }else{
            time++;
        }
            
    }
        
    if(cancel == num){                              //caso todos os programas tenham duracao 0
        t_retorno = 0;
        t_espera = 0;
        t_resposta = 0;
    }else{                                          //elimina os de duracao 0 do calculo
        num -= cancel;
    }
    t_retorno = t_retorno/num;
    t_resposta = t_resposta/num;
    t_espera = t_resposta;

    printf("SJF %.1f %.1f %.1f\n", t_retorno, t_resposta, t_espera);
    
}

void RR (int num, int *time_a, int *duration)
{
    float t_retorno = 0, t_resposta = 0, t_espera = 0;
    int total = 0, quantum = 2, time = 0, i = -1, total_quantum = 0, flag = 0, flag2 = 0;
    int dur_aux[num], resp[num], fim_aux[num], fim = 0, count = 0, prontos = 0, cancel = 0, no_duration = 0;
    struct Queue fila;

    criarQueue (&fila, num);            //cria a fila

    for(int x = 0; x < num; x++){       //cria copia dos arrays 
        dur_aux[x] = duration[x];
        total += dur_aux[x];
        resp[x] = -1;
        fim_aux[x] = 0;
        if(dur_aux[x] <= 0){                                        //variavel de cancelamento de processos de duracao 0 incrementa
            cancel++;
        }
    }
    
    while(prontos < num){                                           //laco principal
        while(1){
            for(int y = 0; y < num; y++){                           //verifica se algum processo chegou pra entrar na fila
                if(dur_aux[y] > 0 && time == time_a[y]){
                    inserir(&fila, y);
                    fim_aux[y] = time_a[y];
                    count++;         
                }else if(dur_aux[y] <= 0 && time_a[y] == time){    //anula os de duracao 0
                    no_duration++;
                    prontos++;
                }
            }if(cancel == num || no_duration){
                    break;
            }if(!count && !fila.nItens && !flag && !flag2 && prontos < num - 1){        //se nao tem itens na fila ou pendentes de termino, incrementa o tempo
                time++;
            }else{
                count = 0;
                break;
            }
        }
        if(flag == quantum){                            //caso o processo completou o quantum e ainda possui duracao, retorna pra fila
            inserir(&fila, i);
            flag = 0;
            i = -1;
        }
        

        if((time == total_quantum && !flag && !flag2) || fim || i == -1){       //remove o item que vai ser executado da fila
            if(fila.nItens){
                i = remover(&fila);
            }else{
                i = -1;
            }
            fim = 0;
        }    
        
        if(i != -1){
            if(resp[i] == -1){                                                  //tempos de resposta de cada processo
                resp[i] = time - time_a[i];
                t_resposta += resp[i];
            }

            if(dur_aux[i] > quantum || flag == 1){                              //caso o processo tenha duracao maior que o quantum entra nessa condicao
                flag++;

            }else if(dur_aux[i] == quantum){                                    //caso a duracao seja igual ao quantum
                flag = 0;
                flag2 = 1; 
                

            }else if(dur_aux[i] < quantum && dur_aux[i] > 0){                   //caso a duracao seja menor que o quantum
                flag = 0;
                flag2 = 0;
                fim++;
                prontos++;  
                
            }
            
            t_espera += time - fim_aux[i];                                     //incrementa os dados totais da funcao
            dur_aux[i]--;
            time++;
            fim_aux[i] = time;

            if(fim){                                                            //caso o processo termine, incrementa o tempo de retorno
                t_retorno += time - time_a[i];
            }

            if(time == (total_quantum + quantum)){
                total_quantum += quantum;
            }
        }else{
            time++;
        }
        if(cancel == num){                                                      //caso de quebra se todos os programas tenham duracao 0
            break;
        }
    }
    
    if(cancel == num){                                                          //caso todos os programas tenham duracao 0
        t_retorno = 0;
        t_espera = 0;
        t_resposta = 0;
    }else if(cancel > 0 && cancel < num){                                       //tira os de duracao 0 do calculo
        num -= cancel;
    }
    
    t_retorno = t_retorno/num;
    t_resposta = t_resposta/num;
    t_espera = t_espera/num;

    printf("RR %.1f %.1f %.1f", t_retorno, t_resposta, t_espera);
}

int main(void)
{
    FILE *fp;
    char *txt = NULL, *tmp = NULL, *num;
    size_t tam = 0;
    int i = 0;

    fp = fopen("entrada.txt", "r");                      //abre o arquivo
    
    if(fp == NULL){                                      //casos de erro ao abrir o arquivo
        printf("Erro, nao foi possivel abrir o arquivo\n");
        return 1;
    }

    while ((getline(&txt, &tam, fp)) != -1){
        i++;                                              //numero de linhas
    }

    rewind(fp);                                           //volta pro inicio do txt

    int time_a[i], duration[i];                           //tempo de chegada e duracao
    i = 0;                                                //processo atual

    while ((getline(&txt, &tam, fp)) != -1){              //laco de leitura do arquivo
        num = strtok (txt, " "); 
        time_a[i] = atoi(num);
        num = strtok(NULL, " ");
        duration[i] = atoi(num);
        
        if(duration[i] == NULL){
            duration[i] = 0;
        }
        i++;
    }

    InsertionSort(i, time_a, duration);             //ordena todos os processos por ordem de chegada
    FCFS(i, time_a, duration);                      //chama o FCFS
    SJF(i, time_a, duration);                       //chama o SJF
    RR(i, time_a, duration);                        //chama o RR

    fclose(fp);                                     //fecha o arquivo

    return 0;
}