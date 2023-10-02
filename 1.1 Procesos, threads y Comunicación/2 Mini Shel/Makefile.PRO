NAME  = shell
OBJS  = 
FLAGS = -Wall

$(TARGET) : $(OBJS)
	gcc $(FLAGS) $(OBJS) -o $(NAME)


%.o : %.c
	gcc -c $(FLAGS) $< -o $@

.PHONY: clean
clean :
	rm -f *.o $(NAME)
