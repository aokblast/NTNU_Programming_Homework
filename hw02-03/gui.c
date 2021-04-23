#include "all.h"
#include <glib/gi18n.h>
#include "gui.h"

void closeAll(gtkobj widget, gpointer data) {
    g_application_quit(widget);
    g_application_quit((gtkobj) data);
    exit(0);
}

void fileSelect(gtkobj editor_window) {

    gtkobj fileChooser = gtk_file_chooser_dialog_new(_("Open save"),
                                                    GTK_WINDOW(editor_window),
	                                                GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
	                                                GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                NULL);

    //setting botton
    //g_signal_connect(fileChooser, "destroy", G_CALLBACK (closeAll), editor_window);


    //setting filter
    gtkfobj filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.S5P");
    gtk_file_filter_set_name(filter, "S5PK save(.S5P)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fileChooser), filter);

    //file Select
    switch (gtk_dialog_run(GTK_DIALOG(fileChooser))) {
        case GTK_RESPONSE_ACCEPT:
        {
            gchar *fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));
            printf("%s\n", fileName);
            break;
        }
    }
    
    gtk_widget_destroy(fileChooser);
}

void  init_Window() {

    gtk_init(NULL, NULL);
    //Editor Setting
    gtkobj editor_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(editor_window), "SAN5PK save editor");
    g_signal_connect(editor_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    fileSelect(editor_window);
    

    //Start
    gtk_widget_show(editor_window);
    gtk_main();
}