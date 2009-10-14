/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include <gtk/gtk.h>

#include "freetuxtv-utils.h"

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"

gchar*
format_time(gint seconds)
{
	const gint s = seconds % 60;
	const gint m = ((seconds - s) / 60)%60;
	const gint h = (seconds - m*60 - s) / (60*60);

	return g_strdup_printf(_("%02dh%02dm%02ds"), h, m, s);
}

gchar*
format_time2(gint seconds)
{
	const gint s = seconds % 60;
	const gint m = ((seconds - s) / 60)%60;
	const gint h = (seconds - m*60 - s) / (60*60);

	return g_strdup_printf(_("%02d:%02d:%02d"), h, m, s);
}

gchar*
format_size(glong size)
{
	if(size < 1){
		return g_strdup_printf(_("%ld byte"), size);
	}else if(size < 1000){
		return g_strdup_printf(_("%ld bytes"), size);
	}else if(size < 1000000){
		return g_strdup_printf(_("%1.1f kB"), size/1000.0);		
	}else if(size < 1000000000){
		return g_strdup_printf(_("%1.1f MB"), size/1000000.0);	
	}else{
		return g_strdup_printf(_("%1.1f GB"), size/1000000000.0);		
	}
}

gchar*
get_recording_options(FreetuxTVApp *app, gchar* base_filename, gboolean from_preview, gchar **out_filename)
{
	gchar *text;

	gchar *dir;
	gchar *mux;
	gchar *transcode_options;
	gchar *transcode;
	
	GtkWidget* widget;

	gint transcoding_mode;
	gchar *transcoding_format;


	GtkTreeModel *model;
	GtkTreeIter iter;

	model = (GtkTreeModel *) gtk_builder_get_object (app->gui, "liststore_transcodeformat");

	if(from_preview){		

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_directoryrecordings");	
		dir = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (widget));

		if(dir == NULL){
			dir = g_strdup(app->prefs.directoryrecordings);			
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_radiotranscodingno");	
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			transcoding_mode = 0;
		}

		widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogpreferences_radiotranscodingpredefinedformats");		
		if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget))){
			transcoding_mode = 1;
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
								       "dialogpreferences_transcodingformats");
			if(gtk_combo_box_get_active_iter (GTK_COMBO_BOX(widget), &iter)){
				transcoding_format = gtk_tree_model_get_string_from_iter (model, &iter);
			}else{
				transcoding_format = g_strdup(app->prefs.transcoding_format);	
			}
		}		

	}else{
		dir = g_strdup(app->prefs.directoryrecordings);
		transcoding_mode = app->prefs.transcoding_mode;
		
		if(transcoding_mode == 1){
			transcoding_format =  g_strdup(app->prefs.transcoding_format);
		}
	}

	switch(transcoding_mode){
	case 0:
		mux = g_strdup("ts");
		transcode = g_strdup("");
		break;
	case 1:
		gtk_tree_model_get_iter_from_string (model, &iter, transcoding_format);

		gtk_tree_model_get (model, &iter, 1, &transcode_options, 2, &mux, -1);
		
		transcode = g_strdup_printf("transcode{%s}:", transcode_options);
		g_free(transcode_options);
		g_free(transcoding_format);
		break;
	}

	if(out_filename != NULL){
		*out_filename = g_strdup_printf("%s.%s", base_filename, mux);	
	}

	text = g_strdup_printf(":sout=#%sduplicate{dst=std{access=file,mux=%s,dst='%s/%s.%s'},dst=display}", transcode, mux, dir, base_filename, mux);

	g_free(dir);
	g_free(mux);
	
	return text;
}
