#!/bin/bash
# =============================================
# Advanced Minishell Tester
# Senior Developer Level
# Compares Bash vs Minishell, always prints expected/actual
# =============================================

MINISHELL="${1:-./minishell}"

# Temp files
BASH_OUT=$(mktemp)
MINI_OUT=$(mktemp)
BASH_ERR=$(mktemp)
MINI_ERR=$(mktemp)
BASH_EXIT=$(mktemp)
MINI_EXIT=$(mktemp)

# Counters
TEST_COUNT=0
PASS_COUNT=0
FAIL_COUNT=0

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# -------------------------
# Run a single test
# -------------------------
run_test() {
    local name="$1"
    local cmd="$2"
    TEST_COUNT=$((TEST_COUNT+1))
    
    echo -e "\n${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${YELLOW}Test #${TEST_COUNT}: $name${NC}"
    echo -e "${BLUE}Command:${NC} $cmd"

    # Run in Bash
    bash -c "$cmd" > "$BASH_OUT" 2> "$BASH_ERR"
    echo $? > "$BASH_EXIT"

    # Run in Minishell, strip prompts
    echo "$cmd" | $MINISHELL > "$MINI_OUT" 2> "$MINI_ERR"
    echo $? > "$MINI_EXIT"
    sed -i '/^MS\$ /d' "$MINI_OUT"
    sed -i '/^MS\$ exit/d' "$MINI_OUT"

    # Compare
    local stdout_match=0
    local stderr_match=0
    local exit_match=0
    diff -q "$BASH_OUT" "$MINI_OUT" >/dev/null && stdout_match=1
    diff -q "$BASH_ERR" "$MINI_ERR" >/dev/null && stderr_match=1
    diff -q "$BASH_EXIT" "$MINI_EXIT" >/dev/null && exit_match=1

    # Always print expected vs actual
    echo -e "${YELLOW}Expected stdout (bash):${NC}"; cat "$BASH_OUT"
    echo -e "${YELLOW}Actual stdout (minishell):${NC}"; cat "$MINI_OUT"
    echo -e "${YELLOW}Expected stderr (bash):${NC}"; cat "$BASH_ERR"
    echo -e "${YELLOW}Actual stderr (minishell):${NC}"; cat "$MINI_ERR"
    echo -e "${YELLOW}Expected exit code (bash):${NC} $(cat $BASH_EXIT)"
    echo -e "${YELLOW}Actual exit code (minishell):${NC} $(cat $MINI_EXIT)"

    if [ $stdout_match -eq 1 ] && [ $stderr_match -eq 1 ] && [ $exit_match -eq 1 ]; then
        echo -e "${GREEN}✓ Passed${NC}"
        PASS_COUNT=$((PASS_COUNT+1))
    else
        echo -e "${RED}✗ Failed${NC}"
        FAIL_COUNT=$((FAIL_COUNT+1))
    fi
}

# -------------------------
# Environment
# -------------------------
export TESTVAR="value"
export EMPTY_VAR=""
touch file.txt in.txt out.txt err.txt
echo "Hello World" > file.txt
echo "Input data" > in.txt

# -------------------------
# TEST CASES
# -------------------------

# -------------------------
# Echo / Variables / Quotes
# -------------------------
run_test "echo simple" "echo hello"
run_test "echo double quotes" "echo \"hello world\""
run_test "echo single quotes" "echo 'hello world'"
run_test "variable expansion simple" "echo \$TESTVAR"
run_test "variable expansion with text" "echo hello\$TESTVAR world"
run_test "nonexistent variable" "echo \$NONEXISTENT"
run_test "mixed quotes and variable" "echo \"\$TESTVAR is '\$TESTVAR'\""
run_test "quotes in middle of words" "echo hel'lo'wor\"ld\""

# -------------------------
# Wildcards
# -------------------------
touch test1.txt test2.txt
run_test "wildcard *" "echo *.txt"
run_test "wildcard with text" "echo file*.txt"

# -------------------------
# Redirections
# -------------------------
run_test "stdout redirect" "echo hello > out.txt"
run_test "stderr redirect" "ls nonexistent 2>err.txt"
run_test "append redirect" "echo hi >> out.txt"
run_test "stdin redirect" "cat < file.txt"
run_test "complex redirect" "ls file.txt lame > result.txt 2>out.txt >> outt.txt"

# -------------------------
# Pipes / Logical Operators
# -------------------------
run_test "simple pipe" "echo hello | cat"
run_test "pipe chain" "echo hello | cat | cat"
run_test "AND operator success" "echo ok && echo yes"
run_test "AND operator fail" "ls nope && echo yes"
run_test "OR operator success" "echo ok || echo fail"
run_test "OR operator fail" "ls nope || echo recovered"
run_test "combined AND/OR" "echo ok && ls nope || echo fallback"

# -------------------------
# Subshells / Parentheses
# -------------------------
run_test "simple subshell" "(echo hello)"
run_test "nested subshell" "((echo nested))"
run_test "subshell with pipe" "(echo hello) | cat"
run_test "subshell with logical" "(echo ok && echo yes)"

# -------------------------
# Heredocs
# -------------------------
run_test "simple heredoc" "cat << EOF
hello
world
EOF"
run_test "heredoc with pipe" "cat << EOF | cat
hello heredoc
EOF"
run_test "heredoc with logical" "cat << EOF && echo done
hi
EOF"
run_test "heredoc subshell" "(cat << EOF)
subshell
EOF"

# -------------------------
# Built-ins
# -------------------------
run_test "pwd after cd /tmp" "cd /tmp && pwd"
run_test "cd home" "cd ~ && pwd"
run_test "cd invalid" "cd /nonexistent"
run_test "export variable" "export NEWVAR=hello && echo \$NEWVAR"
run_test "export empty" "export EMPTYVAR="

# -------------------------
# Edge Cases
# -------------------------
run_test "empty input" ""
run_test "only spaces" "   "
run_test "only operators" "|||| &&&& >><<"

# -------------------------
# Summary
# -------------------------
echo -e "\n${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${YELLOW}TEST SUMMARY${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "Total tests: $TEST_COUNT"
echo -e "${GREEN}Passed: $PASS_COUNT${NC}"
echo -e "${RED}Failed: $FAIL_COUNT${NC}"

# Cleanup
rm -f file.txt in.txt out.txt err.txt test1.txt test2.txt outt.txt result.txt
rm -f "$BASH_OUT" "$MINI_OUT" "$BASH_ERR" "$MINI_ERR" "$BASH_EXIT" "$MINI_EXIT"

exit $FAIL_COUNT

