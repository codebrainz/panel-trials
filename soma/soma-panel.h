/*
 * soma-panel.h - The main window/panel widget
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
 *
 */

#ifndef SOMA_PANEL_H_
#define SOMA_PANEL_H_ 1

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SOMA_TYPE_PANEL            (soma_panel_get_type ())
#define SOMA_PANEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOMA_TYPE_PANEL, SomaPanel))
#define SOMA_PANEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SOMA_TYPE_PANEL, SomaPanelClass))
#define SOMA_IS_PANEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOMA_TYPE_PANEL))
#define SOMA_IS_PANEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SOMA_TYPE_PANEL))
#define SOMA_PANEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SOMA_TYPE_PANEL, SomaPanelClass))

typedef struct SomaPanel_        SomaPanel;
typedef struct SomaPanelClass_   SomaPanelClass;
typedef struct SomaPanelPrivate_ SomaPanelPrivate;

struct SomaPanel_
{
  GtkWindow parent;
  SomaPanelPrivate *priv;
};

struct SomaPanelClass_
{
  GtkWindowClass parent_class;
};

GType soma_panel_get_type (void);
GtkWidget *soma_panel_new (void);
void soma_panel_add_launcher (SomaPanel *panel, const gchar *app_name);
void soma_panel_remove_launcher (SomaPanel *panel, const gchar *app_name);
GList soma_panel_list_launchers (SomaPanel *panel);

G_END_DECLS

#endif /* SOMA_PANEL_H_ */
