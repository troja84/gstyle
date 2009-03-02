#include "gstyleable.h"


static void g_styleable_base_init (GStyleableIface *iface);

GType
g_styleable_get_type (void)
{
  static GType styleable_type = 0;

  if (!styleable_type)
    {
      const GTypeInfo styleable_info =
      {
        sizeof (GStyleableIface),
        (GBaseInitFunc)     g_styleable_base_init,
        (GBaseFinalizeFunc) NULL,
      };

      styleable_type = g_type_register_static (G_TYPE_INTERFACE,
                                               "GStyleable",
                                               &styleable_info, 0);
    }

  return styleable_type;
}

static void
g_styleable_base_init (GStyleableIface *iface)
{
  static gboolean initialized = FALSE;

  if (initialized)
    return;

  initialized = TRUE;
}

GStyleable*
g_styleable_get_parent_node (GStyleable const *styleable)
{
  g_return_val_if_fail (G_IS_STYLEABLE (styleable), NULL);

  return G_STYLEABLE_GET_INTERFACE (styleable)->get_parent_node (styleable);
}

GStyleable*
g_styleable_get_first_child (GStyleable const *styleable)
{
  g_return_val_if_fail (G_IS_STYLEABLE (styleable), NULL);

  return G_STYLEABLE_GET_INTERFACE (styleable)->get_first_child (styleable);
}

GStyleable*
g_styleable_get_next_sibling (GStyleable const *styleable)
{
  g_return_val_if_fail (G_IS_STYLEABLE (styleable), NULL);

  return G_STYLEABLE_GET_INTERFACE (styleable)->get_next_sibling (styleable);
}

GStyleable*
g_styleable_get_previous_sibling (GStyleable const *styleable)
{
  g_return_val_if_fail (G_IS_STYLEABLE (styleable), NULL);

  return G_STYLEABLE_GET_INTERFACE (styleable)->get_previous_sibling (styleable);
}

gchar const*
g_styleable_get_node_name (GStyleable const *styleable)
{
  g_return_val_if_fail (G_IS_STYLEABLE (styleable), NULL);

  return G_STYLEABLE_GET_INTERFACE (styleable)->get_node_name (styleable);
}

gchar*
g_styleable_get_attribute (GStyleable const *styleable,
                           gchar      const *property)
{
  g_return_val_if_fail (G_IS_STYLEABLE (styleable), NULL);

  return G_STYLEABLE_GET_INTERFACE (styleable)->get_attribute (styleable, property);
}

guint
g_styleable_get_children_count (GStyleable const *styleable)
{
  g_return_val_if_fail (G_IS_STYLEABLE (styleable), 0);

  return G_STYLEABLE_GET_INTERFACE (styleable)->get_children_count (styleable);
}

guint
g_styleable_get_index (GStyleable const *styleable)
{
  g_return_val_if_fail (G_IS_STYLEABLE (styleable), 0);

  return G_STYLEABLE_GET_INTERFACE (styleable)->get_index (styleable);
}

void
g_styleable_release (GStyleable *styleable)
{
  g_return_if_fail (G_IS_STYLEABLE (styleable));

  return G_STYLEABLE_GET_INTERFACE (styleable)->release (styleable);
}
