#include <gtk/gtk.h>
#include <soma/soma-panel.h>

int main (int argc, char *argv[])
{
  GtkWidget *panel;

  gtk_init (&argc, &argv);

  panel = soma_panel_new ();
  g_signal_connect (panel, "destroy", gtk_main_quit, NULL);

  gtk_widget_show_all (panel);

  gtk_main ();

  return 0;
}
