COMPILER = c++
FLAGS = -Wall -Werror -Wextra -std=c++98

NAME = ircserv

INC_DIR = include
INC_FILES =	Server.hpp AntiSpamBot.hpp ServerConfig.hpp OperatorList.hpp \
			Client.hpp Channel.hpp ChannelMode.hpp Request.hpp \
			response_codes.hpp utils.hpp commands.hpp modes.hpp
HEADERS = $(addprefix $(INC_DIR)/, $(INC_FILES))
LIB = -I$(INC_DIR)

# COMMAND CATEGORIES
CONNECTION_COM_DIR = connections
CONNECTION_COM_FILES = NICK.cpp OPER.cpp PASS.cpp QUIT.cpp USER.cpp
CONNECTION_COM = $(addprefix $(CONNECTION_COM_DIR)/, $(CONNECTION_COM_FILES))

CHANNEL_COM_DIR = channels
CHANNEL_COM_FILES =	JOIN.cpp PART.cpp MODE.cpp MODE_channel.cpp MODE_user.cpp \
					TOPIC.cpp NAMES.cpp LIST.cpp INVITE.cpp KICK.cpp
CHANNEL_COM = $(addprefix $(CHANNEL_COM_DIR)/, $(CHANNEL_COM_FILES))

## MODE OPTIONS
MODES_COM_DIR = modes
MODES_COM_FILES =	mode_i.cpp mode_k.cpp mode_l.cpp mode_n.cpp mode_o.cpp \
					mode_t.cpp
MODES_COM = $(addprefix $(MODES_COM_DIR)/, $(MODES_COM_FILES))
##

MESSAGE_COM_DIR = messages
MESSAGE_COM_FILES = PRIVMSG.cpp
MESSAGE_COM = $(addprefix $(MESSAGE_COM_DIR)/, $(MESSAGE_COM_FILES))

SERVER_COM_DIR = servers
SERVER_COM_FILES = TIME.cpp VERSION.cpp
SERVER_COM = $(addprefix $(SERVER_COM_DIR)/, $(SERVER_COM_FILES))

USERS_COM_DIR = users
USERS_COM_FILES = WHO.cpp WHOIS.cpp AWAY.cpp
USERS_COM = $(addprefix $(USERS_COM_DIR)/, $(USERS_COM_FILES))


# 
COMMANDS_DIR = commands
COMMANDS_FILES =	$(CONNECTION_COM) $(CHANNEL_COM) $(MODES_COM) \
					$(MESSAGE_COM) $(SERVER_COM) $(USERS_COM)
COMMANDS = $(addprefix $(COMMANDS_DIR)/, $(COMMANDS_FILES))

#
CLASSES_DIR = classes
CLASSES_FILES =	Server.cpp AntiSpamBot.cpp ServerConfig.cpp OperatorList.cpp \
				Client.cpp Channel.cpp ChannelMode.cpp Request.cpp
CLASSES = $(addprefix $(CLASSES_DIR)/, $(CLASSES_FILES))

SRC_DIR = src
SRC_FILES = $(COMMANDS) $(CLASSES) utils.cpp
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ = $(SRC:.cpp=.o)

%.o: %.cpp
	@echo "\033[0;32mCompiling $(notdir $@)\033[0m" 
	@$(COMPILER) $(FLAGS) -c -o $@ $< $(LIB) -DDEBUG_MODE

$(NAME): $(OBJ) $(HEADERS)
	@echo "\033[0;32mCompiling $(NAME)\033[0m"
	@$(COMPILER) $(FLAGS) -o $(NAME) $(OBJ) main.cpp $(LIB)
	@echo "\033[0;32mCompilation completed\033[0m"

all: $(NAME)

clean:
	@echo "\033[0;31mCleaning objects...\033[0m"
	@rm -rf $(OBJ)

fclean: clean
	@echo "\033[0;31mCleaning $(NAME)...\033[0m"
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
