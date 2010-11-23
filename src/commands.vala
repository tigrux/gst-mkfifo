[CCode (has_target = false)]
delegate void CommandFunction(string line);

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
    {"eos", command_eos},
    {"exit", command_exit},
    {null, null}
};


void command_parse(string line) {
    try {
        pipeline = Gst.parse_launch(line);
    }
    catch {
        printerr("Could not parse the pipeline '%s'\n", line);
    }
    if(pipeline != null) {
        Gst.Bus bus = pipeline.get_bus();
        bus.add_signal_watch();
        bus.message["eos"].connect(on_bus_message_eos);
        bus.message["error"].connect(on_bus_message_error);
    }
}


void command_play(string? line)
requires(pipeline != null) {
    pipeline.set_state(Gst.State.PLAYING);
}


void command_pause(string? line)
requires(pipeline != null) {
    pipeline.set_state(Gst.State.PAUSED);
}


void command_ready(string? line)
requires(pipeline != null) {
    pipeline.set_state(Gst.State.READY);
}


void command_null(string? line)
requires(pipeline != null) {
    pipeline.set_state(Gst.State.NULL);
}


void command_seek(string line) {
    int64 useconds = (int64)(line.to_double() * Gst.SECOND);

    var seek_event =
        new Gst.Event.seek(
            1.0, Gst.Format.TIME,
            Gst.SeekFlags.FLUSH | Gst.SeekFlags.KEY_UNIT,
            Gst.SeekType.SET, useconds,
            Gst.SeekType.NONE, 0);
    pipeline.send_event(seek_event);
}


void command_eos(string? line)
requires(pipeline != null) {
    pipeline.send_event(new Gst.Event.eos());
}


void command_exit(string? line)
requires(loop != null) {
    loop.quit();
}

