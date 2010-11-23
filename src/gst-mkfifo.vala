Gst.Element pipeline;
MainLoop loop;
string fifo_path;
HashTable<string, CommandFunction> commands_table;
IOChannel channel;


bool init_channel() {
    if(channel != null) {
        Posix.close(channel.unix_get_fd());
        channel = null;
    }
    
    int fd = Posix.open(fifo_path, Posix.O_NONBLOCK | Posix.O_RDONLY);
    if(fd >= 0) {
        channel = new IOChannel.unix_new(fd);
        channel.add_watch(IOCondition.IN | IOCondition.HUP, on_channel);
        return true;
    }
    else {
        print("Could not open '%s' for reading\n", fifo_path);
        return false;
    }
}


int main(string[] args) {
    Gst.init(ref args);
    init_commands();

    if(args.length != 2) {
        printerr("Usage: %s <fifo>\n", args[0]);
        printerr("Where <fifo> can be written the following commands:\n");
        commands_table.foreach( (key) => {print("  %s\n", (string)key);} );
        return 1;
    }
        

    fifo_path = args[1];
    if(Posix.mkfifo(fifo_path, 0666) != 0) {
        printerr("Could not create the fifo '%s'\n", fifo_path);
        return 1;
    }

    if(!init_channel())
        return 1;

    loop = new MainLoop(null, false);
    loop.run();

    Posix.unlink(fifo_path);
    return 0;
}

