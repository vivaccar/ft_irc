SRCS = srcs/main.cpp srcs/Server.cpp srcs/Client.cpp srcs/commands.cpp srcs/tests.cpp

OBJS = $(SRCS:.cpp=.o)

NAME = ircserv

CC = c++

CPP_FLAGS = -Wall -Wextra -Werror -std=c++98 -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CPP_FLAGS) $(OBJS) -o $(NAME)

$(OBJS): %.o: %.cpp	
	$(CC) $(CPP_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re