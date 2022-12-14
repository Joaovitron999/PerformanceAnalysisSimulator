//João Vitor Fonseca | 2020.1.08.003
//Análise de Desempenho Trabalho 1
// Novembro /2022
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

typedef struct little_{
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

double maximo(double num1, double num2){
    if(num1 > num2){
        return num1;
    }
    return num2;
}

typedef struct AnaliseIntervalo{
  int index;
  double eN;
  double eW;
  double lambda;
  double erroL;
  double ocupacao;
} AnaliseIntervalo;

//Funcao criada para o trabalho com base no código do professor
AnaliseIntervalo CalcL( little e_n, little e_w_chegada, little e_w_saida, int tempo, double soma_tempo_servico, int index){
  double tempo_decorrido = (double)tempo;
  //little
  e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior)*e_n.no_eventos;
  e_n.tempo_anterior = tempo_decorrido;
  e_n.no_eventos--;

  e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior)*e_w_saida.no_eventos;
  e_w_saida.tempo_anterior = tempo_decorrido;
  e_w_saida.no_eventos++;

  e_w_chegada.soma_areas +=
  (tempo_decorrido - e_w_chegada.tempo_anterior)*e_w_chegada.no_eventos;
  
  e_w_saida.soma_areas +=
  (tempo_decorrido - e_w_saida.tempo_anterior)*e_w_saida.no_eventos;

  double e_n_final = e_n.soma_areas / tempo_decorrido;
  double e_w_final =
  (e_w_chegada.soma_areas - e_w_saida.soma_areas)/e_w_chegada.no_eventos;
  double lambda = e_w_chegada.no_eventos / tempo_decorrido;


  printf("E[N]: %lF\n", e_n_final);
  printf("E[W]: %lF\n", e_w_final);
  printf("lambda: %lF\n\n", lambda);

  printf("Erro de Little: %.20lF\n\n", e_n_final - lambda * e_w_final);

  printf("Ocupacao: %lF.\n", soma_tempo_servico/tempo_decorrido);

    AnaliseIntervalo analise;
    analise.index = index;
    analise.eN = e_n_final;
    analise.eW = e_w_final;
    analise.lambda = lambda;
    analise.erroL = e_n_final - lambda * e_w_final;
    analise.ocupacao = soma_tempo_servico/tempo_decorrido;
    return analise;
}


double aleatorio() {
	double u = rand() / ((double) RAND_MAX + 1);
	//limitando entre (0,1]
	u = 1.0 - u;

	return (u);
}

double minimo(double num1, double num2){
    if(num1 < num2){
        return num1;
    }
    return num2;
}



void inicia_little(little * l){
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

int main(){
    //Vetor de análise de intervalos
    AnaliseIntervalo analises[1000];


    double tempo_simulacao;
    double tempo_decorrido = 0.0;

    double intervalo_medio_chegada;
    double tempo_medio_servico;

    double chegada;
    double servico;

    double soma_tempo_servico = 0.0;

    unsigned long int fila = 0;
    unsigned long int max_fila = 0;

    /**
    Little
    */
    little e_n;
    little e_w_chegada;
    little e_w_saida;

    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);
    /**
    Little -- fim
    */

    //srand(time(NULL));
    srand(10000); // Seed fixada para a correção do trabalho

/*
    Dados fixados com base na instrução do trabalho
    "Os seguintes parâmetros devem ser fixados na simulação:
    • Taxa média de chegada = 5 / segundo (intervalo médio entre chegadas igual a 0,2 segundos).
    • Tempo de simulação = 36.000 segundos (10 horas)."
*/
    //printf("Informe o tempo de simulacao (segundos): ");
    //scanf("%lF",&tempo_simulacao);
    tempo_simulacao = 36000; // 10 horas
    //printf("Informe o intervalo medio entre chegadas (segundos): ");
    //scanf("%lF",&intervalo_medio_chegada);
    intervalo_medio_chegada = 0.2; // 5 chegadas por segundo em média

    printf("Informe o tempo medio de servico (segundos): ");
    scanf("%lF",&tempo_medio_servico);

    //gerando o tempo de chegada da primeira requisicao.
    chegada = (-1.0 / (1.0/intervalo_medio_chegada)) * log(aleatorio());
    //double timer = 0;
    //timer+=chegada;
    
    int index = 0; // index para cada intervalo de 100 segundos
    double divisorDeIntervalo = 100.0; // divisor para cada intervalo de 100 segundos
    double tempo = 0; // variável que permite a divisão dos intervalos
    while(tempo_decorrido <= tempo_simulacao){
      
        tempo_decorrido = !fila ? chegada : minimo(chegada, servico);
        if(tempo_decorrido>tempo){
          if(index>=1){ // para não calcular o primeiro intervalo
            analises[index-1] = CalcL( e_n,  e_w_chegada,  e_w_saida,  tempo,  soma_tempo_servico, index);            
        } 
          tempo+=100;
          index++;
        }

        //chegada
        if(tempo_decorrido == chegada){
            //printf("Chegada em %lF.\n", tempo_decorrido);
            if(!fila){
                servico = tempo_decorrido + (-1.0 / (1.0/tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
            fila++;
            max_fila = fila > max_fila? fila: max_fila;

            chegada = tempo_decorrido + (-1.0 / (1.0/intervalo_medio_chegada)) * log(aleatorio());

            //little
            e_n.soma_areas += 
            (tempo_decorrido - e_n.tempo_anterior)*e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas +=
            (tempo_decorrido - e_w_chegada.tempo_anterior)*e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;

        }else{//saida
            //printf("Saida em %lF.\n", tempo_decorrido);
            fila--;

            if(fila){
                servico = tempo_decorrido + (-1.0 / (1.0/tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }

            //little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior)*e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos--;

            e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior)*e_w_saida.no_eventos;
            e_w_saida.tempo_anterior = tempo_decorrido;
            e_w_saida.no_eventos++;
        }
    }
    e_w_chegada.soma_areas +=
    (tempo_decorrido - e_w_chegada.tempo_anterior)*e_w_chegada.no_eventos;
    
    e_w_saida.soma_areas +=
    (tempo_decorrido - e_w_saida.tempo_anterior)*e_w_saida.no_eventos;

    double e_n_final = e_n.soma_areas / tempo_decorrido;
    double e_w_final =
    (e_w_chegada.soma_areas - e_w_saida.soma_areas)/e_w_chegada.no_eventos;
    double lambda = e_w_chegada.no_eventos / tempo_decorrido;

    printf("E[N]: %lF\n", e_n_final);
    printf("E[W]: %lF\n", e_w_final);
    printf("lambda: %lF\n\n", lambda);

    printf("Erro de Little: %.20lF\n\n", e_n_final - lambda * e_w_final);

    printf("Ocupacao: %lF.\n", soma_tempo_servico/maximo(tempo_decorrido, servico));
    printf("Max fila: %ld.\n", max_fila);


    //Exibindo os resultados
    for(int i=1; i<index; i++){
      printf("Intervalo %d: \n", i);
      printf("E[N]: %lF\n", analises[i-1].eN);
      printf("E[W]: %lF\n", analises[i-1].eW);
      printf("lambda: %lF\n\n", analises[i-1].lambda);
      printf("Erro de Little: %.20lF\n\n", fabs(analises[i-1].eN - analises[i-1].lambda * analises[i-1].eW));
    }

    return 0;
}


//Compilar
//gcc main.c main -lm 