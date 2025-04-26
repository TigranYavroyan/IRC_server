NAME       = ircserv
CXX        = c++
RM         = rm -f

SRC_DIRS   = ./user \
             ./server/channel \
             ./server/eventhandler \
             ./server/irc_server \
             ./exceptions \
             ./commands \
             ./cmdExecutor \
			 ./main

INCLUDES   = $(addprefix -I, $(SRC_DIRS) .)

SRCS       = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS       = $(SRCS:.cpp=.o)

CXXFLAGS   = -Wall -Wextra -Werror -std=c++98 $(INCLUDES)


all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

info:
	$(info Sources: $(SRCS))
	$(info Includes: $(INCLUDES))

.PHONY: all clean fclean re info