/* commands.c generated by valac 0.10.0, the Vala compiler
 * generated from commands.vala, do not modify */


#include <glib.h>
#include <glib-object.h>
#include <stdlib.h>
#include <string.h>
#include <gst/gst.h>


#define TYPE_COMMAND (command_get_type ())
typedef struct _Command Command;
#define _g_free0(var) (var = (g_free (var), NULL))
#define _g_hash_table_unref0(var) ((var == NULL) ? NULL : (var = (g_hash_table_unref (var), NULL)))
#define _gst_object_unref0(var) ((var == NULL) ? NULL : (var = (gst_object_unref (var), NULL)))
#define _gst_event_unref0(var) ((var == NULL) ? NULL : (var = (gst_event_unref (var), NULL)))

typedef gboolean (*CommandFunction) (const char* line);
struct _Command {
	char* name;
	CommandFunction function;
};


extern GHashTable* commands_table;
extern GstBin* pipeline;
extern GMainLoop* loop;

GType command_get_type (void) G_GNUC_CONST;
Command* command_dup (const Command* self);
void command_free (Command* self);
void command_copy (const Command* self, Command* dest);
void command_destroy (Command* self);
gboolean command_parse (const char* line);
static gboolean _command_parse_command_function (const char* line);
gboolean command_play (const char* line);
static gboolean _command_play_command_function (const char* line);
gboolean command_pause (const char* line);
static gboolean _command_pause_command_function (const char* line);
gboolean command_ready (const char* line);
static gboolean _command_ready_command_function (const char* line);
gboolean command_null (const char* line);
static gboolean _command_null_command_function (const char* line);
gboolean command_seek (const char* line);
static gboolean _command_seek_command_function (const char* line);
gboolean command_set (const char* line);
static gboolean _command_set_command_function (const char* line);
gboolean command_eos (const char* line);
static gboolean _command_eos_command_function (const char* line);
gboolean command_exit (const char* line);
static gboolean _command_exit_command_function (const char* line);
void init_commands (void);
void on_bus_message_eos (void);
static void _on_bus_message_eos_gst_bus_message (GstBus* _sender, GstMessage* message, gpointer self);
void on_bus_message_error (GstBus* bus, GstMessage* message);
static void _on_bus_message_error_gst_bus_message (GstBus* _sender, GstMessage* message, gpointer self);
char* pop_string (char** line);

const Command commands[10] = {{"parse", _command_parse_command_function}, {"play", _command_play_command_function}, {"pause", _command_pause_command_function}, {"ready", _command_ready_command_function}, {"null", _command_null_command_function}, {"seek", _command_seek_command_function}, {"set", _command_set_command_function}, {"eos", _command_eos_command_function}, {"exit", _command_exit_command_function}, {NULL, NULL}};


void command_copy (const Command* self, Command* dest) {
	dest->name = g_strdup (self->name);
	dest->function = self->function;
}


void command_destroy (Command* self) {
	_g_free0 (self->name);
}


Command* command_dup (const Command* self) {
	Command* dup;
	dup = g_new0 (Command, 1);
	command_copy (self, dup);
	return dup;
}


void command_free (Command* self) {
	command_destroy (self);
	g_free (self);
}


GType command_get_type (void) {
	static volatile gsize command_type_id__volatile = 0;
	if (g_once_init_enter (&command_type_id__volatile)) {
		GType command_type_id;
		command_type_id = g_boxed_type_register_static ("Command", (GBoxedCopyFunc) command_dup, (GBoxedFreeFunc) command_free);
		g_once_init_leave (&command_type_id__volatile, command_type_id);
	}
	return command_type_id__volatile;
}


static gboolean _command_parse_command_function (const char* line) {
	gboolean result;
	result = command_parse (line);
	return result;
}


static gboolean _command_play_command_function (const char* line) {
	gboolean result;
	result = command_play (line);
	return result;
}


static gboolean _command_pause_command_function (const char* line) {
	gboolean result;
	result = command_pause (line);
	return result;
}


static gboolean _command_ready_command_function (const char* line) {
	gboolean result;
	result = command_ready (line);
	return result;
}


static gboolean _command_null_command_function (const char* line) {
	gboolean result;
	result = command_null (line);
	return result;
}


static gboolean _command_seek_command_function (const char* line) {
	gboolean result;
	result = command_seek (line);
	return result;
}


static gboolean _command_set_command_function (const char* line) {
	gboolean result;
	result = command_set (line);
	return result;
}


static gboolean _command_eos_command_function (const char* line) {
	gboolean result;
	result = command_eos (line);
	return result;
}


static gboolean _command_exit_command_function (const char* line) {
	gboolean result;
	result = command_exit (line);
	return result;
}


void init_commands (void) {
	GHashTable* _tmp0_;
	g_return_if_fail (commands_table == NULL);
	commands_table = (_tmp0_ = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL), _g_hash_table_unref0 (commands_table), _tmp0_);
	{
		gint i;
		i = 0;
		{
			gboolean _tmp1_;
			_tmp1_ = TRUE;
			while (TRUE) {
				if (!_tmp1_) {
					i++;
				}
				_tmp1_ = FALSE;
				if (!(commands[i].name != NULL)) {
					break;
				}
				g_hash_table_insert (commands_table, g_strdup (commands[i].name), commands[i].function);
			}
		}
	}
}


static void _on_bus_message_eos_gst_bus_message (GstBus* _sender, GstMessage* message, gpointer self) {
	on_bus_message_eos ();
}


static void _on_bus_message_error_gst_bus_message (GstBus* _sender, GstMessage* message, gpointer self) {
	on_bus_message_error (_sender, message);
}


gboolean command_parse (const char* line) {
	gboolean result = FALSE;
	GstBus* bus;
	GError * _inner_error_ = NULL;
	g_return_val_if_fail (line != NULL, FALSE);
	{
		GstElement* _tmp0_;
		GstBin* _tmp1_;
		_tmp0_ = gst_parse_launch (line, &_inner_error_);
		if (_inner_error_ != NULL) {
			goto __catch1_g_error;
		}
		pipeline = (_tmp1_ = GST_BIN (_tmp0_), _gst_object_unref0 (pipeline), _tmp1_);
	}
	goto __finally1;
	__catch1_g_error:
	{
		g_clear_error (&_inner_error_);
		_inner_error_ = NULL;
		{
			GstBin* _tmp2_;
			pipeline = (_tmp2_ = NULL, _gst_object_unref0 (pipeline), _tmp2_);
		}
	}
	__finally1:
	if (_inner_error_ != NULL) {
		g_critical ("file %s: line %d: uncaught error: %s (%s, %d)", __FILE__, __LINE__, _inner_error_->message, g_quark_to_string (_inner_error_->domain), _inner_error_->code);
		g_clear_error (&_inner_error_);
		return FALSE;
	}
	if (pipeline == NULL) {
		g_printerr ("Could not parse the pipeline '%s'\n", line);
		result = FALSE;
		return result;
	}
	bus = gst_element_get_bus ((GstElement*) pipeline);
	gst_bus_add_signal_watch (bus);
	g_signal_connect (bus, "message::eos", (GCallback) _on_bus_message_eos_gst_bus_message, NULL);
	g_signal_connect (bus, "message::error", (GCallback) _on_bus_message_error_gst_bus_message, NULL);
	result = TRUE;
	_gst_object_unref0 (bus);
	return result;
}


gboolean command_play (const char* line) {
	gboolean result = FALSE;
	GstStateChangeReturn state_change;
	g_return_val_if_fail (pipeline != NULL, FALSE);
	state_change = gst_element_set_state ((GstElement*) pipeline, GST_STATE_PLAYING);
	result = state_change != GST_STATE_CHANGE_FAILURE;
	return result;
}


gboolean command_pause (const char* line) {
	gboolean result = FALSE;
	GstStateChangeReturn state_change;
	g_return_val_if_fail (pipeline != NULL, FALSE);
	state_change = gst_element_set_state ((GstElement*) pipeline, GST_STATE_PAUSED);
	result = state_change != GST_STATE_CHANGE_FAILURE;
	return result;
}


gboolean command_ready (const char* line) {
	gboolean result = FALSE;
	GstStateChangeReturn state_change;
	g_return_val_if_fail (pipeline != NULL, FALSE);
	state_change = gst_element_set_state ((GstElement*) pipeline, GST_STATE_READY);
	result = state_change != GST_STATE_CHANGE_FAILURE;
	return result;
}


gboolean command_null (const char* line) {
	gboolean result = FALSE;
	GstStateChangeReturn state_change;
	g_return_val_if_fail (pipeline != NULL, FALSE);
	state_change = gst_element_set_state ((GstElement*) pipeline, GST_STATE_NULL);
	result = state_change != GST_STATE_CHANGE_FAILURE;
	return result;
}


static gpointer _gst_event_ref0 (gpointer self) {
	return self ? gst_event_ref (self) : NULL;
}


gboolean command_seek (const char* line) {
	gboolean result = FALSE;
	gint direction = 0;
	gint64 useconds;
	gint64 position = 0LL;
	GstEvent* seek_event;
	g_return_val_if_fail (line != NULL, FALSE);
	if (g_str_has_prefix (line, "+")) {
		direction = 1;
	} else {
		if (g_str_has_prefix (line, "-")) {
			direction = -1;
		} else {
			direction = 0;
		}
	}
	if (direction != 0) {
		line = g_utf8_next_char (line);
	}
	useconds = (gint64) (g_ascii_strtod (line, NULL) * GST_SECOND);
	if (direction != 0) {
		GstFormat time_format;
		time_format = GST_FORMAT_TIME;
		if (gst_element_query_position ((GstElement*) pipeline, &time_format, &position)) {
			position = position + (useconds * direction);
		} else {
			g_printerr ("Could not get the current position\n");
			result = FALSE;
			return result;
		}
	} else {
		position = useconds;
	}
	seek_event = gst_event_new_seek (1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE, GST_SEEK_TYPE_SET, position, GST_SEEK_TYPE_NONE, (gint64) 0);
	result = gst_element_send_event ((GstElement*) pipeline, _gst_event_ref0 (seek_event));
	_gst_event_unref0 (seek_event);
	return result;
}


gboolean command_set (const char* line) {
	gboolean result = FALSE;
	char* args;
	char* element_name;
	char* property_name;
	char* value_string;
	GstElement* element;
	GParamSpec* property;
	GType property_type;
	GValue property_value = {0};
	args = g_strdup (line);
	if (args == NULL) {
		g_printerr ("No element given\n");
		result = FALSE;
		_g_free0 (args);
		return result;
	}
	element_name = pop_string (&args);
	if (args == NULL) {
		g_printerr ("No property given\n");
		result = FALSE;
		_g_free0 (element_name);
		_g_free0 (args);
		return result;
	}
	property_name = pop_string (&args);
	if (args == NULL) {
		g_printerr ("No value given\n");
		result = FALSE;
		_g_free0 (property_name);
		_g_free0 (element_name);
		_g_free0 (args);
		return result;
	}
	value_string = pop_string (&args);
	element = gst_bin_get_by_name (pipeline, element_name);
	if (element == NULL) {
		g_printerr ("No element named '%s'\n", element_name);
		result = FALSE;
		_gst_object_unref0 (element);
		_g_free0 (value_string);
		_g_free0 (property_name);
		_g_free0 (element_name);
		_g_free0 (args);
		return result;
	}
	property = g_object_class_find_property (G_OBJECT_GET_CLASS ((GObject*) element), property_name);
	if (property == NULL) {
		g_printerr ("No property named '%s'\n", property_name);
		result = FALSE;
		_gst_object_unref0 (element);
		_g_free0 (value_string);
		_g_free0 (property_name);
		_g_free0 (element_name);
		_g_free0 (args);
		return result;
	}
	property_type = property->value_type;
	g_value_init (&property_value, property_type);
	if (gst_value_deserialize (&property_value, value_string)) {
		g_object_set_property ((GObject*) element, property_name, &property_value);
		result = TRUE;
		_gst_object_unref0 (element);
		_g_free0 (value_string);
		_g_free0 (property_name);
		_g_free0 (element_name);
		_g_free0 (args);
		return result;
	} else {
		g_printerr ("Could not transform value %s to type %s\n", value_string, g_type_name (property_type));
		result = FALSE;
		_gst_object_unref0 (element);
		_g_free0 (value_string);
		_g_free0 (property_name);
		_g_free0 (element_name);
		_g_free0 (args);
		return result;
	}
	_gst_object_unref0 (element);
	_g_free0 (value_string);
	_g_free0 (property_name);
	_g_free0 (element_name);
	_g_free0 (args);
}


gboolean command_eos (const char* line) {
	gboolean result = FALSE;
	g_return_val_if_fail (pipeline != NULL, FALSE);
	result = gst_element_send_event ((GstElement*) pipeline, gst_event_new_eos ());
	return result;
}


gboolean command_exit (const char* line) {
	gboolean result = FALSE;
	g_return_val_if_fail (loop != NULL, FALSE);
	g_main_loop_quit (loop);
	result = TRUE;
	return result;
}




