all:
	make -C ./application all
	make -C ./modules all
	cp -f ./application/app /home/uesr/share
	cp -f ./modules/led.ko /home/uesr/share
	
clean:
	make -C ./application clean
	make -C ./modules clean