To start idf.py run - source ~/.zshrc
To find the port - ls /dev/tty.*
steps:-
1. cd to you project folder
2. run - idf.py set-target esp32s3
3. run - idf.py menuconfig
4. run - idf.py build
5. run - idf.py -p /dev/cu.usbmodemXXXX flash
6. 