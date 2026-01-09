#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'
YELLOW='\033[1;33m'

MINISHELL="./minishell"

# Track results
PASS=0
FAIL=0

make re
run_test() {
    local cmd="$1"
    
    # Run command in Bash
    echo "$cmd" | bash > /dev/null 2>&1
    local bash_exit=$?
    
    # Run command in Minishell
    echo "$cmd" | $MINISHELL > /dev/null 2>&1
    local mini_exit=$?
    
    if [ "$bash_exit" -eq "$mini_exit" ]; then
        echo -e "${GREEN}[PASS]${RESET} Command: \"$cmd\" (Exit: $mini_exit)"
        ((PASS++))
    else
        echo -e "${RED}[FAIL]${RESET} Command: \"$cmd\""
        echo -e "       Expected: $bash_exit | Got: $mini_exit"
        ((FAIL++))
    fi
}

echo -e "${YELLOW}--- Starting Exit Code Tests ---${RESET}"

# 1. Basic Success & Failure
run_test "ls"
run_test "not_a_command"

# 2. Builtins
run_test "exit 42"
run_test "exit 259" # Testing wrap-around (259 % 256)
run_test "cd non_existent_dir"

# 3. Syntax & Path Errors
run_test "ls /root/no_permission"
run_test "cat non_existent_file"

# 4. Logical Operators (if implemented)
run_test "ls && false"
run_test "false || true"

# 5. Pipes and Redirects
run_test "ls | grep nothing"
run_test "cat < non_existent_file"

# New function specifically for signals
run_signal_test() {
    local label="$1"
    local cmd="$2"
    
    # We use perl to trigger specific signals reliably
    echo "$cmd" | bash > /dev/null 2>&1
    local bash_exit=$?
    
    echo "$cmd" | $MINISHELL > /dev/null 2>&1
    local mini_exit=$?
    
    if [ "$bash_exit" -eq "$mini_exit" ]; then
        echo -e "${GREEN}[PASS]${RESET} Signal: $label (Exit: $mini_exit)"
        ((PASS++))
    else
        echo -e "${RED}[FAIL]${RESET} Signal: $label"
        echo -e "       Expected: $bash_exit | Got: $mini_exit"
        ((FAIL++))
    fi
}

echo -e "${YELLOW}--- Starting Signal Tests ---${RESET}"

# SIGINT (Signal 2) -> Expected 130
run_signal_test "SIGINT (Ctrl+C)" "perl -e 'kill 2, \$$'"

# SIGQUIT (Signal 3) -> Expected 131
run_signal_test "SIGQUIT (Ctrl+\\)" "perl -e 'kill 3, \$$'"

# SIGKILL (Signal 9) -> Expected 137
run_signal_test "SIGKILL" "perl -e 'kill 9, \$$'"

# SIGTERM (Signal 15) -> Expected 143
run_signal_test "SIGTERM" "perl -e 'kill 15, \$$'"

# Complex: Signal inside a pipe
# Only the last command's exit code should be captured
run_test "ls | perl -e 'kill 9, \$$'"


echo -e "${YELLOW}--- Starting Builtin Tests ---${RESET}"

# 1. Exit (The most critical builtin)
run_test "exit 0"
run_test "exit 255"
run_test "exit 42"
run_test "exit -1"         # Should wrap to 255
run_test "exit 256"        # Should wrap to 0
run_test "exit abc"        # Non-numeric: Expected 2 in Bash (Numeric argument required)
run_test "exit abc 123"    # Non-numeric with extra args: Expected 2 in Bash
run_test "exit 1 2"        # Too many arguments: Expected 1 in Bash

# 2. CD (Change Directory)
run_test "cd /"            # Success: 0
run_test "cd /nonexistent" # Error: 1
run_test "cd .."           # Success: 0
run_test "cd /bin/ls"      # Not a directory: 1

# 3. Export & Unset
run_test "export VAR=val"  # Success: 0
run_test "export 123VAR"   # Invalid identifier: 1
run_test "unset 123VAR"    # Invalid identifier: 1
run_test "unset PATH"      # Success: 0

# 4. PWD (Print Working Directory)
run_test "pwd"             # Success: 0
run_test "pwd extra_arg"   # Bash ignores args for pwd: 0

# 5. Echo
run_test "echo hello"      # Success: 0
run_test "echo -n test"    # Success: 0

echo -e "${YELLOW}--- Starting Advanced Builtin Error Tests ---${RESET}"

# 1. CD (Change Directory) Errors
run_test "cd"                   # Home not set? (If you unset HOME, this should error)
run_test "cd /tmp/permission"   # No search permission (chmod 000 a dir to test)

# 2. EXIT Errors (The 1 vs 2 distinction)
run_test "exit 9223372036854775808" # Out of range (Long Long Max + 1): Expected 2
run_test "exit --"                  # No arg: Expected 0
run_test "exit +5"                  # Valid numeric: Expected 5
run_test "exit 1 2 3"               # Too many args: Expected 1

# 3. EXPORT / UNSET (Invalid Identifiers)
run_test "export ="                 # Invalid: 1
run_test "export VAR-NAME=val"      # Hyphens not allowed: 1
run_test "export 9VAR=val"          # Starts with digit: 1
run_test "unset VAR!NAME"           # Invalid char: 1

# 4. PWD (Environment interference)
# In Bash, if you unset PWD, pwd still works by looking at the system.
run_test "unset PWD && pwd"         # Expected: 0

# 5. ECHO (Edge cases)
run_test "echo -nnnnnnnn"           # Bash treats multiple 'n's as -n: 0
run_test "echo -n -n -n hello"      # Multiple flags: 0

echo -e "${YELLOW}--- Summary ---${RESET}"
echo -e "Total Passed: ${GREEN}$PASS${RESET}"
echo -e "Total Failed: ${RED}$FAIL${RESET}"

 make fclean

if [ $FAIL -ne 0 ]; then
    exit 1
fi
