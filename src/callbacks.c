/* callbacks.c generated by valac 0.10.0, the Vala compiler
 * generated from callbacks.vala, do not modify */


#include <glib.h>
#include <glib-object.h>
#include <stdlib.h>
#include <string.h>
#include <gst/gst.h>

#define _g_free0(var) (var = (g_free (var), NULL))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))

typedef gboolean (*CommandFunction) (const char* line);

extern GHashTable* commands_table;
extern GMainLoop* loop;
extern GstBin* pipeline;

gboolean on_fifo_channel (GIOChannel* channel, GIOCondition condition);
char* pop_string (char** line);
gboolean exec_command (const char* command_name, const char* line);
gboolean init_channel (void);
void on_bus_message_eos (void);
void on_bus_message_error (GstBus* bus, GstMessage* message);



static char* string_strip (const char* self) {
	char* result = NULL;
	char* _result_;
	g_return_val_if_fail (self != NULL, NULL);
	_result_ = g_strdup (self);
	g_strstrip (_result_);
	result = _result_;
	return result;
}


gboolean on_fifo_channel (GIOChannel* channel, GIOCondition condition) {
	gboolean result = FALSE;
	char* command_name;
	GError * _inner_error_ = NULL;
	g_return_val_if_fail (channel != NULL, FALSE);
	g_return_val_if_fail (commands_table != NULL, FALSE);
	command_name = NULL;
	if ((condition & G_IO_IN) != 0) {
		char* line;
		line = NULL;
		{
			char* _tmp0_ = NULL;
			char* _tmp1_;
			g_io_channel_read_line (channel, &_tmp0_, NULL, NULL, &_inner_error_);
			line = (_tmp1_ = _tmp0_, _g_free0 (line), _tmp1_);
			if (_inner_error_ != NULL) {
				goto __catch0_g_error;
			}
		}
		goto __finally0;
		__catch0_g_error:
		{
			g_clear_error (&_inner_error_);
			_inner_error_ = NULL;
			{
				char* _tmp2_;
				g_printerr ("Could not read from channel\n");
				line = (_tmp2_ = NULL, _g_free0 (line), _tmp2_);
			}
		}
		__finally0:
		if (_inner_error_ != NULL) {
			_g_free0 (line);
			_g_free0 (command_name);
			g_critical ("file %s: line %d: uncaught error: %s (%s, %d)", __FILE__, __LINE__, _inner_error_->message, g_quark_to_string (_inner_error_->domain), _inner_error_->code);
			g_clear_error (&_inner_error_);
			return FALSE;
		}
		if (line != NULL) {
			char* _tmp3_;
			char* _tmp4_;
			line = (_tmp3_ = string_strip (line), _g_free0 (line), _tmp3_);
			g_print ("Got line '%s'\n", line);
			command_name = (_tmp4_ = pop_string (&line), _g_free0 (command_name), _tmp4_);
			if (command_name != NULL) {
				exec_command (command_name, line);
			}
		}
		_g_free0 (line);
	}
	if ((condition & G_IO_HUP) != 0) {
		if (!init_channel ()) {
			g_main_loop_quit (loop);
		}
		result = FALSE;
		_g_free0 (command_name);
		return result;
	}
	result = TRUE;
	_g_free0 (command_name);
	return result;
}


void on_bus_message_eos (void) {
	g_return_if_fail (pipeline != NULL);
	gst_element_set_state ((GstElement*) pipeline, GST_STATE_NULL);
	exec_command ("exit", NULL);
}


void on_bus_message_error (GstBus* bus, GstMessage* message) {
	GError* _error_;
	char* debug;
	GError* _tmp0_ = NULL;
	GError* _tmp1_;
	char* _tmp2_ = NULL;
	char* _tmp3_;
	char* _tmp4_;
	g_return_if_fail (bus != NULL);
	g_return_if_fail (message != NULL);
	_error_ = NULL;
	debug = NULL;
	(gst_message_parse_error (message, &_tmp0_, &_tmp2_), _error_ = (_tmp1_ = _tmp0_, _g_error_free0 (_error_), _tmp1_));
	debug = (_tmp3_ = _tmp2_, _g_free0 (debug), _tmp3_);
	g_printerr ("ERROR from element %s: %s\n", gst_object_get_name (message->src), _error_->message);
	_tmp4_ = g_strdup (debug);
	if (_tmp4_ == NULL) {
		char* _tmp5_;
		_tmp4_ = (_tmp5_ = g_strdup ("none"), _g_free0 (_tmp4_), _tmp5_);
	}
	g_printerr ("Debugging info: %s\n", _tmp4_);
	on_bus_message_eos ();
	_g_free0 (_tmp4_);
	_g_free0 (debug);
	_g_error_free0 (_error_);
}




