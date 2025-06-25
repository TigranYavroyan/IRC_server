NAME       = ircserv
CXX        = c++
RM         = rm -f

RED        = \033[1;31m
GREEN      = \033[1;32m
YELLOW     = \033[1;33m
RESET      = \033[0;37m
SKY        = \033[1;36m
PORT       = 6667
PASS       = aa

SRC_DIRS   = ./user \
             ./server/channel \
             ./server/eventhandler \
             ./server/irc_server \
             ./exceptions \
             ./commands \
             ./executor \
             ./main \
             ./inputvalidator \
             ./helpers \
             ./constants \
             ./usertable \
             ./typedefs \
			 ./replies \
			 ./logger

INCLUDES   = $(addprefix -I, $(SRC_DIRS) .)

BUILD_DIR  = build

SRCS       = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
OBJS       = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

CXXERRFLAGS = -g3 -Wall -Wextra -Werror
DEBUG       = #-fsanitize=address
CXXFLAGS    = $(CXXERRFLAGS) -std=c++98 $(INCLUDES)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(DEBUG) $^ -o $@
	@echo "$(GREEN) - The executable file is ready$(RESET)"

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "$(YELLOW) - Compiled $< → $@$(RESET)"

clean:
	@$(RM) $(OBJS)
	@echo "$(RED) - The object files are removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED) - The executable file is removed$(RESET)"

re: fclean all

push:
	@read -p "Input the commit message: " msg; \
	git add .; \
	git commit -m "$$msg"; \
	git push

leaks: $(NAME)
	@echo "$(SKY) - Leaks check with valgrind$(RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-limit=no ./$(NAME) $(PORT) $(PASS)

test: $(NAME)
	@echo "$(SKY)---------------------- Test ----------------------$(RESET)"
	@./$(NAME) $(PORT) $(PASS)

.PHONY: all clean fclean re push leaks test