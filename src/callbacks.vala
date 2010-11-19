bool on_channel_in(IOChannel channel)
requires(commands_table != null) {
    string line = null;
    try {
        channel.read_line(out line, null, null);
    }
    catch {
        printerr("Could not read from channel\n");
        return true;
    }

    line = line.strip();
    print("Got line '%s'\n", line);
    
    string command_name;
    line = partition(line, out command_name);

    CommandFunction function = commands_table.lookup(command_name);
    if(function != null)
        function(line);
    else
        printerr("No function for command '%s'\n", command_name);
    
    return true;
}


void on_bus_message_eos()
requires(pipeline != null) {
    pipeline.set_state(Gst.State.NULL);
    command_quit(null);
}


void on_bus_message_error(Gst.Bus bus, Gst.Message message) {
    Error error = null;
    string debug = null;
    message.parse_error(out error, out debug);

    printerr("ERROR from element %s: %s\n", message.src.name, error.message);
    printerr("Debugging info: %s\n", (debug != null) ? debug : "none");
    on_bus_message_eos();
}


string partition(string line, out string head) {
    string[] parts = line.split(" ", 2);
    head = parts[0];
    return parts[1];
}
