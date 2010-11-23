Gst.Element pipeline;
MainLoop loop;
string fifo_path;
HashTable<string, CommandFunction> commands_table;

bool init_channel() {
    int fd = Posix.open(fifo_path, Posix.O_NONBLOCK | Posix.O_RDONLY);
    if(fd >= 0) {
        IOChannel channel = new IOChannel.unix_new(fd);
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

    if(args.length != 2) {
        printerr("Usage: %s <fifo>\n", args[0]);
        return 1;
    }
        
    commands_table = new HashTable<string, CommandFunction> (str_hash, str_equal);
    for(int i=0; commands[i].name != null; i++)
        commands_table.insert(commands[i].name, commands[i].function);

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

