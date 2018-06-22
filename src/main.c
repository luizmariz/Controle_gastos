// as janelas e a interface estao em um arquivo.glade que esta em XML
// para cada objeto e acao da interface foi associado um ID, assim, a partir deles serao
// executadas as suas respectivas funcoes.
// a funcao main designa apenas a execucao principal da interface, assim, de acordo com a documentacao
// o funcionamento eh semelhante a um callback em que a interacao do usuario funciona como gatilho

#include <stdio.h>
#include <gtk/gtk.h>
 
GtkWidget *g_lbl_receita;
GtkWidget *g_lbl_orcamento;
GtkWidget *g_lbl_extra;
GtkWidget *g_lbl_valor_meta;
GtkWidget *g_lbl_poupar_tempo;
GtkWidget *g_lbl_objetivo;

GtkWidget *g_spbtn_valor1;
GtkWidget *g_spbtn_valor2;

GtkWidget  *g_etr_entrada1;
GtkWidget  *g_etr_entrada2;


FILE *p_main_file;


int main(int argc, char *argv[]) 
{
    GtkBuilder  *builder; 
    GtkWidget   *window; 

    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    
    //

    p_main_file = fopen("data/main.txt","r+");
    
    g_lbl_receita = GTK_WIDGET(gtk_builder_get_object(builder, "saldo_em_contas"));
    g_lbl_orcamento = GTK_WIDGET(gtk_builder_get_object(builder, "Orcamento_valor_mostruario"));
    g_lbl_extra = GTK_WIDGET(gtk_builder_get_object(builder,"extra_display"));
    g_lbl_valor_meta = GTK_WIDGET(gtk_builder_get_object(builder,"meta_valor"));
    g_lbl_poupar_tempo = GTK_WIDGET(gtk_builder_get_object(builder,"valor_meta_tempo"));
    g_lbl_objetivo = GTK_WIDGET(gtk_builder_get_object(builder,"meta_nome"));
    
    g_spbtn_valor1 = GTK_WIDGET(gtk_builder_get_object(builder,"Valor_receita_despesa"));
    g_spbtn_valor2 = GTK_WIDGET(gtk_builder_get_object(builder,"Valor_meta_orcamento"));

    g_etr_entrada1 = GTK_WIDGET(gtk_builder_get_object(builder,"descricao_receita_despesa"));
    g_etr_entrada2 = GTK_WIDGET(gtk_builder_get_object(builder,"Descricao_meta_orcamento"));

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();


    return 0;
}
 
// funcoes - a destroy eh chamda qndo a janela e fechada

// a funcao sprintf funciona de forma analoga a printf com a diferenca de que a sua resultante
// sera colocada em uma string

void on_Nova_receita_clicked()
{
    //double quantidade = 0; 
    //char saida_str[100] = {0}; 
    
    
    //g_print("%s\n",gtk_entry_get_text(g_etr_entrada1));
 
   
    //quantidade = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor1));
    //sprintf(saida_str,"%s", descricao_p);
    //gtk_label_set_text(GTK_LABEL(g_lbl_receita), saida_str);
}
 
void on_Nova_meta_clicked()
{
    //double quantidade = 0; 
    //char saida_str[100] = {0}; 
 
    //quantidade = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor2));
    //sprintf(saida_str,"%.2lf", quantidade);
    //gtk_label_set_text(GTK_LABEL(g_lbl_receita), saida_str);
}

void on_Novo_orcamento_clicked()
{
    //double quantidade = 0; 
    //char saida_str[100] = {0}; 
 
    //quantidade = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor2));
    //sprintf(saida_str,"%.2lf", quantidade);
    //gtk_label_set_text(GTK_LABEL(g_lbl_receita), saida_str);
}


void on_Nova_despesa_clicked()
{
    //double quantidade = 0; 
    //char saida_str[100] = {0}; 
 
    //quantidade = gtk_spin_button_get_value(GTK_SPIN_BUTTON(g_spbtn_valor1));
    //sprintf(saida_str,"%.2lf", quantidade);
    //gtk_label_set_text(GTK_LABEL(g_lbl_receita), saida_str);
}
 
void on_Update_clicked()
{

}

void on_window_main_destroy()
{
    gtk_main_quit();
}