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

GtkWidget *g_bar_meta;
GtkWidget *g_bar_orcamento;

GtkCalendar *g_cal_calendario;

typedef struct{
    char descricao[40];
    char conta[20];
    double valor; 
}entrada;

typedef struct{
    char descricao[40];
    double valor;
    double progresso;
}desejo;

typedef struct{
    double um;
    double dois;
    double carteira;
    double met;
    double despesas;
    double status_met;
    double orcament;
    double status_orc;

}saldo_contas;

saldo_contas saldo;

void atualizar_saldo(char *conta, double valor){
    
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
        saldo.met = saldo.met + valor;
    }
    
}// essa funcao evita a repeticao dos if ao longo do programa

void update(int comando){ // 0 - ler, 1 - escrever

    FILE *p_bin = fopen("main.dat","ab+");

    if(comando == 0){
        if(fread(&saldo, sizeof(saldo_contas), 1, p_bin) == 0){
       
            saldo.um = 0;
            saldo.dois = 0;
            saldo.carteira = 0;
            saldo.met = 0;
            saldo.despesas = 0;
            saldo.status_met = 0;
            saldo.orcament = 0;
            saldo.status_orc = 0;

            fwrite(&saldo, sizeof(saldo_contas), 1, p_bin);
        }
    }

    if(comando == 1){
        fwrite(&saldo, sizeof(saldo_contas), 1, p_bin);
    }

    fclose(p_bin);
}

int main(int argc, char *argv[]){
    
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
    g_lbl_poupar_tempo = GTK_WIDGET(gtk_builder_get_object(builder,"valor_meta_tempo"));
    g_lbl_objetivo     = GTK_WIDGET(gtk_builder_get_object(builder,"meta_nome"));
    
    g_spbtn_valor1     = GTK_WIDGET(gtk_builder_get_object(builder,"Valor_receita_despesa"));
    g_spbtn_valor2     = GTK_WIDGET(gtk_builder_get_object(builder,"Valor_meta_orcamento"));

    g_etr_entrada1     = GTK_WIDGET(gtk_builder_get_object(builder,"descricao_receita_despesa"));
    g_etr_entrada2     = GTK_WIDGET(gtk_builder_get_object(builder,"Descricao_meta_orcamento"));

    g_comb_contas      = GTK_WIDGET(gtk_builder_get_object(builder,"Contas"));

    g_bar_meta         = GTK_WIDGET(gtk_builder_get_object(builder,"meta_barra_progresso"));
    g_bar_orcamento    = GTK_WIDGET(gtk_builder_get_object(builder,"barra_progresso_orcamento"));

    g_cal_calendario   = GTK_CALENDAR(gtk_builder_get_object(builder,"calendario"));

    //set da data do calendario usando as funcoes da biblioteca time

    time_t tempo = time(NULL); 
    struct tm tm = *localtime(&tempo);

    int dia = tm.tm_mday, mes = tm.tm_mon, ano = tm.tm_year +1900;

    gtk_calendar_select_month (g_cal_calendario, mes, ano);
    gtk_calendar_select_day (g_cal_calendario, dia);

    //

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

    char nome_arquivo_txt[25] = "data/historico/";
    char data[12];

    pegar_dia(data);
    strcat(nome_arquivo_txt, data);

    FILE *p_historico = fopen(nome_arquivo_txt, "a");

    receita.valor = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor1));
    strcpy(receita.descricao , gtk_entry_get_text(GTK_ENTRY(g_etr_entrada1)));

    fprintf(p_historico,"\n%s: +R$%.2f %s", receita.conta, receita.valor, receita.descricao);

    fclose(p_historico);

    atualizar_saldo(receita.conta, receita.valor);
    
}

void on_Nova_despesa_clicked(){
    
    entrada despesa;

    strcpy(despesa.conta , gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(g_comb_contas)));

    char nome_arquivo_txt[25] = "data/historico/";
    char data[12];

    pegar_dia(data);
    strcat(nome_arquivo_txt, data);

    FILE *p_historico = fopen(nome_arquivo_txt, "a");

    despesa.valor = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor1));
    strcpy(despesa.descricao , gtk_entry_get_text(GTK_ENTRY(g_etr_entrada1)));

    fprintf(p_historico,"\n%s: -R$%.2f %s", despesa.conta, despesa.valor, despesa.descricao);

    fclose(p_historico);

    atualizar_saldo(despesa.conta, despesa.valor);
    saldo.despesas = saldo.despesas + despesa.valor;

}
 
void on_Nova_meta_clicked(){
    double quantidade = 0; 
    quantidade = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor2));
}

void on_Novo_orcamento_clicked(){
    double quantidade = 0; 
    quantidade = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor2));   
}

void on_calendario_day_selected(){

}

void on_reajuste_clicked(){

}

void on_window_main_destroy(){
    gtk_main_quit();
}