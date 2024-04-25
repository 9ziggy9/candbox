# COLOR ALIASES
RED     = \033[31m
GREEN   = \033[32m
YELLOW  = \033[33m
BLUE    = \033[34m
MAGENTA = \033[35m
CYAN    = \033[36m
BG      = \033[100m
GRAY    = \033[38;5;244m
RESET   = \033[0m

# colored output function
define print_in_color
	@printf "$1"
	@printf "$2"
	@printf "\033[0m"
endef
