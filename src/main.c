// as janelas e a interface estao em um arquivo.glade que esta em XML
// para cada objeto e acao da interface foi associado um ID, assim, a partir deles serao
// executadas as suas respectivas funcoes.
// a funcao main designa apenas a execucao principal da interface, assim, de acordo com a documentacao
// o funcionamento eh semelhante a um callback em que a interacao do usuario funciona como gatilho

#include <gtk/gtk.h>
#include <glib.h>
 
GtkWidget *g_lbl_receita;

int main(int argc, char *argv[]) 
{
    GtkBuilder  *builder; 
    GtkWidget   *window; 
 

    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    
    
    g_lbl_receita = GTK_WIDGET(gtk_builder_get_object(builder, "saldo_em_contas"));
    

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
    static unsigned int count = 0;
    char str_count[30] = {0};
    count++;
    sprintf(str_count, "R$ %d", count);
    gtk_label_set_text(GTK_LABEL(g_lbl_receita), str_count);
}


void on_Nova_meta_clicked()
{
    static unsigned int count = 0;
    char str_count[30] = {0};
    count++;
    sprintf(str_count, "R$ %d", count);
    gtk_label_set_text(GTK_LABEL(g_lbl_receita), str_count);
}


void on_Novo_orcamento_clicked()
{
    static unsigned int count = 0;
    char str_count[30] = {0};
    count++;
    sprintf(str_count, "R$ %d", count);
    gtk_label_set_text(GTK_LABEL(g_lbl_receita), str_count);
}


void on_Nova_despesa_clicked()
{
    static unsigned int count = 0;
    char str_count[30] = {0};
    count++;
    sprintf(str_count, "R$ %d", count);
    gtk_label_set_text(GTK_LABEL(g_lbl_receita), str_count);
}
 

void on_window_main_destroy()
{
    gtk_main_quit();
}