NAME       = ircserv
CXX        = c++
RM         = rm -f

RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
RESET	= \033[0;37m
SKY		= \033[1;36m

SRC_DIRS   = ./user \
             ./server/channel \
             ./server/eventhandler \
             ./server/irc_server \
             ./exceptions \
             ./commands \
             ./executor \
			 ./main \
			 ./parsing \
			 ./helpers \
			 ./constants \
			 ./usertable

INCLUDES   = $(addprefix -I, $(SRC_DIRS) .)

SRCS       = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS       = $(SRCS:.cpp=.o)

CXXERRFLAGS = #-Wall -Wextra -Werror
CXXFLAGS   = $(CXXERRFLAGS) -std=c++98 $(INCLUDES)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(OBJS) -o $@
	@echo "$(GREEN)The executable file is ready$(RESET)"

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "$(YELLOW)The object file is compiled$(RESET)"

clean:
	@$(RM) $(OBJS)
	@echo "$(RED)The object file is removed$(RESET)"


fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)The executable file is removed$(RESET)"

re: fclean all

push:
	@read -p "Input the commit message: " msg; \
	git add .; \
	git commit -m "$$msg"; \
	git push

leaks:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-limit=no ./$(NAME) 6667 hello

.PHONY: all clean fclean re push leaks