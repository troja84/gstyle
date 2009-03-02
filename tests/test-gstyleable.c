#include "gstylesheet.h"
#include "gstyleclass.h"

static GStyleClass *button = NULL;
static GStyleClass *notebook = NULL;
static GStyleClass *notebooktab = NULL;
static GStyleClass *notebooktab_first = NULL;
static GStyleClass *treeviewrow = NULL;

static GStyleClass*
init_style_class_hierarchy (void)
{
  GStyleClass *root, *child, *element, *sub_element;
  GStyleClass *sub_child, *sub_sub_child, *sub_sub_sub_child;
  gint x = 0;
  
  root = g_style_class_new ("window");

  notebook = child = g_style_class_new ("notebook");

  /* add tabs - START */
  for (x = 0; x < 3; x++)
    {
      element = g_style_class_new ("tab");
      g_style_class_add_child (child, element);

      if (x == 0)
        notebooktab_first = element;
      else
        notebooktab = element;

      sub_child = g_style_class_new ("hbox");
      g_style_class_add_child (element, sub_child);

      sub_sub_child = g_style_class_new ("label");
      g_style_class_add_child (sub_child, sub_sub_child);

      button = sub_sub_child = g_style_class_new ("button");
      g_style_class_add_child (sub_child, sub_sub_child);

      sub_sub_sub_child = g_style_class_new ("label");
      g_style_class_add_child (sub_sub_child, sub_sub_sub_child);
    }
  /* END */
  
  element = g_style_class_new ("page");
  sub_element = g_style_class_new ("scrolled window");

  sub_child = g_style_class_new ("treeview");
  /* add rows - START */
  treeviewrow = sub_sub_child = g_style_class_new ("row");
  g_style_class_add_child (sub_child, sub_sub_child);
  sub_sub_sub_child = g_style_class_new ("button");
  g_style_class_add_child (sub_sub_child, sub_sub_sub_child);

  sub_sub_child = g_style_class_new ("row");
  g_style_class_add_child (sub_child, sub_sub_child);
  sub_sub_sub_child = g_style_class_new ("button");
  g_style_class_add_child (sub_sub_child, sub_sub_sub_child);

  sub_sub_child = g_style_class_new ("row");
  g_style_class_add_child (sub_child, sub_sub_child);
  sub_sub_sub_child = g_style_class_new ("button");
  g_style_class_add_child (sub_sub_child, sub_sub_sub_child);
  /* END */
  g_style_class_add_child (sub_element, sub_child);

  /* add scrollbars - START */
  sub_child = g_style_class_new ("scrollbar");
  g_style_class_add_child (sub_element, sub_child);
  sub_child = g_style_class_new ("scrollbar");
  g_style_class_add_child (sub_element, sub_child);
  /* END */

  g_style_class_add_child (element, sub_element);
  g_style_class_add_child (child, element);

  g_style_class_add_child (root, child); 

  return root;
}

int main (void)
{
  GStylesheet *stylesheet;
  GStyleClass *style_class;
  gchar *property = NULL;

  g_print ("Started...\n");
  g_type_init ();

  style_class = init_style_class_hierarchy ();

  g_print ("> Name: %s\n", g_style_class_get_name (style_class));
  
  g_print ("Load file...\n");

  stylesheet = g_stylesheet_new_from_file ("./hierarchy.css", NULL);

  g_print ("Get property <button> ");
  g_stylesheet_get_property (stylesheet,
                             G_STYLEABLE (button),
                             "color",
                             &property);
  g_print ("%s\n", property);

  g_print ("Get property <notebook> ");
  g_stylesheet_get_property (stylesheet,
                             G_STYLEABLE (notebook),
                             "color",
                             &property);
  g_print ("%s\n", property);

  g_print ("Get property <notebook > tab> ");
  g_stylesheet_get_property (stylesheet,
                             G_STYLEABLE (notebooktab),
                             "color",
                             &property);
  g_print ("%s\n", property);

  g_print ("Get property <notebook > tab:first-child> ");
  g_stylesheet_get_property (stylesheet,
                             G_STYLEABLE (notebooktab_first),
                             "color",
                             &property);
  g_print ("%s\n", property);

  g_print ("Get property <treeview > row> ");
  g_stylesheet_get_property (stylesheet,
                             G_STYLEABLE (treeviewrow),
                             "color",
                             &property);
  g_print ("%s\n", property);

  g_print ("Finished...\n");

  return 0;
}
