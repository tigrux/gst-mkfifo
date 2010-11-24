[CCode (has_target = false)]
delegate bool CommandFunction(string line);

struct Command {
    string name;
    CommandFunction function;
}


const Command commands[] = {
    {"parse", command_parse},
    {"play", command_play},
    {"pause", command_pause},
    {"ready", command_ready},
    {"null", command_null},
    {"seek", command_seek},
    {"set", command_set},
    {"eos", command_eos},
    {"exit", command_exit},
    {null, null}
};


void init_commands()
requires(commands_table == null) {
    commands_table = new HashTable<string, CommandFunction> (str_hash, str_equal);
    for(int i=0; commands[i].name != null; i++)
        commands_table.insert(commands[i].name, commands[i].function);
}


bool command_parse(string line) {
    try {
        pipeline = (Gst.Bin)Gst.parse_launch(line);
    }
    catch {
        pipeline = null;
    }

    if(pipeline == null) {
        printerr("Could not parse the pipeline '%s'\n", line);
        return false;
    }
    Gst.Bus bus = pipeline.get_bus();
    bus.add_signal_watch();
    bus.message["eos"].connect(on_bus_message_eos);
    bus.message["error"].connect(on_bus_message_error);
    return true;
}


bool command_play(string? line)
requires(pipeline != null) {
    Gst.StateChangeReturn state_change = pipeline.set_state(Gst.State.PLAYING);
    return state_change != Gst.StateChangeReturn.FAILURE;
}


bool command_pause(string? line)
requires(pipeline != null) {
    Gst.StateChangeReturn state_change = pipeline.set_state(Gst.State.PAUSED);
    return state_change != Gst.StateChangeReturn.FAILURE;
}


bool command_ready(string? line)
requires(pipeline != null) {
    Gst.StateChangeReturn state_change = pipeline.set_state(Gst.State.READY);
    return state_change != Gst.StateChangeReturn.FAILURE;
}


bool command_null(string? line)
requires(pipeline != null) {
    Gst.StateChangeReturn state_change = pipeline.set_state(Gst.State.NULL);
    return state_change != Gst.StateChangeReturn.FAILURE;
}


bool command_seek(string line) {
    int direction;

    if(line.has_prefix("+"))
        direction = 1;
    else
    if(line.has_prefix("-"))
        direction = -1;
    else
        direction = 0;

    if(direction != 0)
        line = line.next_char();

    int64 useconds = (int64)(line.to_double()*Gst.SECOND);
    int64 position;

    if(direction != 0) {
        Gst.Format time_format = Gst.Format.TIME;
        if(pipeline.query_position(ref time_format, out position))
            position += useconds*direction;
        else {
            printerr("Could not get the current position\n");
            return false;
        }
    }
    else
        position = useconds;

    var seek_event =
        new Gst.Event.seek(
            1.0, Gst.Format.TIME,
            Gst.SeekFlags.FLUSH | Gst.SeekFlags.ACCURATE,
            Gst.SeekType.SET, position,
            Gst.SeekType.NONE, 0);
    return pipeline.send_event(seek_event);
}


bool command_set(string ?line) {
    string args = line;

    if(args == null) {
        printerr("No element given\n");
        return false;
    }
    string element_name = pop_string(ref args);

    if(args == null) {
        printerr("No property given\n");
        return false;
    }
    string property_name = pop_string(ref args);

    if(args == null) {
        printerr("No value given\n");
        return false;
    }
    string value_string = pop_string(ref args);

    Gst.Element element = pipeline.get_by_name(element_name);
    if(element == null) {
        printerr("No element named '%s'\n", element_name);
        return false;
    }

    weak ParamSpec property = element.get_class().find_property(property_name);
    if(property == null) {
        printerr("No property named '%s'\n", property_name);
        return false;
    }
    Type property_type = property.value_type;
    Gst.Value property_value = Value(property_type);

    if(property_value.deserialize(value_string)) {
        element.set_property(property_name, property_value);
        return true;
    }
    else {
        printerr("Could not transform value %s to type %s\n",
            value_string, property_type.name());
        return false;
    }
}


bool command_eos(string? line)
requires(pipeline != null) {
    return pipeline.send_event(new Gst.Event.eos());
}


bool command_exit(string? line)
requires(loop != null) {
    loop.quit();
    return true;
}

