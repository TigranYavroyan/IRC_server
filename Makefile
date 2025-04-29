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
			 ./main \
			 ./parsing \
			 ./helpers

INCLUDES   = $(addprefix -I, $(SRC_DIRS) .)

SRCS       = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS       = $(SRCS:.cpp=.o)

CXXERRFLAGS =# -Wall -Wextra -Werror
CXXFLAGS   = $(CXXERRFLAGS) -std=c++98 $(INCLUDES)

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

push:
	@read -p "Input the commit message: " msg; \
	git add .; \
	git commit -m "$$msg"; \
	git push

.PHONY: all clean fclean re push