bool on_channel(IOChannel channel, IOCondition condition)
requires(commands_table != null) {
    string command_name = null;
    if((condition & IOCondition.IN) != 0) {
        string line;
        try {
            channel.read_line(out line, null, null);
        }
        catch {
            printerr("Could not read from channel\n");
            line = null;
        }

        if(line != null) {
            line = line.strip();
            print("Got line '%s'\n", line);
            command_name = pop_string(ref line);
            if(command_name != null)
                exec_command(command_name, line);
        }
    }

    if((condition & IOCondition.HUP) != 0) {
        if(!init_channel())
            exec_command("quit", null);
        return false;
    }

    return true;
}


void on_bus_message_eos()
requires(pipeline != null) {
    pipeline.set_state(Gst.State.NULL);
    exec_command("exit", null);
}


void on_bus_message_error(Gst.Bus bus, Gst.Message message) {
    Error error = null;
    string debug = null;
    message.parse_error(out error, out debug);

    printerr("ERROR from element %s: %s\n", message.src.name, error.message);
    printerr("Debugging info: %s\n", (debug != null) ? debug : "none");
    on_bus_message_eos();
}


void exec_command(string command_name, string? line) {
    CommandFunction function = commands_table.lookup(command_name);
    if(function != null) {
        if(!function(line))
            printerr("Command '%s' failed\n", command_name);
    }
    else
        printerr("No function for command '%s'\n", command_name);
}


string? pop_string(ref string? line) {
    if(line == null)
        return null;
    string[] parts = line.split(" ", 2);
    string head = parts[0];
    if(head != null)
        head = head.strip();
    string tail = parts[1];
    if(tail != null)
        tail = tail.strip();
    line = tail;
    return head;
}

