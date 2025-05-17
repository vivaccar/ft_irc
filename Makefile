SRCS = srcs/main.cpp srcs/Server.cpp srcs/Client.cpp srcs/Channel.cpp \
		srcs/Commands/pass.cpp srcs/Commands/nick.cpp srcs/Commands/user.cpp srcs/Commands/join.cpp \
		srcs/Commands/topic.cpp srcs/Commands/mode.cpp srcs/Utils/utils.cpp srcs/Commands/kick.cpp \
		srcs/Commands/invite.cpp srcs/Commands/privmsg.cpp srcs/Commands/who.cpp srcs/Bonus/numbersApi.cpp 		

OBJS = $(SRCS:.cpp=.o)

NAME = ircserv

CC = c++
CPP_FLAGS = -Wall -Wextra -Werror -std=c++98

TEST_SRC = tester/tester.cpp
TEST_NAME = irc_tester

GREEN = \e[1;32m
BLUE = \e[0;034m
RED = \e[0;31m
MAGENTA = \033[1;35m
ORANGE = \033[1;38;5;208m
GREY = \033[0;37m
CYAN = \e[0;36m
RESET = \e[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(ORANGE)GENERATING EXECUTABLE FILE ircserv$(RESET)\n"
	@$(CC) $(CPP_FLAGS) $(OBJS) -o $(NAME)
	@echo "$(MAGENTA)╔═══════════════════════════════════════╗"
	@echo           "║      WELCOME TO FT_IRC PROJECT        ║"
	@echo           "║  DEVELOPED BY THE 42 PORTO STUDENTS   ║"
	@echo           "║     aconceic - mfassbin - vivaccar    ║"
	@echo           "╚═══════════════════════════════════════╝\n"
	@echo "$(GREEN)Run ./ircserv <port> <password> to start your server and have fun!$(RESET)"

$(OBJS): %.o: %.cpp	
	@echo "$(BLUE)COMPILING: $<$(RESET)"
	@$(CC) $(CPP_FLAGS) -c $< -o $@

clean:
	@echo "$(CYAN)Removing objects$(RESET)"
	@rm -f $(OBJS)

fclean: clean
	@echo "$(RED)Removing executable$(RESET)	"
	@rm -f $(NAME) $(TEST_NAME)

re: fclean all

$(TEST_NAME) : $(TEST_SRC)
	@echo "$(ORANGE)GENERATING TESTS$(RESET)\n"
	@$(CC) $(TEST_SRC) -o $(TEST_NAME)

test: $(TEST_NAME)
	@echo "$(MAGENTA)EXECUTING TESTS$(RESET)\n"
	@./$(TEST_NAME)

.PHONY: all clean fclean re