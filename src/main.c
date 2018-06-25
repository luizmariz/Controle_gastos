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
}entrada;

typedef struct{
    double um;
    double dois;
    double carteira;
    double met;
    double met_atual;
    double despesas;
    double status_met;
    double orcament;
    double status_orc;
    char obj_nome[30];
    double valor_poup;
    double tempo;

}saldo_contas;

saldo_contas saldo;

void atualizar_saldo(char *conta, double valor, int tipo,int reajuste){// reajuste? 1,0-nao 1,1-sim  receita? 1 despesa? 0
    
    if(reajuste == 0 && tipo == 1){
        if (strcmp(conta, "Conta1") == 0){
            saldo.um = saldo.um + valor;
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
}// essa funcao evita a repeticao dos if ao longo do programa

void update(int comando){ // 0 - ler, 1 - escrever

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
            update(0);
        }
        
    }

    if(comando == 1){

        p_bin = fopen("data/saldos/main.dat","wb");
        fwrite(&saldo, sizeof(saldo_contas), 1, p_bin);
        fclose(p_bin);
    }   
}

void valor_conta_selecionada(char *conta){
    
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

void update_labels(){
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
        saldo.status_met = 0;
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
    
    update(0); 

    GtkBuilder  *builder; 
    GtkWidget   *window; 

    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    

    //ponteiros para a GUI
    
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

    gtk_text_view_set_buffer (g_lbl_texto, g_buffer_texto);

   
    //set da data do calendario usando as funcoes da biblioteca time

    time_t tempo = time(NULL); 
    struct tm tm = *localtime(&tempo);

    int dia = tm.tm_mday, mes = tm.tm_mon, ano = tm.tm_year +1900;

    gtk_calendar_select_month (g_cal_calendario, mes, ano);
    gtk_calendar_select_day (g_cal_calendario, dia);

    //

    update_labels();

    g_object_unref(builder);

    gtk_widget_show(window);  

    gtk_main();

    return 0;
}

/* a funcao sprintf funciona de forma analoga a printf com a diferenca de que a sua resultante
sera colocada em uma string - sprintf(str, "", variavel) */

// encerrar programa  -- gtk_main_quit() - encerra a interface grafica
// pegar valor        -- gtk gtk_spin_button_get_value(GTK_SPIN_BUTTON(ponteiro)) - retorna valor
// pegar texto        -- gtk_entry_get_text(GTK_ENTRY(ponteiro))) - retorna ponteiro para o texto
// selecionar a conta -- gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(ponteiro)) - retorna a combobox selecionada
// controle progresso -- gtk_progress_bar_set_fraction(ponteiro, fracao double de 0.0 a 1.0) - retorna a apresentacao da barra
// pegar data         -- gtk_calendar_get_date (ponteiro, ponteiro ano, ponteiro mes, ponteiro dia);


void pegar_dia(char *da){
    
    unsigned int d,m,a;
    unsigned int *dia, *ano, *mes;

    dia = &d;
    mes = &m;
    ano = &a;

    gtk_calendar_get_date (g_cal_calendario, ano, mes, dia);

    sprintf(da,"%d-%d-%d",d,m,a);

}


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

    saldo.tempo = (360*saldo.met)/(200000 + saldo.met);
    
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

    char conteudo[100000];
    
    char *p_conteudo = conteudo;

    char nome_arquivo_dat[25] = "data/historico/";
    char data[12];

    pegar_dia(data);
    strcat(nome_arquivo_dat, data);

    FILE *p_historico = fopen(nome_arquivo_dat, "ab+");

    while(!feof(p_historico)){
        char texto[400];
        fread(&texto, sizeof(char),400,p_historico);
        strcat(p_conteudo,texto);
    }
    
    fclose(p_historico);
    
    gtk_text_buffer_set_text (g_buffer_texto, p_conteudo, -1);

    gtk_widget_show_all(g_win_pop);


}

void on_reajuste_clicked(){

    entrada reajuste;

    strcpy(reajuste.conta , gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_comb_contas)));

    char nome_arquivo_txt[25] = "data/historico/";
    char data[12];

    pegar_dia(data);
    strcat(nome_arquivo_txt, data);

    FILE *p_historico = fopen(nome_arquivo_txt, "a");

    reajuste.valor = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor2));
    strcpy(reajuste.descricao , gtk_entry_get_text(GTK_ENTRY(g_etr_entrada2)));

    fprintf(p_historico,"\n%s: Reajustado para: R$%.2f %s", reajuste.conta, reajuste.valor, reajuste.descricao);

    fclose(p_historico);

    atualizar_saldo(reajuste.conta, reajuste.valor,1,1);
    update_labels();
}

void on_Contas_move_active(){
   
    char conta_selecionada[15];

    strcpy(conta_selecionada , gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_comb_contas)));
    valor_conta_selecionada(conta_selecionada);
}

void on_Novo_mes_clicked(){

    saldo.despesas = 0;
    saldo.orcament = 0;
    update_labels();
}

void on_window_main_destroy(){
    
    update(1);
    gtk_main_quit();

}