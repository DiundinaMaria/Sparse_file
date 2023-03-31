all:
		gcc test1.c -o myprogram
		gcc test1.c -DBUF_SIZE=100 -o block100
		gcc create_file.c -o input_file

