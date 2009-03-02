#include "gstylesheet.h"

#include <glib.h>
#include <libcroco/libcroco.h>

G_DEFINE_TYPE (GStylesheet, g_stylesheet, G_TYPE_OBJECT)

#define G_STYLESHEET_GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), G_TYPE_STYLESHEET, GStylesheetPrivate))

static CRNodeIface const * peek_node_iface (void);
 
typedef struct _GStylesheetPrivate GStylesheetPrivate;

struct _GStylesheetPrivate {
  CRStyleSheet *stylesheet;
};

static void
g_stylesheet_dispose (GObject *object)
{
  G_OBJECT_CLASS (g_stylesheet_parent_class)->dispose (object);
}

static void
g_stylesheet_finalize (GObject *object)
{
  G_OBJECT_CLASS (g_stylesheet_parent_class)->finalize (object);
}

static void
g_stylesheet_class_init (GStylesheetClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (GStylesheetPrivate));

  object_class->dispose = g_stylesheet_dispose;
  object_class->finalize = g_stylesheet_finalize;
}

static void
g_stylesheet_init (GStylesheet *self)
{
  GStylesheetPrivate *priv;

  priv = G_STYLESHEET_GET_PRIVATE (self);

  priv->stylesheet = NULL;
}

GStylesheet*
g_stylesheet_new (void)
{
  /*
  static GStylesheet *singleton = NULL;

  if (!singleton)
    {
      singleton = g_object_new (G_TYPE_STYLESHEET, NULL);

      return singleton;
    }

  return singleton;
  */

  return g_object_new (G_TYPE_STYLESHEET, NULL);
}

GStylesheet* g_stylesheet_new_from_file (const gchar *filename,
                                         GError      **error)
{
  GStylesheet *stylesheet;
  GStylesheetPrivate *priv;
  GIOChannel *file;
  GIOStatus gio_status;
  gchar *buffer;
  gsize length;

  buffer = NULL;
  stylesheet = NULL;

  stylesheet = g_stylesheet_new ();
  
  priv = G_STYLESHEET_GET_PRIVATE (stylesheet);

  file = g_io_channel_new_file (filename, "r", error);

  if (file)
    {
      gio_status = g_io_channel_read_to_end (file, &buffer, &length, error);

      gio_status = g_io_channel_shutdown (file, FALSE, error);

      if (!error)
        {
          enum CRStatus status;

          status = cr_om_parser_simply_parse_buf ((guchar*)buffer,
                                                  (gulong)length,
                                                  CR_ASCII,
                                                  &priv->stylesheet);

          g_print ("> STATUS: %d %ld\n\n", 
                   status == CR_OK,
                   length);

          cr_stylesheet_dump (priv->stylesheet, stdout);

          g_print ("\n\n");
        }
    }

  return stylesheet;
}

static CRTerm*
g_stylesheet_get_property_real (CRStyleSheet *stylesheet,
                                const CRNode *node,
                                const gchar  *property_name)
{
  CRStatement **stmts_tab = NULL;
  gulong length;
  CRSelEng *engine;
  enum CRStatus status;
  gulong x;

  engine = cr_sel_eng_new ();

  status = cr_sel_eng_get_matched_rulesets (engine,
                                            stylesheet,
                                            node,
                                            &stmts_tab,
                                            &length);

  for (x = 0; x < length; x++)
    {
      if (stmts_tab[x]->type == RULESET_STMT)
        {
          CRDeclaration *decl_list = NULL;
          CRDeclaration *decl_property = NULL;

          cr_statement_ruleset_get_declarations (stmts_tab[x],
                                                 &decl_list);

          decl_property = cr_declaration_get_by_prop_name (decl_list,
                                                           (const guchar*)
                                                           property_name);

          if (decl_property)
            return decl_property->value;
        }
    }

  return NULL;
}

typedef struct {
        CRNode     base;
        GStyleable *node;
} GStyleableNode;

static CRNode*
g_stylesheet_node_new (GStyleable *styleable)
{
  GStyleableNode *node;

  g_return_val_if_fail (styleable, NULL);

  node = g_new0 (GStyleableNode, 1);
  node->base.iface = peek_node_iface ();
  node->node = styleable;

  return &(node->base);
}

static void
g_stylesheet_node_free (GStyleableNode *node)
{
  g_return_if_fail (node);

  g_free (node);
}

static CRNode*
get_parent_node (CRNode const *node)
{
  GStyleableNode *s_node;
  GStyleable *target;
 
  s_node = (GStyleableNode*)node;

  g_return_val_if_fail (s_node, NULL);
  g_return_val_if_fail (s_node->node, NULL);

  target = g_styleable_get_parent_node (s_node->node);

  return g_stylesheet_node_new (target);
}

static CRNode*
get_first_child (CRNode const *node)
{
  GStyleableNode *s_node;
  GStyleable *target;
 
  s_node = (GStyleableNode*)node;

  g_return_val_if_fail (s_node, NULL);
  g_return_val_if_fail (s_node->node, NULL);

  target = g_styleable_get_first_child (s_node->node);

  return g_stylesheet_node_new (target);
}

static CRNode*
get_next_sibling (CRNode const *node)
{
  GStyleableNode *s_node;
  GStyleable *target;
 
  s_node = (GStyleableNode*)node;

  g_return_val_if_fail (s_node, NULL);
  g_return_val_if_fail (s_node->node, NULL);

  target = g_styleable_get_next_sibling (s_node->node);

  return g_stylesheet_node_new (target);
}

static CRNode*
get_previous_sibling (CRNode const *node)
{
  GStyleableNode *s_node;
  GStyleable *target;
 
  s_node = (GStyleableNode*)node;

  g_return_val_if_fail (s_node, NULL);
  g_return_val_if_fail (s_node->node, NULL);

  target = g_styleable_get_previous_sibling (s_node->node);

  return g_stylesheet_node_new (target);
}

static const gchar*
get_node_name (CRNode const *node)
{
  GStyleableNode *s_node;
 
  s_node = (GStyleableNode*)node;

  g_return_val_if_fail (s_node, NULL);
  g_return_val_if_fail (s_node->node, NULL);

  return g_styleable_get_node_name (s_node->node);
}

static gchar*
get_attribute (CRNode const *node,
               const gchar  *property)
{
  GStyleableNode *s_node;
 
  s_node = (GStyleableNode*)node;

  g_return_val_if_fail (s_node, NULL);
  g_return_val_if_fail (s_node->node, NULL);

  return g_styleable_get_attribute (s_node->node, property);
}

static guint
get_children_count (CRNode const *node)
{
  GStyleableNode *s_node;
 
  s_node = (GStyleableNode*)node;

  g_return_val_if_fail (s_node, 0);
  g_return_val_if_fail (s_node->node, 0);

  return g_styleable_get_children_count (s_node->node);
}

static guint
get_index (CRNode const *node)
{
  GStyleableNode *s_node;
 
  s_node = (GStyleableNode*)node;

  g_return_val_if_fail (s_node, 0);
  g_return_val_if_fail (s_node->node, 0);

  return g_styleable_get_index (s_node->node);
}

static void
release (CRNode *node)
{
  GStyleableNode *s_node = (GStyleableNode*)node;

  g_return_if_fail (s_node);

  g_stylesheet_node_free (s_node);
}

static CRNodeIface const *
peek_node_iface (void)
{
        static CRNodeIface const _node_iface = {
        	.get_parent_node        = get_parent_node,
        	.get_first_child        = get_first_child,
        	.get_next_sibling       = get_next_sibling,
        	.get_previous_sibling   = get_previous_sibling,
        	.get_node_name          = get_node_name,
        	.get_attribute          = get_attribute,
          .get_children_count     = get_children_count,
          .get_index              = get_index,
        	.release                = release
        };

        return &_node_iface;
}

gboolean
g_stylesheet_get_property (GStylesheet *stylesheet,
                           GStyleable  *styleable,
                           const gchar *property_name,
                           gchar       **property)
{
  GStylesheetPrivate *priv;
  CRTerm *value = NULL;
  CRNode *node;

  priv = G_STYLESHEET_GET_PRIVATE (stylesheet);

  node = g_stylesheet_node_new (styleable);

  value = g_stylesheet_get_property_real (priv->stylesheet,
                                          node,
                                          property_name);
                                          
  if (value)
    {
      *property = (gchar *)cr_term_to_string (value);
      return TRUE;
    }
  else
    {
      *property = NULL;
      return FALSE;
    }
}
