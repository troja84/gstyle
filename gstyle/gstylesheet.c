#include "gstylesheet.h"

#include <glib.h>
#include <libcroco/libcroco.h>

G_DEFINE_TYPE (GStylesheet, g_stylesheet, G_TYPE_OBJECT)

#define G_STYLESHEET_GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), G_TYPE_STYLESHEET, GStylesheetPrivate))

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

          g_debug ("> STATUS: %d %ld\n", 
                   status == CR_OK,
                   length);

          cr_stylesheet_dump (priv->stylesheet, stdout);

          g_print ("\n\n");
        }
    }

  return stylesheet;
}

gboolean
g_stylesheet_get_property (GStylesheet  *stylesheet,
                           CRNode const *node,
                           const gchar  *property_name,
                           gchar        **property)
{
  GStylesheetPrivate *priv;
  CRStatement **stmts_tab = NULL;
  gulong length;
  CRSelEng *engine;
  enum CRStatus status;
  gulong x;

  priv = G_STYLESHEET_GET_PRIVATE (stylesheet);

  engine = cr_sel_eng_new ();

  status = cr_sel_eng_get_matched_rulesets (engine,
                                            priv->stylesheet,
                                            node,
                                            &stmts_tab,
                                            &length);

  for (x = 0; x < length; x++)
    {
      if (stmts_tab[x]->type == RULESET_STMT)
        {
          CRDeclaration *decl_list = NULL;
          CRDeclaration *decl_property = NULL;
          CRTerm *value = NULL;

          cr_statement_ruleset_get_declarations (stmts_tab[x],
                                                 &decl_list);

          decl_property = cr_declaration_get_by_prop_name (decl_list,
                                                           property_name);

          value = decl_property->value;

          *property = cr_term_to_string (value);
        }
    }
}
