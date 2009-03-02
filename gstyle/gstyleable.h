#ifndef _G_STYLEABLE
#define _G_STYLEABLE

#include <glib-object.h>

G_BEGIN_DECLS

#define G_TYPE_STYLEABLE g_styleable_get_type()

#define G_STYLEABLE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_STYLEABLE, GStyleable))

#define G_IS_STYLEABLE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_STYLEABLE))

#define G_STYLEABLE_GET_INTERFACE(obj) \
  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), G_TYPE_STYLEABLE, GStyleableIface))

typedef struct _GStyleable GStyleable; /* dummy object */

typedef struct {
  GTypeInterface base_iface;

  GStyleable*        (*get_parent_node)      (const GStyleable*);
	GStyleable*        (*get_first_child)      (const GStyleable*);
	GStyleable*        (*get_next_sibling)     (const GStyleable*);
	GStyleable*        (*get_previous_sibling) (const GStyleable*);
	char const*        (*get_node_name)        (const GStyleable*);
	char*              (*get_attribute)        (const GStyleable*,
                                              const char*);
	guint              (*get_children_count)   (const GStyleable*);
	guint              (*get_index)            (const GStyleable*);
	void               (*release)              (GStyleable*);

} GStyleableIface;

GType              g_styleable_get_type             (void);

GStyleable*        g_styleable_get_parent_node      (const GStyleable *node);
GStyleable*        g_styleable_get_first_child      (const GStyleable *node);
GStyleable*        g_styleable_get_next_sibling     (const GStyleable *node);
GStyleable*        g_styleable_get_previous_sibling (const GStyleable *node);
gchar const*       g_styleable_get_node_name        (const GStyleable *node);
gchar*             g_styleable_get_attribute        (const GStyleable *node,
                                                     const gchar      *property);
guint              g_styleable_get_children_count   (const GStyleable *node);
guint              g_styleable_get_index            (const GStyleable *node);
void               g_styleable_release              (GStyleable       *node);


G_END_DECLS

#endif /* _G_STYLEABLE */
