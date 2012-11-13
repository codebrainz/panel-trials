/*
 * soma-panel.c - The main window/panel widget
 *
 * Copyright (c) 2012 Matthew Brush <mbrush@codebrainz.ca>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <soma/soma-panel.h>
#include <soma/soma-panel-css.h>
#include <soma/soma-panel-ui.h>

struct SomaPanelPrivate_
{
  gboolean supports_alpha;
};

G_DEFINE_TYPE (SomaPanel, soma_panel, GTK_TYPE_WINDOW)


/* I couldn't figure out how to style widgets named through Glade/GtkBuilder
 * so this function just takes the buildable name and sets the widget's name
 * to that.
 * TODO: learn CSS (selectors specifically) */
static void
soma_panel_names_from_buildable (GtkWidget *widget)
{
  const gchar *name;

  if (!GTK_IS_WIDGET (widget))
    return;

  if (GTK_IS_BUILDABLE (widget))
    {
      name = gtk_buildable_get_name (GTK_BUILDABLE (widget));
      if (name != NULL)
        gtk_widget_set_name (widget, name);
    }

  if (GTK_IS_CONTAINER (widget))
    {
      GList *children, *iter;
      children = gtk_container_get_children (GTK_CONTAINER (widget));
      for (iter = children; iter != NULL; iter = g_list_next (iter))
        {
          GtkWidget *child = GTK_WIDGET (iter->data);
          soma_panel_names_from_buildable (child);
        }
    }
}

/* Once all the X resources are ready, tell the WM to reserve some space at
 * the edge of the screen for the panel, so windows can get above/below it. */
static gboolean
soma_panel_map_event (GtkWidget   *widget,
                      GdkEventAny *event)
{
  gulong data[4] = {0};

  data[2] = gtk_widget_get_allocated_height (widget);

  gdk_property_change(gtk_widget_get_window (widget),
                      gdk_atom_intern ("_NET_WM_STRUT", FALSE),
                      gdk_atom_intern ("CARDINAL", FALSE),
                      32, GDK_PROP_MODE_REPLACE,
                      (guchar*)data, 4);

  GTK_WIDGET_CLASS (soma_panel_parent_class)->realize (widget);
}

/* Set an RGBA visual on the window if possible for the new screen to be
 * able to use the compositor/opacity and make sure the panel is properly
 * seated in the correct position on the new screen. */
static void
on_soma_panel_screen_changed (GtkWidget *widget,
                              GdkScreen *old_screen,
                              gpointer   user_data)
{
  SomaPanel *self;
  GError *error = NULL;
  GdkScreen *screen;
  GdkVisual *visual;
  GtkCssProvider *css_provider;
  GtkStyleContext *style_context;

  self = SOMA_PANEL (widget);
  screen = gtk_widget_get_screen (widget);
  visual = gdk_screen_get_rgba_visual (screen);

  if (!visual)
    {
      visual = gdk_screen_get_system_visual (screen);
      self->priv->supports_alpha = FALSE;
    }
  else
    self->priv->supports_alpha = TRUE;

  gtk_widget_set_visual (widget, visual);

  css_provider = gtk_css_provider_new ();
  if (!gtk_css_provider_load_from_data (css_provider,
                                        soma_panel_css,
                                        soma_panel_css_length,
                                        &error))
    {
      g_critical ("Unable to load Soma panel CSS definition: %s",
                  error->message);
      g_error_free (error); error = NULL;
    }

  gtk_style_context_add_provider_for_screen (screen,
                                             GTK_STYLE_PROVIDER (css_provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_USER);

  g_object_unref (css_provider);

  gtk_window_move (GTK_WINDOW (widget), 0, 0);
  gtk_window_resize (GTK_WINDOW (widget),
                     200/*gdk_screen_get_width (screen)*/,
                     gtk_widget_get_allocated_height (widget));
}

static void
soma_panel_class_init (SomaPanelClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  g_type_class_add_private ((gpointer)klass, sizeof (SomaPanelPrivate));

  widget_class->map_event = soma_panel_map_event;
}

static void
soma_panel_init (SomaPanel *self)
{
  GError *error = NULL;
  GtkBuilder *builder;

  self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, SOMA_TYPE_PANEL, SomaPanelPrivate);

  builder = gtk_builder_new ();
  if (gtk_builder_add_from_string (builder,
                                   soma_panel_ui,
                                   soma_panel_ui_length,
                                   &error))
    {
      GtkWidget *widget;
      /* TODO: take any pointers to the widget that'll be needed later */
      /* steal the event box (root child) from the GtkBuilder and put it
       * inside the panel */
      widget = GTK_WIDGET (gtk_builder_get_object (builder, "soma_viewport"));
      gtk_widget_reparent (widget, GTK_WIDGET (self));
      //gtk_widget_show_all (widget);
      widget = GTK_WIDGET (gtk_builder_get_object (builder, "soma_panel_window"));
      /* delete the old window from the glade file since it's not needed */
      gtk_widget_destroy (widget);
    }
  else
    {
      gchar *msg;
      GtkWidget *label;
      msg = g_strdup_printf ("Unable to load Soma panel UI definition: %s",
                             error->message);
      g_error_free (error); error = NULL;
      g_critical ("%s", msg);
      label = gtk_label_new (msg);
      g_free (msg);
      gtk_container_add (GTK_CONTAINER (self), label);
    }
  g_object_unref (builder);

  gtk_window_stick (GTK_WINDOW (self));

  /* Using a normal signal here instead of overiding screen_changed method
   * from parent class since it caused inexplicable and undebuggable segfaults. */
  g_signal_connect (GTK_WIDGET (self),
                    "screen-changed",
                    G_CALLBACK (on_soma_panel_screen_changed),
                    NULL);

  soma_panel_names_from_buildable (GTK_WIDGET (self));

  on_soma_panel_screen_changed (GTK_WIDGET (self), NULL, NULL);
}

GtkWidget *
soma_panel_new (void)
{
  return g_object_new (SOMA_TYPE_PANEL,
                       "type",              GTK_WINDOW_TOPLEVEL,
                       "type-hint",         GDK_WINDOW_TYPE_HINT_DOCK,
                       "has-resize-grip",   FALSE,
                       "decorated",         FALSE,
                       "skip-pager-hint",   TRUE,
                       "skip-taskbar-hint", TRUE,
                       NULL);
}
