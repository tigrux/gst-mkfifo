INPUT=input

if test x$1 = x
then
    echo "Usage: $0 <media file>"
    exit 1
fi

send()
{
    echo $@
    echo $@ > $INPUT
}

./gst-mkfifo $INPUT &
sleep 1

if ! test -p $INPUT
then
    echo "Could not start gst-mkfifo"
fi

send parse playbin2 uri=file://`pwd`/$1
sleep 1

send pause
sleep 1

send play
sleep 5

send pause
sleep 3

send play
sleep 5

send seek 60
sleep 5

send eos

send quit
