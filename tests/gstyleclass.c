#include "gstyleclass.h"
#include "gstyleable.h"

#define G_STYLE_CLASS_GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), G_TYPE_STYLE_CLASS, GStyleClassPrivate))

typedef struct _GStyleClassPrivate GStyleClassPrivate;

struct _GStyleClassPrivate {
  gchar *name;

  GStyleable *parent;

  GList *children;

  GHashTable *attributes;
};

static void g_styleable_interface_init (GStyleableIface *iface);

/* GStyleable functions */
static GStyleable* g_style_class_get_parent_node      (const GStyleable *node);
static GStyleable* g_style_class_get_first_child      (const GStyleable *node);
static GStyleable* g_style_class_get_next_sibling     (const GStyleable *node);
static GStyleable* g_style_class_get_previous_sibling (const GStyleable *node);
static const char* g_style_class_get_node_name        (const GStyleable *node);
static char*       g_style_class_get_attribute        (const GStyleable *node,
                                                       const char       *property);
static guint       g_style_class_get_children_count   (const GStyleable *node);
static guint       g_style_class_get_index            (const GStyleable *node);
static void        g_style_class_release              (GStyleable       *node);


/*
G_DEFINE_TYPE (GStyleClass, g_style_class, G_TYPE_OBJECT);
*/
G_DEFINE_TYPE_WITH_CODE (GStyleClass, g_style_class, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (G_TYPE_STYLEABLE,
                                                g_styleable_interface_init));

static void
g_styleable_interface_init (GStyleableIface *iface)
{
  iface->get_parent_node = g_style_class_get_parent_node;
  iface->get_first_child = g_style_class_get_first_child;
  iface->get_next_sibling = g_style_class_get_next_sibling;
  iface->get_previous_sibling = g_style_class_get_previous_sibling;
  iface->get_node_name = g_style_class_get_node_name;
  iface->get_attribute = g_style_class_get_attribute;
  iface->get_children_count = g_style_class_get_children_count;
  iface->get_index = g_style_class_get_index;
  iface->release = g_style_class_release;
}

static void
g_style_class_dispose (GObject *object)
{
  G_OBJECT_CLASS (g_style_class_parent_class)->dispose (object);
}

static void
g_style_class_finalize (GObject *object)
{
  G_OBJECT_CLASS (g_style_class_parent_class)->finalize (object);
}

static void
g_style_class_class_init (GStyleClassClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = g_style_class_dispose;
  object_class->finalize = g_style_class_finalize;

  g_type_class_add_private (klass, sizeof (GStyleClassPrivate));
}

static void
g_style_class_init (GStyleClass *self)
{
  GStyleClassPrivate *priv;

  priv = G_STYLE_CLASS_GET_PRIVATE (self);

  priv->name = NULL;

  priv->parent = NULL;

  priv->children = NULL;

  priv->attributes = g_hash_table_new (g_str_hash, g_str_equal);
}

GStyleClass*
g_style_class_new (gchar *name)
{
  GStyleClass *style_class;

  style_class =  g_object_new (G_TYPE_STYLE_CLASS, NULL);

  g_style_class_set_name (style_class, name);

  return style_class;
}

/* GStyleable functions */
static GStyleable*
g_style_class_get_parent_node (const GStyleable *node)
{
  GStyleClassPrivate *priv;

  priv = G_STYLE_CLASS_GET_PRIVATE (node);

  return priv->parent;
}

static GStyleable*
g_style_class_get_first_child (const GStyleable *node)
{
  GStyleClassPrivate *priv;

  priv = G_STYLE_CLASS_GET_PRIVATE (node);

  if (!priv->children)
    return NULL;

  return G_STYLEABLE (priv->children->data);
}

static GStyleable*
g_style_class_get_next_sibling (const GStyleable *node)
{
  GStyleClassPrivate *priv;
  GStyleClassPrivate *parent_priv;
  GStyleable *sibling = NULL;

  priv = G_STYLE_CLASS_GET_PRIVATE (node);

  if (priv->parent)
    {
      GList *children;

      parent_priv = G_STYLE_CLASS_GET_PRIVATE (priv->parent);

      children = parent_priv->children;

      while (children && children->data != node)
        children = children->next;

      sibling = G_STYLEABLE (children->next->data);
    }

  return sibling;
}

static GStyleable*
g_style_class_get_previous_sibling (const GStyleable *node)
{
  GStyleClassPrivate *priv;
  GStyleClassPrivate *parent_priv;
  GStyleable *sibling = NULL;

  priv = G_STYLE_CLASS_GET_PRIVATE (node);

  if (priv->parent)
    {
      GList *children;

      parent_priv = G_STYLE_CLASS_GET_PRIVATE (priv->parent);

      children = parent_priv->children;

      while (children && children->data != node)
        children = children->next;

      sibling = G_STYLEABLE (children->prev->data);
    }

  return sibling;
}

static const char*
g_style_class_get_node_name (const GStyleable *node)
{
  return g_style_class_get_name (G_STYLE_CLASS (node));
}

static char*
g_style_class_get_attribute (const GStyleable *node,
                             const char       *property)
{
  GStyleClassPrivate *priv;

  priv = G_STYLE_CLASS_GET_PRIVATE (node);

  return (gchar*)g_hash_table_lookup (priv->attributes, 
                                      (gconstpointer)property);
}

static guint
g_style_class_get_children_count (const GStyleable *node)
{
  GStyleClassPrivate *priv;

  priv = G_STYLE_CLASS_GET_PRIVATE (node);

  return g_list_length (priv->children);
}

static guint
g_style_class_get_index (const GStyleable *node)
{
  GStyleClassPrivate *priv;
  GStyleClassPrivate *parent_priv;
  gint index = -1;

  priv = G_STYLE_CLASS_GET_PRIVATE (node);

  if (priv->parent)
    {
      parent_priv = G_STYLE_CLASS_GET_PRIVATE (priv->parent);

      index = g_list_index (parent_priv->children, node);
    }

  g_return_val_if_fail (index >= 0, 0);

  return (guint)index;
}

static void
g_style_class_release (GStyleable *node)
{
  GStyleClassPrivate *priv;
  GList *children;

  priv = G_STYLE_CLASS_GET_PRIVATE (node);

  g_hash_table_unref (priv->attributes);

  children = priv->children;

  while (children)
    {
      g_style_class_release (G_STYLEABLE (children->data));

      children = children->next;
    }

  g_object_unref (node);

}

/* GStyleClass functions */
void
g_style_class_set_name (GStyleClass *style_class,
                        gchar       *name)
{
  GStyleClassPrivate *priv;

  priv = G_STYLE_CLASS_GET_PRIVATE (style_class);

  priv->name = name;
}

gchar*
g_style_class_get_name (GStyleClass *style_class)
{
  GStyleClassPrivate *priv;

  priv = G_STYLE_CLASS_GET_PRIVATE (style_class);

  return priv->name;
}

void
g_style_class_add_child (GStyleClass *node,
                         GStyleClass *child)
{
  GStyleClassPrivate *priv;
  GStyleClassPrivate *child_priv;

  g_return_if_fail (node && child);

  priv = G_STYLE_CLASS_GET_PRIVATE (node);

  if (g_list_index (priv->children, child) >= 0)
    {
      g_warning ("Attempting to add a child to a styleable, "
                 "but the widget is already a child of the styleable. "
                 "Nothing will be added!");
      return;
    }

  priv->children = g_list_append (priv->children, child);

  child_priv = G_STYLE_CLASS_GET_PRIVATE (child);

  /*
  g_print ("> ADD INDEX NODE: %s %s %d\n", priv->name, 
           child_priv->name, g_list_index (priv->children, child));
  */

  child_priv->parent = G_STYLEABLE (node);
}

void
g_style_class_set_first_child (GStyleClass *node,
                               GStyleClass *first_child)
{
  GStyleClassPrivate *priv;

  g_return_if_fail (node && first_child);

  priv = G_STYLE_CLASS_GET_PRIVATE (node);

  if (g_list_index (priv->children, first_child) >= 0)
    {
      GList *child;

      child = g_list_remove (priv->children, first_child);
      g_free (child);
    }
  else
    {
      GStyleClassPrivate *child_priv;

      child_priv = G_STYLE_CLASS_GET_PRIVATE (first_child);

      child_priv->parent = G_STYLEABLE (node);
    }

  priv->children = g_list_prepend (priv->children, first_child);
}
