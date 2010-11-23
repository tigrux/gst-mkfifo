/* gst-mkfifo.c generated by valac 0.10.0, the Vala compiler
 * generated from gst-mkfifo.vala, do not modify */


#include <glib.h>
#include <glib-object.h>
#include <gst/gst.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define _g_io_channel_unref0(var) ((var == NULL) ? NULL : (var = (g_io_channel_unref (var), NULL)))
#define _g_hash_table_unref0(var) ((var == NULL) ? NULL : (var = (g_hash_table_unref (var), NULL)))

#define TYPE_COMMAND (command_get_type ())
typedef struct _Command Command;
#define _g_free0(var) (var = (g_free (var), NULL))
#define _g_main_loop_unref0(var) ((var == NULL) ? NULL : (var = (g_main_loop_unref (var), NULL)))

typedef void (*CommandFunction) (const char* line);
struct _Command {
	char* name;
	CommandFunction function;
};


extern GstElement* pipeline;
GstElement* pipeline = NULL;
extern GMainLoop* loop;
GMainLoop* loop = NULL;
extern char* fifo_path;
char* fifo_path = NULL;
extern GHashTable* commands_table;
GHashTable* commands_table = NULL;
extern GIOChannel* channel;
GIOChannel* channel = NULL;

gboolean init_channel (void);
gboolean on_channel (GIOChannel* channel, GIOCondition condition);
static gboolean _on_channel_gio_func (GIOChannel* source, GIOCondition condition, gpointer self);
gint _vala_main (char** args, int args_length1);
GType command_get_type (void) G_GNUC_CONST;
Command* command_dup (const Command* self);
void command_free (Command* self);
void command_copy (const Command* self, Command* dest);
void command_destroy (Command* self);
void command_parse (const char* line);
static void _command_parse_command_function (const char* line);
void command_play (const char* line);
static void _command_play_command_function (const char* line);
void command_pause (const char* line);
static void _command_pause_command_function (const char* line);
void command_ready (const char* line);
static void _command_ready_command_function (const char* line);
void command_null (const char* line);
static void _command_null_command_function (const char* line);
void command_seek (const char* line);
static void _command_seek_command_function (const char* line);
void command_eos (const char* line);
static void _command_eos_command_function (const char* line);
void command_exit (const char* line);
static void _command_exit_command_function (const char* line);

extern const Command commands[9];


static gboolean _on_channel_gio_func (GIOChannel* source, GIOCondition condition, gpointer self) {
	gboolean result;
	result = on_channel (source, condition);
	return result;
}


gboolean init_channel (void) {
	gboolean result = FALSE;
	gint fd;
	if (channel != NULL) {
		GIOChannel* _tmp0_;
		close (g_io_channel_unix_get_fd (channel));
		channel = (_tmp0_ = NULL, _g_io_channel_unref0 (channel), _tmp0_);
	}
	fd = open (fifo_path, O_NONBLOCK | O_RDONLY, 0);
	if (fd >= 0) {
		GIOChannel* _tmp1_;
		channel = (_tmp1_ = g_io_channel_unix_new (fd), _g_io_channel_unref0 (channel), _tmp1_);
		g_io_add_watch (channel, G_IO_IN | G_IO_HUP, _on_channel_gio_func, NULL);
		result = TRUE;
		return result;
	} else {
		g_print ("Could not open '%s' for reading\n", fifo_path);
		result = FALSE;
		return result;
	}
}


static void _command_parse_command_function (const char* line) {
	command_parse (line);
}


static void _command_play_command_function (const char* line) {
	command_play (line);
}


static void _command_pause_command_function (const char* line) {
	command_pause (line);
}


static void _command_ready_command_function (const char* line) {
	command_ready (line);
}


static void _command_null_command_function (const char* line) {
	command_null (line);
}


static void _command_seek_command_function (const char* line) {
	command_seek (line);
}


static void _command_eos_command_function (const char* line) {
	command_eos (line);
}


static void _command_exit_command_function (const char* line) {
	command_exit (line);
}


gint _vala_main (char** args, int args_length1) {
	gint result = 0;
	GHashTable* _tmp0_;
	char* _tmp2_;
	GMainLoop* _tmp3_;
	gst_init (&args_length1, &args);
	if (args_length1 != 2) {
		g_printerr ("Usage: %s <fifo>\n", args[0]);
		result = 1;
		return result;
	}
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
	fifo_path = (_tmp2_ = g_strdup (args[1]), _g_free0 (fifo_path), _tmp2_);
	if (mkfifo (fifo_path, (mode_t) 0666) != 0) {
		g_printerr ("Could not create the fifo '%s'\n", fifo_path);
		result = 1;
		return result;
	}
	if (!init_channel ()) {
		result = 1;
		return result;
	}
	loop = (_tmp3_ = g_main_loop_new (NULL, FALSE), _g_main_loop_unref0 (loop), _tmp3_);
	g_main_loop_run (loop);
	unlink (fifo_path);
	result = 0;
	return result;
}


int main (int argc, char ** argv) {
	g_thread_init (NULL);
	g_type_init ();
	return _vala_main (argv, argc);
}




