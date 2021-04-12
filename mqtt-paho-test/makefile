all:
	@echo ""
	@echo "Start compiling......"
	@echo ""
	gcc -o sub sub.c -lpaho-mqtt3cs -lpthread
	gcc -o pub pub.c -lpaho-mqtt3cs -lpthread
	gcc -o pub2 pub2.c -lpaho-mqtt3cs -lpthread
	gcc -o sub2 sub2.c -lpaho-mqtt3cs -lpthread
	@echo "end"
sub:
	gcc -o sub sub.c -lpaho-mqtt3cs -lpthread

pub:
	gcc -o pub pub.c -lpaho-mqtt3cs -lpthread

pub2:
	gcc -o pub2 pub2.c -lpaho-mqtt3cs -lpthread

sub2:
	gcc -o sub2 sub2.c -lpaho-mqtt3cs -lpthread

clean:
	-rm sub pub pub2

