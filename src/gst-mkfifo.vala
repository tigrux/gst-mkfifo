Gst.Bin pipeline;
MainLoop loop;
string fifo_path;
IOChannel fifo_channel;
HashTable<string, CommandFunction> commands_table;
HashTable<string, string> command_descriptions_table;


bool init_channel() {
    if(fifo_channel != null) {
        Posix.close(fifo_channel.unix_get_fd());
        fifo_channel = null;
    }

    int fifo_fd = Posix.open(fifo_path, Posix.O_NONBLOCK | Posix.O_RDONLY);
    if(fifo_fd >= 0) {
        fifo_channel = new IOChannel.unix_new(fifo_fd);
        fifo_channel.add_watch(
            IOCondition.IN | IOCondition.HUP, on_fifo_channel);
        return true;
    }
    else {
        printerr("Could not open '%s' for reading\n", fifo_path);
        return false;
    }
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


int main(string[] args) {
    Gst.init(ref args);
    init_commands();

    if(args.length != 2) {
        print("Usage: %s <fifo>\n", args[0]);
        print("Where <fifo> can be written the following commands:\n");
        command_descriptions_table.foreach(
            (k,v) => print("\t%s %s\n", (string)k, (string)v ?? "")
        );
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

