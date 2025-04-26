CC = c++

CLASSPATH = ./user/ ./server/channel/ ./server/eventhandler/ ./server/irc_server/ ./exceptions/ ./commands/ ./cmdExecutor/
SRCS = $(wildcard $(CLASSPATH)*.cpp)
OBJS = $(patsubst $(CLASSPATH)%.cpp, $(CLASSPATH)%.o, $(SRCS))
INCLUDES = $(foreach H, $(CLASSPATH), -I$(H))

RM = rm -rf

NAME = ircserv
CFLAGS = -Wall -Wextra -Werror -std=c++98 $(INCLUDES)

all: $(NAME)


$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(SRCSPATH)%.o: $(SRCSPATH)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
