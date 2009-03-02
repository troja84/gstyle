#ifndef _G_STYLE_CLASS
#define _G_STYLE_CLASS

#include <glib-object.h>

G_BEGIN_DECLS

#define G_TYPE_STYLE_CLASS g_style_class_get_type()

#define G_STYLE_CLASS(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_STYLE_CLASS, GStyleClass))

#define G_STYLE_CLASS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), G_TYPE_STYLE_CLASS, GStyleClassClass))

#define G_IS_STYLE_CLASS(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_STYLE_CLASS))

#define G_IS_STYLE_CLASS_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), G_TYPE_STYLE_CLASS))

#define G_STYLE_CLASS_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), G_TYPE_STYLE_CLASS, GStyleClassClass))

typedef struct {
  GObject parent;
} GStyleClass;

typedef struct {
  GObjectClass parent_class;
} GStyleClassClass;

GType g_style_class_get_type (void);

GStyleClass*  g_style_class_new             (gchar *name);

void          g_style_class_set_name        (GStyleClass *style_class,
                                             gchar *name);
gchar*        g_style_class_get_name        (GStyleClass *style_class);

void          g_style_class_add_child       (GStyleClass *node,
                                             GStyleClass *child);
void          g_style_class_set_first_child (GStyleClass *node,
                                             GStyleClass *first_child);

G_END_DECLS

#endif /* _G_STYLE_CLASS */
