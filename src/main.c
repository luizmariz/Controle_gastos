// as janelas e a interface estao em um arquivo.glade que esta em XML

/* para cada objeto e acao da interface foi associado um ID, assim, a partir deles serao
executadas as suas respectivas funcoes*/

/* a funcao main designa apenas a execucao principal da interface, assim, de acordo com a documentacao
o funcionamento eh semelhante a um callback em que a interacao do usuario funciona como gatilho*/

// a biblioteca time.h sera usada para manipular o horario de sistema e setar o calendario da interface

#include <stdio.h>
#include <gtk/gtk.h>
#include <time.h>
#include <string.h>


// declaracao dos ponteiros para as entidades da interface, os tipos sao conforme documentacao

GtkWidget *g_lbl_receita;
GtkWidget *g_lbl_orcamento;
GtkWidget *g_lbl_extra;
GtkWidget *g_lbl_valor_meta;
GtkWidget *g_lbl_poupar_tempo;
GtkWidget *g_lbl_objetivo;

GtkWidget *g_spbtn_valor1;
GtkWidget *g_spbtn_valor2;

GtkWidget *g_etr_entrada1;
GtkWidget *g_etr_entrada2;

GtkWidget *g_comb_contas;

GtkProgressBar *g_bar_meta;
GtkProgressBar *g_bar_orcamento;

GtkCalendar *g_cal_calendario;

GtkWidget *g_win_pop;
    
GtkTextView *g_lbl_texto;
GtkTextBuffer *g_buffer_texto;

GtkWidget *g_scroll_bar;

typedef struct{
    char descricao[40];
    char conta[20];
    double valor; 
}entrada; // tipo entrada definido para usar na manipulacao dos dados de despesa e receita

typedef struct{
    // saldos em contas
    double um;
    double dois;
    double carteira;
    double met_atual;
    // def meta
    char obj_nome[30];
    double met;
    double tempo;
    double status_met;
    double valor_poup;
    // def orcamento
    double orcament;
    double status_orc;
    // despesa total mensal
    double despesas;

}saldo_contas; // tipo principal, struct que contem todas as informacoes principais do programa

saldo_contas saldo;

void atualizar_saldo(char *conta, double valor, int tipo,int reajuste){ // reajuste? 1,1 | receita? 1,0 | despesa? 0, 0
    
    if(reajuste == 0 && tipo == 1){           // essa funcao evita a repeticao dos ifs ao longo 
        if (strcmp(conta, "Conta1") == 0){    // do programa quanto a reajustar o saldo conforme      
            saldo.um = saldo.um + valor;      // a conta selecionada
        }

        if (strcmp(conta, "Conta2") == 0){
            saldo.dois = saldo.dois + valor;
        }

        if (strcmp(conta, "Carteira") == 0){
            saldo.carteira = saldo.carteira + valor;
        }

        if (strcmp(conta, "Meta") == 0){
            saldo.met_atual = saldo.met_atual + valor;
        }
    }

    if(reajuste == 0 && tipo == 0){
        if (strcmp(conta, "Conta1") == 0){
            saldo.um = saldo.um - valor;
        }

        if (strcmp(conta, "Conta2") == 0){
            saldo.dois = saldo.dois - valor;
        }

        if (strcmp(conta, "Carteira") == 0){
            saldo.carteira = saldo.carteira - valor;
        }

        if (strcmp(conta, "Meta") == 0){
            saldo.met_atual = saldo.met_atual - valor;
        }
    }

    if(reajuste == 1 && tipo == 1){
        if (strcmp(conta, "Conta1") == 0){
            saldo.um = valor;
        }

        if (strcmp(conta, "Conta2") == 0){
            saldo.dois = valor;
        }

        if (strcmp(conta, "Carteira") == 0){
            saldo.carteira = valor;
        }

        if (strcmp(conta, "Meta") == 0){
            saldo.met_atual = valor;
        }
    }
}


void update(int comando){ // ler 0 | escrever 1

    FILE *p_bin;

    if(comando == 0){

        p_bin = fopen("data/saldos/main.dat","ab+");

        if(fread(&saldo, sizeof(saldo_contas), 1, p_bin) == 0){
       
            saldo.um = 0;
            saldo.dois = 0;
            saldo.carteira = 0;
            saldo.met = 0;
            saldo.met_atual = 0;
            saldo.despesas = 0;
            saldo.status_met = 0;
            saldo.orcament = 0;
            saldo.status_orc = 0;
            saldo.met_atual = 0;
            strcpy(saldo.obj_nome, "Inicie uma meta");
            saldo.valor_poup = 0;
            saldo.tempo = 0;

            fwrite(&saldo, sizeof(saldo_contas), 1, p_bin);

            fclose(p_bin);

            FILE *p_teste = fopen("data/historico/teste","wb+");

            fclose(p_teste);

            update(0);
        }
    }

    if(comando == 1){

        p_bin = fopen("data/saldos/main.dat","wb");
        
        fwrite(&saldo, sizeof(saldo_contas), 1, p_bin);

        fclose(p_bin);

    }   
} /* a funcao acima atualiza os dados do arquivo, caso seja o primeiro uso, o arquivo main.dat nao existe, 
entao ele e criado junto com um arquivo de teste - obs.: todos os valores sao setados em nulo */

void valor_conta_selecionada(char *conta){  // funcao responsavel por mostrar na interface o saldo em conta da conta selecionada na janela de contas
    
    char str[100];                          

    if (strcmp(conta, "Conta1") == 0){
        sprintf(str,"Despesas: R$ %.2f\nConta 1: R$ %.2f", saldo.despesas,saldo.um);
        gtk_label_set_text(GTK_LABEL(g_lbl_extra),str);
    }

    if (strcmp(conta, "Conta2") == 0){
        sprintf(str,"Despesas: R$ %.2f\nConta 2: R$ %.2f", saldo.despesas,saldo.dois);
        gtk_label_set_text(GTK_LABEL(g_lbl_extra),str);
    }

    if (strcmp(conta, "Carteira") == 0){
        sprintf(str,"Despesas: R$ %.2f\nCarteira: R$ %.2f", saldo.despesas,saldo.carteira);
        gtk_label_set_text(GTK_LABEL(g_lbl_extra),str);
    }

    if (strcmp(conta, "Meta") == 0){
        sprintf(str,"Despesas: R$ %.2f\nMeta: R$ %.2f", saldo.despesas,saldo.met_atual);
        gtk_label_set_text(GTK_LABEL(g_lbl_extra),str);
    }
} 

void update_labels(){ // labels sao as caixas de texto da interface, a funcao atualiza seu conteudo conforme a variavel

    char str[100];

    sprintf(str,"R$ %.2f", saldo.um + saldo.dois + saldo.carteira);
    gtk_label_set_text(GTK_LABEL(g_lbl_receita),str);

    sprintf(str,"%s", saldo.obj_nome);
    gtk_label_set_text(GTK_LABEL(g_lbl_objetivo),str);

    sprintf(str,"Meta: R$ %.2f", saldo.met);
    gtk_label_set_text(GTK_LABEL(g_lbl_valor_meta),str);

    sprintf(str,"Recomendado: R$ %.2f por %.0f meses", saldo.valor_poup, saldo.tempo);
    gtk_label_set_text(GTK_LABEL(g_lbl_poupar_tempo),str);

    sprintf(str,"Orçamento: R$ %.2f", saldo.orcament);
    gtk_label_set_text(GTK_LABEL(g_lbl_orcamento),str);

    sprintf(str,"Despesas: R$ %.2f", saldo.despesas);
    gtk_label_set_text(GTK_LABEL(g_lbl_extra),str);

    if(saldo.met == 0){
        saldo.status_met = 0;    // nao queremos uma divisao por 0 ne? rs
    }
    else{
        saldo.status_met = ((100*saldo.met_atual)/(saldo.met))/100; 
    }

    if(saldo.orcament == 0){
        saldo.status_orc = 0;
    }
    else{
        saldo.status_orc = ((100*saldo.despesas)/(saldo.orcament))/100; 
    }

    gtk_progress_bar_set_fraction(g_bar_meta, saldo.status_met);
    gtk_progress_bar_set_fraction(g_bar_orcamento, saldo.status_orc);

    char conta_selecionada[15];
    strcpy(conta_selecionada , gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_comb_contas)));
    valor_conta_selecionada(conta_selecionada);
}



int main(int argc, char *argv[]){
    
    update(0); //primeiro checa o arquivo, cria se nao existir e atualiza os valores conforme memoria

    /*Declaramos entao os ponteiros que serao associados a janela principal e ao builder - funcao que executa
    a construcao da interface com base em um arquivo .glade que esta em xml*/

    GtkBuilder  *builder;  
    GtkWidget   *window; 

    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    

    /* os ponteros declarados agora apontam para a funcao que "associa" a id da entidade da interface 
    com o codigo em si | GTK_tipo(gtk_builder_get_object(builder,"id"));*/
    
    g_lbl_receita      = GTK_WIDGET(gtk_builder_get_object(builder,"saldo_em_contas"));
    g_lbl_orcamento    = GTK_WIDGET(gtk_builder_get_object(builder,"Orcamento_valor_mostruario"));
    g_lbl_extra        = GTK_WIDGET(gtk_builder_get_object(builder,"extra_display"));
    g_lbl_valor_meta   = GTK_WIDGET(gtk_builder_get_object(builder,"meta_valor"));
    g_lbl_poupar_tempo = GTK_WIDGET(gtk_builder_get_object(builder,"valor_tempo_meta"));
    g_lbl_objetivo     = GTK_WIDGET(gtk_builder_get_object(builder,"meta_nome"));
    
    g_spbtn_valor1     = GTK_WIDGET(gtk_builder_get_object(builder,"Valor_receita_despesa"));
    g_spbtn_valor2     = GTK_WIDGET(gtk_builder_get_object(builder,"Valor_meta_orcamento"));

    g_etr_entrada1     = GTK_WIDGET(gtk_builder_get_object(builder,"descricao_receita_despesa"));
    g_etr_entrada2     = GTK_WIDGET(gtk_builder_get_object(builder,"Descricao_meta_orcamento"));

    g_comb_contas      = GTK_WIDGET(gtk_builder_get_object(builder,"Contas"));

    g_bar_meta         = GTK_PROGRESS_BAR(gtk_builder_get_object(builder,"meta_barra_progresso"));
    g_bar_orcamento    = GTK_PROGRESS_BAR(gtk_builder_get_object(builder,"barra_progresso_orcamento"));

    g_cal_calendario   = GTK_CALENDAR(gtk_builder_get_object(builder,"calendario"));

    g_win_pop = GTK_WIDGET(gtk_builder_get_object(builder,"pop_historico"));

    g_lbl_texto        = GTK_TEXT_VIEW(gtk_builder_get_object(builder,"visualizador_texto"));
    g_buffer_texto     = GTK_TEXT_BUFFER(gtk_builder_get_object(builder,"textbuffer1"));

    g_scroll_bar       = GTK_WIDGET(gtk_builder_get_object(builder, "scroll_bar"));

    /* o text view eh apenas um label vazio, para poder imprimir/escrever um conteudo nele e preciso associa-lo a
    um buffer que no caso eh uma entidade de texto editavel */

    gtk_text_view_set_buffer (g_lbl_texto, g_buffer_texto);

   
    /* set da data do calendario usando as funcoes da biblioteca time e a funcao select do gtk que 
    seta como padrao a data informada, no caso a diaria*/

    time_t tempo = time(NULL); 
    struct tm tm = *localtime(&tempo); 

    int dia = tm.tm_mday, mes = tm.tm_mon, ano = tm.tm_year +1900;

    gtk_calendar_select_month (g_cal_calendario, mes, ano);
    gtk_calendar_select_day (g_cal_calendario, dia);

    /* bloco final da main, respectivamente seta as informacoes, 
    constroi e mostra a interface*/

    update_labels();

    g_object_unref(builder);

    gtk_widget_show(window);  

    /* possivelmente a funcao mais importante, coloca o programa em loop - callback | somente a funcao
    gtk_main_quit() encerra o loop*/

    gtk_main();

    return 0;
}

/* a funcao sprintf funciona de forma analoga a printf com a diferenca de que a sua resultante
sera colocada em um vetor char - sprintf(str, "", variavel). eh perfeita para alguns casos em que precisamos
formatar algo antes de salvar */

// encerrar programa  -- gtk_main_quit() - encerra a interface grafica
// pegar valor        -- gtk gtk_spin_button_get_value(GTK_SPIN_BUTTON(ponteiro)) - retorna valor
// pegar texto        -- gtk_entry_get_text(GTK_ENTRY(ponteiro))) - retorna ponteiro para o texto
// selecionar a conta -- gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(ponteiro)) - retorna a combobox selecionada
// controle progresso -- gtk_progress_bar_set_fraction(ponteiro, fracao double de 0.0 a 1.0) - retorna visualmente o progresso da barra
// pegar data         -- gtk_calendar_get_date (ponteiro, ponteiro ano, ponteiro mes, ponteiro dia)


void pegar_dia(char *da){
    
    unsigned int d,m,a;
    unsigned int *dia, *ano, *mes;

    dia = &d;
    mes = &m;
    ano = &a;

    gtk_calendar_get_date (g_cal_calendario, ano, mes, dia);

    sprintf(da,"%d-%d-%d",d,m,a);

} // essa funcao eh responsavel por pegar a data selecionada no calendario (n necessariamente a atual)


/* obs.: todas as funcoes a partir de agora sao "chamadas" conforme interacao do usuario com a interface, seus nomes
sao na verdade id's definidos a acoes na interface - os nomes ja sao bem sujestivos */

/* as funcoes a seguir, despesa e receita, sao bem semelhantes, ambas seguem o mesmo padrao: pegar as entradas
do usuario (pelas funcoes resumidamente explicadas acima), abrir um arquivo no diretorio data/historico/ cujo nome e 
a data selecionada e criar um historico binario que pode ser acessado ao usuario e atualizar os dados do programa*/

void on_Nova_receita_clicked(){ 
                
    entrada receita;            

    strcpy(receita.conta , gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_comb_contas)));

    char nome_arquivo_dat[25] = "data/historico/";
    char data[12];

    pegar_dia(data);
    strcat(nome_arquivo_dat, data);

    FILE *p_historico = fopen(nome_arquivo_dat, "ab");

    receita.valor = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor1));
    strcpy(receita.descricao , gtk_entry_get_text(GTK_ENTRY(g_etr_entrada1)));

    char texto[400];
    sprintf(texto,"\n%s | %s: +R$%.2f %s", data, receita.conta, receita.valor, receita.descricao);
    
    fwrite(&texto,sizeof(char),400,p_historico);

    fclose(p_historico);

    atualizar_saldo(receita.conta, receita.valor,1,0);
    update_labels();
    
}

void on_Nova_despesa_clicked(){
    
    entrada despesa;

    strcpy(despesa.conta , gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_comb_contas)));

    char nome_arquivo_dat[25] = "data/historico/";
    char data[12];

    pegar_dia(data);
    strcat(nome_arquivo_dat, data);

    FILE *p_historico = fopen(nome_arquivo_dat, "ab");

    despesa.valor = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor1));
    strcpy(despesa.descricao , gtk_entry_get_text(GTK_ENTRY(g_etr_entrada1)));

    char texto[400];
    sprintf(texto,"\n%s | %s: -R$%.2f %s", data, despesa.conta, despesa.valor, despesa.descricao);

    fwrite(&texto,sizeof(char),400,p_historico);

    fclose(p_historico);

    atualizar_saldo(despesa.conta, despesa.valor,0,0);
    saldo.despesas = saldo.despesas + despesa.valor;
    update_labels();

}
 
void on_Nova_meta_clicked(){

    saldo.met = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor2));
    strcpy(saldo.obj_nome , gtk_entry_get_text(GTK_ENTRY(g_etr_entrada2)));

    saldo.tempo = (360*saldo.met)/(200000 + saldo.met); /* essa funcao retorna um tempo "proporcional" ao valor da meta
    pretendida, foi usado o geogebra para desenhar o grafico, detalhe: existe uma assintota horizontal em 30 anos
    ou seja, o teto do tempo recomendado a uma meta eh de 30 anos, e diminui conforme o valor*/

    if(saldo.tempo <= 1){
        saldo.tempo = 1;
    }
   
    saldo.valor_poup = saldo.met/saldo.tempo;

    update_labels();
}

void on_Novo_orcamento_clicked(){
    
    saldo.orcament = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor2));  
    update_labels();
}

void on_calendario_day_selected(){  

    char conteudo[1000000];

    int i;
    for(i=0;i<1000000;i++){
        conteudo[i] = NULL;
    }
    
    char *p_conteudo = &conteudo[0];

    char nome_arquivo_dat[25] = "data/historico/";
    char data[12];

    pegar_dia(data);
    strcat(nome_arquivo_dat, data);

    FILE *p_historico = fopen(nome_arquivo_dat, "ab+");

    char texto[400];

    while(fread(&texto, sizeof(char),400,p_historico) != 0){ //enquanto tiver algo para ler
        strcat(conteudo,texto);
    }
    
    fclose(p_historico);
    
    gtk_text_buffer_set_text (g_buffer_texto, p_conteudo,-1);

    gtk_widget_show_all(g_win_pop);


}   /* gtk_text_buffer_set_text(ponteiro para o buffer, texto, -1) - seta o buffer com um texto que definimos no array 
gtk_widget_show_all(ponteiro para o widget) - mostra uma janela "popup" e seu conteudo, no caso o texto | essa funcao
retorna um log de historico, assim, quando o usuario clica 2x sobre uma data, ela adiciona o conteudo do arquivo_historico
referente aquela data ao log e o mostra em uma janela popup obs.: o log e acumulativo, entao no caso ele sempre vai
printar o historico da nova data abaixo da data previamente selecionada, permitindo o usuario comparar por exemplo */

void on_reajuste_clicked(){ // caso o usuario deseje, ele pode ajustar manualmente os saldos de cada conta, sem precisar executa uma serie de receitas ou despesas

    entrada reajuste;

    strcpy(reajuste.conta , gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_comb_contas)));

    char nome_arquivo_txt[25] = "data/historico/";
    char data[12];

    pegar_dia(data);
    strcat(nome_arquivo_txt, data);

    FILE *p_historico = fopen(nome_arquivo_txt, "a");

    reajuste.valor = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor2));
    strcpy(reajuste.descricao , gtk_entry_get_text(GTK_ENTRY(g_etr_entrada2)));

    fprintf(p_historico,"\n%s | %s: Reajustado para: R$%.2f %s", data, reajuste.conta, reajuste.valor, reajuste.descricao);

    fclose(p_historico);

    atualizar_saldo(reajuste.conta, reajuste.valor,1,1);
    update_labels();
}

void on_Contas_move_active(){ // muda o "mostruario" do saldo em conta da interface conforme conta selecionada
   
    char conta_selecionada[15];

    strcpy(conta_selecionada , gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_comb_contas)));
    valor_conta_selecionada(conta_selecionada);
}

void on_Novo_mes_clicked(){ // Reinicia o orcamento e as despesas do mes

    saldo.despesas = 0;
    saldo.orcament = 0;
    update_labels();
}

void on_window_main_destroy(){ // quando a janela e fechada, salva as informacoes e encerra o loop da main
    
    update(1);
    gtk_main_quit();

}