/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; c-indent-level: 8 -*- */
/* this file is part of evince, a gnome document viewer
 *
 *  Copyright (C) 2005 Red Hat, Inc.
 *
 * Evince is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Evince is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ev-link.h"

enum {
	PROP_0,
	PROP_TITLE,
	PROP_TYPE,
	PROP_PAGE,
	PROP_URI,
	PROP_LEFT,
	PROP_TOP,
	PROP_ZOOM
};


struct _EvLink {
	GObject base_instance;
	EvLinkPrivate *priv;
};

struct _EvLinkClass {
	GObjectClass base_class;
};

struct _EvLinkPrivate {
	char *title;
	char *uri;
	EvLinkType type;
	int page;
	double top;
	double left;
	double zoom;
};

G_DEFINE_TYPE (EvLink, ev_link, G_TYPE_OBJECT)

#define EV_LINK_GET_PRIVATE(object) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((object), EV_TYPE_LINK, EvLinkPrivate))

GType
ev_link_type_get_type (void)
{
	static GType type = 0;

	if (G_UNLIKELY (type == 0)) {
		static const GEnumValue values[] = {
			{ EV_LINK_TYPE_TITLE, "EV_LINK_TYPE_TITLE", "title" },
			{ EV_LINK_TYPE_PAGE, "EV_LINK_TYPE_PAGE", "page" },
			{ EV_LINK_TYPE_PAGE_XYZ, "EV_LINK_TYPE_PAGE_XYZ", "page-xyz" },
			{ EV_LINK_TYPE_EXTERNAL_URI, "EV_LINK_TYPE_EXTERNAL_URI", "external" },
			{ 0, NULL, NULL }
                };

                type = g_enum_register_static ("EvLinkType", values);
        }

        return type;
}

const char *
ev_link_get_title (EvLink *self)
{
	g_return_val_if_fail (EV_IS_LINK (self), NULL);
	
	return self->priv->title;
}

const char *
ev_link_get_uri (EvLink *self)
{
	g_return_val_if_fail (EV_IS_LINK (self), NULL);
	
	return self->priv->uri;
}

EvLinkType
ev_link_get_link_type (EvLink *self)
{
	g_return_val_if_fail (EV_IS_LINK (self), 0);
	
	return self->priv->type;
}

int
ev_link_get_page (EvLink *self)
{
	g_return_val_if_fail (EV_IS_LINK (self), 0);
	
	return self->priv->page;
}

double
ev_link_get_top (EvLink *self)
{
	g_return_val_if_fail (EV_IS_LINK (self), 0);
	
	return self->priv->top;
}

double
ev_link_get_left (EvLink *self)
{
	g_return_val_if_fail (EV_IS_LINK (self), 0);
	
	return self->priv->left;
}

double
ev_link_get_zoom (EvLink *self)
{
	g_return_val_if_fail (EV_IS_LINK (self), 0);
	
	return self->priv->zoom;
}

static void
ev_link_get_property (GObject *object, guint prop_id, GValue *value,
		      GParamSpec *param_spec)
{
	EvLink *self;

	self = EV_LINK (object);

	switch (prop_id) {
	case PROP_TITLE:
		g_value_set_string (value, self->priv->title);
		break;
	case PROP_URI:
		g_value_set_string (value, self->priv->uri);
		break;
	case PROP_TYPE:
		g_value_set_enum (value, self->priv->type);
		break;
	case PROP_PAGE:
		g_value_set_int (value, self->priv->page);
		break;
	case PROP_TOP:
		g_value_set_double (value, self->priv->top);
		break;
	case PROP_LEFT:
		g_value_set_double (value, self->priv->left);
		break;
	case PROP_ZOOM:
		g_value_set_double (value, self->priv->zoom);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
						   prop_id,
						   param_spec);
		break;
	}
}

static void
ev_link_set_property (GObject *object, guint prop_id, const GValue *value,
		      GParamSpec *param_spec)
{
	EvLink *link = EV_LINK (object);
	
	switch (prop_id) {
	case PROP_TITLE:
		link->priv->title = g_strdup (g_value_get_string (value));	
		break;
	case PROP_URI:
		link->priv->uri = g_strdup (g_value_get_string (value));
		break;
	case PROP_TYPE:
		link->priv->type = g_value_get_enum (value);
		break;
	case PROP_PAGE:
		link->priv->page = g_value_get_int (value);
		break;
	case PROP_TOP:
		link->priv->top = g_value_get_double (value);
		break;
	case PROP_LEFT:
		link->priv->left = g_value_get_double (value);
		break;
	case PROP_ZOOM:
		link->priv->zoom = g_value_get_double (value);
		break;

	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object,
						   prop_id,
						   param_spec);
		break;
	}
}

static void
ev_window_dispose (GObject *object)
{
	EvLinkPrivate *priv;

	g_return_if_fail (EV_IS_LINK (object));

	priv = EV_LINK (object)->priv;

	if (priv->title) {
		g_free (priv->title);
		priv->title = NULL;
	}

	if (priv->uri) {
		g_free (priv->uri);
		priv->uri = NULL;
	}

	G_OBJECT_CLASS (ev_link_parent_class)->dispose (object);
}

static void
ev_link_init (EvLink *ev_link)
{
	ev_link->priv = EV_LINK_GET_PRIVATE (ev_link);

	ev_link->priv->type = EV_LINK_TYPE_TITLE;
}

static void
ev_link_class_init (EvLinkClass *ev_window_class)
{
	GObjectClass *g_object_class;

	g_object_class = G_OBJECT_CLASS (ev_window_class);
	g_object_class->dispose = ev_window_dispose;
	g_object_class->set_property = ev_link_set_property;
	g_object_class->get_property = ev_link_get_property;

	g_type_class_add_private (g_object_class, sizeof (EvLinkPrivate));

	g_object_class_install_property (g_object_class,
					 PROP_TITLE,
					 g_param_spec_string ("title",
				     	 		      "Link Title",
				     			      "The link title",
							      NULL,
							      G_PARAM_READWRITE |
				     			      G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property (g_object_class,
					 PROP_URI,
					 g_param_spec_string ("uri",
				     	 		      "Link URI",
				     			      "The link URI",
							      NULL,
							      G_PARAM_READWRITE |
				     			      G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property (g_object_class,
					 PROP_TYPE,
			 		 g_param_spec_enum  ("type",
                              				     "Link Type",
							     "The link type",
							     EV_TYPE_LINK_TYPE,
							     EV_LINK_TYPE_TITLE,
							     G_PARAM_READWRITE |
							     G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property (g_object_class,
					 PROP_PAGE,
					 g_param_spec_int ("page",
							   "Link Page",
							   "The link page",
							    -1,
							    G_MAXINT,
							    0,
							    G_PARAM_READWRITE |
							    G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property (g_object_class,
					 PROP_LEFT,
					 g_param_spec_double ("left",
							      "Left coordinate",
							      "The left coordinate",
							      -G_MAXDOUBLE,
							      G_MAXDOUBLE,
							      0,
							      G_PARAM_READWRITE |
							      G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property (g_object_class,
					 PROP_TOP,
					 g_param_spec_double ("top",
							      "Top coordinate",
							      "The top coordinate",
							      -G_MAXDOUBLE,
							      G_MAXDOUBLE,
							      0,
							      G_PARAM_READWRITE |
							      G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property (g_object_class,
					 PROP_ZOOM,
					 g_param_spec_double ("zoom",
							      "Zoom",
							      "Zoom",
							      0,
							      G_MAXDOUBLE,
							      0,
							      G_PARAM_READWRITE |
							      G_PARAM_CONSTRUCT_ONLY));
}

EvLink *
ev_link_new_title (const char *title)
{
	return EV_LINK (g_object_new (EV_TYPE_LINK,
				      "title", title,
				      "type", EV_LINK_TYPE_TITLE,
				      NULL));
}

EvLink *
ev_link_new_page (const char *title, int page)
{
	return EV_LINK (g_object_new (EV_TYPE_LINK,
				      "title", title,
				      "page", page,
				      "type", EV_LINK_TYPE_PAGE,
				      NULL));
}

EvLink *
ev_link_new_page_xyz (const char *title,
		      int         page,
		      double      left,
		      double      top,
		      double      zoom)
{
	return EV_LINK (g_object_new (EV_TYPE_LINK,
				      "title", title,
				      "page", page,
				      "type", EV_LINK_TYPE_PAGE_XYZ,
				      "left", left,
				      "top", top,
				      "zoom", zoom,
				      NULL));
}

EvLink *
ev_link_new_external (const char *title, const char *uri)
{
	return EV_LINK (g_object_new (EV_TYPE_LINK,
				      "title", title,
				      "uri", uri,
				      "type", EV_LINK_TYPE_EXTERNAL_URI,
				      NULL));
}



static void
ev_link_mapping_free_foreach (EvLinkMapping *mapping)
{
	g_object_unref (G_OBJECT (mapping->link));
	g_free (mapping);
}

void
ev_link_mapping_free (GList *link_mapping)
{
	if (link_mapping == NULL)
		return;

	g_list_foreach (link_mapping, (GFunc) (ev_link_mapping_free_foreach), NULL);
	g_list_free (link_mapping);
}


EvLink *
ev_link_mapping_find (GList   *link_mapping,
		      gdouble  x,
		      gdouble  y)
{
	GList *list;
	EvLink *link = NULL;
	int i;
	
	i = 0;

	for (list = link_mapping; list; list = list->next) {
		EvLinkMapping *mapping = list->data;

		i++;
		if ((x >= mapping->x1) &&
		    (y >= mapping->y1) &&
		    (x <= mapping->x2) &&
		    (y <= mapping->y2)) {
			link = mapping->link;
			break;
		}
	}

	return link;
}

