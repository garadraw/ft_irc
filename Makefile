CC			= c++
CFLAGS		= -Wall -Werror -Wextra -std=c++98 -ggdb3 -fsanitize=address -g -Og
RM			= rm -f
NAME		= ircserv

SRC			= main.cpp \
				server.cpp \
				user.cpp \
				channel.cpp \
				commands.cpp \
				CommanHandler.cpp \
				responses.cpp

# OBJ			= $(SRC:cpp=o)

all:		$(NAME)

$(NAME):	#$(OBJ)
			$(CC) $(SRC) $(CFLAGS) -o $(NAME)

#%.o: %.cpp
#	$(CC) $(CFLAGS) $^ -o $@

clean:
			$(RM) $(OBJ)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re